#include "unity.h"
#include "voice.h"

// CMock generated files
#include "mock_envelope.h"
#include "mock_oscillator.h"

static void run_updates(int updates);
static void expect_new_note_started(uint32_t freq);

// Dummy configurations
envelope_config_t m_env_configs[VOICE_OPERATORS_PER_VOICE];
oscillator_config_t m_osc_configs[VOICE_OPERATORS_PER_VOICE];

voice_t m_voice;

int16_t m_mod_matrix[][VOICE_OPERATORS_PER_VOICE] = {
    {0x0000, 0x7FFF, 0x0000},
    {0x0000, 0x0000, 0x7FFF},
    {0x0000, 0x0000, 0x0000},
    {0x7FFF, 0x7FFF, 0x7FFF},
};

void setUp(void)
{
    envelope_init_Ignore();
    oscillator_init_Ignore();
    envelope_open_gate_Ignore();
    oscillator_set_frequency_Ignore();
    oscillator_reset_phase_Ignore();

    voice_init(&m_voice, m_env_configs, m_osc_configs, m_mod_matrix);
}

void tearDown(void)
{
}

/******************************************************************************
 * Initialization
 ******************************************************************************/

void test_init_initializes_components(void)
{
    for (int i = 0; i < VOICE_OPERATORS_PER_VOICE; i ++)
    {
        envelope_init_Expect(NULL, &m_env_configs[i]);
        envelope_init_IgnoreArg_env();

        oscillator_init_Expect(NULL, &m_osc_configs[i]);
        oscillator_init_IgnoreArg_osc();
    }

    voice_init(&m_voice, m_env_configs, m_osc_configs, m_mod_matrix);
}

void test_start_from_idle(void)
{
    expect_new_note_started(500);

    voice_start(&m_voice, 500);
}

/******************************************************************************
 * Update
 ******************************************************************************/

void test_update_in_idle_returns_zero(void)
{
    voice_output_t actual;

    envelope_update_IgnoreAndReturn(0);
    oscillator_update_IgnoreAndReturn(0);

    actual = voice_update(&m_voice);

    TEST_ASSERT_EQUAL_INT(0, actual);
}

void test_update_after_start_updates_components(void)
{
    voice_start(&m_voice, 500);

    for (int i = 0; i < VOICE_OPERATORS_PER_VOICE; i ++)
    {
        envelope_update_ExpectAnyArgsAndReturn(0);
    }

    for (int i = 0; i < VOICE_OPERATORS_PER_VOICE; i ++)
    {
        oscillator_update_ExpectAndReturn(NULL, 0, 0);
        oscillator_update_IgnoreArg_osc();
    }

    voice_update(&m_voice);
}

void test_update_after_start_returns_correct_value(void)
{
    voice_output_t actual;
    voice_start(&m_voice, 500);

    for (int i = 0; i < VOICE_OPERATORS_PER_VOICE; i ++)
    {
        envelope_update_ExpectAnyArgsAndReturn(0x0200); // Half level
    }

    for (int i = 0; i < VOICE_OPERATORS_PER_VOICE; i ++)
    {
        oscillator_update_ExpectAndReturn(NULL, 0, 0x4000);
        oscillator_update_IgnoreArg_osc();
    }

    actual = voice_update(&m_voice);

    TEST_ASSERT_EQUAL_HEX(0x5FFD, actual);
}

void test_update_applies_correct_phase_offset(void)
{
    voice_output_t actual;
    voice_start(&m_voice, 500);

    // First sample
    for (int i = 0; i < VOICE_OPERATORS_PER_VOICE; i ++)
    {
        envelope_update_ExpectAnyArgsAndReturn(0x0200); // Half level
    }

    for (int i = 0; i < VOICE_OPERATORS_PER_VOICE; i ++)
    {
        // All phase offsets are zero at start
        oscillator_update_ExpectAndReturn(NULL, 0, 0x4000);
        oscillator_update_IgnoreArg_osc();
    }
    voice_update(&m_voice);

    // Second sample (osc 0)
    envelope_update_ExpectAnyArgsAndReturn(0x0200); // Half level

    // Second sample (osc 1)
    envelope_update_ExpectAnyArgsAndReturn(0x0200); // Half level

    // Second sample (osc 2)
    envelope_update_ExpectAnyArgsAndReturn(0x0200); // Half level

    // Osc 0 is modulated by osc 1
    oscillator_update_ExpectAndReturn(NULL, 0x1FFF, 0x4000);
    oscillator_update_IgnoreArg_osc();

    // Osc 1 is modulated by osc 2
    oscillator_update_ExpectAndReturn(NULL, 0x1FFF, 0x4000);
    oscillator_update_IgnoreArg_osc();

    // Osc 2 is not modulated
    oscillator_update_ExpectAndReturn(NULL, 0x0000, 0x4000);
    oscillator_update_IgnoreArg_osc();

    voice_update(&m_voice);
}

/******************************************************************************
 * Note Transition
 ******************************************************************************/

void test_start_after_start_closes_env_gates(void)
{
    voice_start(&m_voice, 500);

    // Return non-zero to signal envelopes are still active.
    envelope_update_IgnoreAndReturn(0x0100);
    oscillator_update_IgnoreAndReturn(0x0000);
    voice_update(&m_voice);

    for (int i = 0; i < VOICE_OPERATORS_PER_VOICE; i ++)
    {
        envelope_close_gate_fast_ExpectAnyArgs();
    }

    voice_start(&m_voice, 600);
}

void test_start_after_start_new_note_starts_after_all_envelopes_zero(void)
{
    voice_start(&m_voice, 500);

    // Return non-zero to signal envelopes are still active.
    envelope_update_IgnoreAndReturn(0x0100);
    oscillator_update_IgnoreAndReturn(0x0000);
    voice_update(&m_voice);

    // Second start; should move to transition state.
    envelope_close_gate_fast_Ignore();
    voice_start(&m_voice, 600);

    // Return non-zero to signal envelopes are still active.
    envelope_update_IgnoreAndReturn(0x0100);
    oscillator_update_IgnoreAndReturn(0x0000);
    voice_update(&m_voice);

    // Should start new note automatically, since all envelope outputs are zero.
    expect_new_note_started(600);
    envelope_update_IgnoreAndReturn(0x0000);
    voice_update(&m_voice);
}

/******************************************************************************
 * Release
 ******************************************************************************/

void test_release_closes_envelope_gates(void)
{
    voice_start(&m_voice, 500);

    for (int i = 0; i < VOICE_OPERATORS_PER_VOICE;  i ++)
    {
        envelope_close_gate_ExpectAnyArgs();
    }
    voice_release(&m_voice);
}

void test_start_after_release_complete(void)
{
    voice_start(&m_voice, 500);

    envelope_close_gate_Ignore();
    voice_release(&m_voice);

    // All envelopes return zero, signalling end of release.
    run_updates(1);

    // New note should start immediately.
    expect_new_note_started(700);
    voice_start(&m_voice, 700);
}

/******************************************************************************
 * Stop
 ******************************************************************************/

void test_stop_calls_close_gate_fast(void)
{
    voice_start(&m_voice, 500);

    for (int i = 0; i < VOICE_OPERATORS_PER_VOICE; i ++)
    {
        envelope_close_gate_fast_ExpectAnyArgs();
    }
    voice_stop(&m_voice);
}

/******************************************************************************
 * Static Helper Functions
 ******************************************************************************/

static void run_updates(int updates)
{
    envelope_update_IgnoreAndReturn(0x0000);
    oscillator_update_IgnoreAndReturn(0x0000);
    for (int i = 0; i < updates; i ++)
    {
        voice_update(&m_voice);
    }
}

static void expect_new_note_started(uint32_t freq)
{
    for (int i = 0; i < VOICE_OPERATORS_PER_VOICE; i ++)
    {
        envelope_open_gate_ExpectAnyArgs();
        oscillator_set_frequency_Expect(NULL, freq);
        oscillator_set_frequency_IgnoreArg_osc();
        oscillator_reset_phase_ExpectAnyArgs();
    }
}
