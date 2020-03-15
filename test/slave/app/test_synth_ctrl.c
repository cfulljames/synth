#include "unity.h"
#include "synth_ctrl.h"
#include "mock_audio.h"
#include "mock_voice.h"
#include "mock_envelope.h"
#include "mock_oscillator.h"
#include "mock_cmd_parser.h"
#include "common_cmd.h"

#include <string.h>
#include <stdio.h>

/******************************************************************************
 * Macros
 ******************************************************************************/

#define DEFAULT_CHANNEL (0)
#define DEFAULT_DATA (0)
#define FIRST_VOICE (0)
#define LAST_VOICE (SYNTH_CTRL_NUM_VOICES - 1)

#define FIRST_OPERATOR (0)
#define LAST_OPERATOR (VOICE_OPERATORS_PER_VOICE - 1)

#define DEFAULT_MOD_CHANNEL (0)
#define MID_MOD_CHANNEL (12)
#define LAST_MOD_CHANNEL (15)
#define OUTPUT_MOD_ROW (VOICE_OPERATORS_PER_VOICE)

/******************************************************************************
 * Static Function Declarations
 ******************************************************************************/

static void cmd_parser_set_callback_stub(cmd_parser_cb_t cb, int num_calls);
static void voice_init_stub(
        voice_t *voice,
        envelope_config_t *env_configs,
        oscillator_config_t *osc_configs,
        void *mod_matrix,
        int num_calls);
static void envelope_config_init_stub(envelope_config_t *cfg, int num_calls);
static void oscillator_config_init_stub(oscillator_config_t *cfg, int num_calls);
static void audio_init_stub(voice_t *voices, uint16_t num_voices, int num_calls);
static void send_complete_command(void);

/******************************************************************************
 * Module-Scope Data
 ******************************************************************************/

// Function to call when a new command is received.
cmd_parser_cb_t m_cmd_callback;

// Envelope configuration instances created by the synth controller.
envelope_config_t *m_env_cfgs[VOICE_OPERATORS_PER_VOICE];

// Oscillator configuration instances created by the synth controller.
oscillator_config_t *m_osc_cfgs[VOICE_OPERATORS_PER_VOICE];

// Voice instances created by the synth controller.
voice_t *m_voices[SYNTH_CTRL_NUM_VOICES];

// Modulation matrix created by the synth controller.
int16_t (*m_mod_matrix)[VOICE_OPERATORS_PER_VOICE];

/******************************************************************************
 * Setup/Teardown
 ******************************************************************************/

void setUp(void)
{
    m_cmd_callback = NULL;
    memset(m_voices, 0, sizeof(m_voices));

    cmd_parser_set_callback_AddCallback(cmd_parser_set_callback_stub);
    voice_init_AddCallback(voice_init_stub);
    envelope_config_init_AddCallback(envelope_config_init_stub);
    oscillator_config_init_AddCallback(oscillator_config_init_stub);
    audio_init_AddCallback(audio_init_stub);

    cmd_parser_init_Expect();

    // Expect initialization of envelopes and oscillators.
    for (int i = 0; i < VOICE_OPERATORS_PER_VOICE; i ++)
    {
        envelope_config_init_ExpectAnyArgs();
        oscillator_config_init_ExpectAnyArgs();
    }

    /// Expect initialization of voices.
    for (int i = 0; i < SYNTH_CTRL_NUM_VOICES; i ++)
    {
        // Arguments are checked in stub.
        voice_init_ExpectAnyArgs();
    }

    audio_init_ExpectAnyArgs();
    cmd_parser_set_callback_ExpectAnyArgs();

    synth_ctrl_init();
}

void tearDown(void)
{
}

/******************************************************************************
 * Initialization
 ******************************************************************************/

void test_init_initializes_components(void)
{
    // Initialization happens in setUp

    // Voice pointers should have been set by init.
    for (int i = 0; i < SYNTH_CTRL_NUM_VOICES; i ++)
    {
        TEST_ASSERT_NOT_NULL(m_voices[i]);
    }

    // Callback should have been set by init.
    TEST_ASSERT_NOT_NULL(m_cmd_callback);
}

/******************************************************************************
 * Update
 ******************************************************************************/

void test_update_updates_components(void)
{
    cmd_parser_update_Expect();
    audio_update_Expect();

    synth_ctrl_update();
}

/******************************************************************************
 * Command Buffer Tests
 ******************************************************************************/

void test_cmd_not_processed_without_complete(void)
{
    // Don't expect any function calls without complete command.
    m_cmd_callback(
            COMMON_CMD_TYPE_GLOBAL,
            DEFAULT_CHANNEL,
            COMMON_CMD_GLOBAL_STOP_ALL,
            DEFAULT_DATA);
}

void test_complete_cmd_ignored_with_empty_buffer(void)
{
    // Don't expect any function calls with no commands in buffer.
    send_complete_command();
}

void test_cmd_processed_after_complete(void)
{
    // No function calls yet, just place this in buffer.
    m_cmd_callback(
            COMMON_CMD_TYPE_GLOBAL,
            DEFAULT_CHANNEL,
            COMMON_CMD_GLOBAL_STOP_ALL,
            DEFAULT_DATA);

    // Previous command processed when complete received.
    for (int i = 0; i < SYNTH_CTRL_NUM_VOICES; i ++)
    {
        voice_stop_Expect(m_voices[i]);
    }

    send_complete_command();

}

void test_multiple_commands_processed_after_complete(void)
{
    // No function calls yet, just place these in buffer.
    m_cmd_callback(
            COMMON_CMD_TYPE_VOICE,
            DEFAULT_CHANNEL,
            COMMON_CMD_VOICE_START_NOTE,
            DEFAULT_DATA);

    m_cmd_callback(
            COMMON_CMD_TYPE_OPERATOR,
            DEFAULT_CHANNEL,
            COMMON_CMD_OPERATOR_SET_BITMASK,
            DEFAULT_DATA);

    // Previous commands processed when complete received.
    voice_start_Expect(m_voices[DEFAULT_CHANNEL], DEFAULT_DATA);
    oscillator_config_set_bitmask_Expect(
            m_osc_cfgs[DEFAULT_CHANNEL], DEFAULT_DATA);

    send_complete_command();
}

void test_multiple_command_complete_cycles(void)
{
    for (int i = 0; i < SYNTH_CTRL_BUFFER_SIZE * 2; i ++)
    {
        // No function calls yet, just place these in buffer.
        m_cmd_callback(
                COMMON_CMD_TYPE_VOICE,
                DEFAULT_CHANNEL,
                COMMON_CMD_VOICE_START_NOTE,
                DEFAULT_DATA);

        m_cmd_callback(
                COMMON_CMD_TYPE_OPERATOR,
                DEFAULT_CHANNEL,
                COMMON_CMD_OPERATOR_SET_BITMASK,
                DEFAULT_DATA);

        // Previous commands processed when complete received.
        voice_start_Expect(m_voices[DEFAULT_CHANNEL], DEFAULT_DATA);
        oscillator_config_set_bitmask_Expect(
                m_osc_cfgs[DEFAULT_CHANNEL], DEFAULT_DATA);

        send_complete_command();
    }
}

void test_oldest_commands_dropped_on_buffer_overflow(void)
{
    // Overflow buffer with 5 extra commands.
    for (int i = 0; i < (SYNTH_CTRL_BUFFER_SIZE + 5); i ++)
    {
        // No function calls yet, just place these in buffer.
        m_cmd_callback(
                COMMON_CMD_TYPE_VOICE,
                DEFAULT_CHANNEL,
                COMMON_CMD_VOICE_START_NOTE,
                DEFAULT_DATA);
    }

    // Buffer is already full; this will overwrite next command.
    m_cmd_callback(
            COMMON_CMD_TYPE_OPERATOR,
            DEFAULT_CHANNEL,
            COMMON_CMD_OPERATOR_SET_BITMASK,
            DEFAULT_DATA);

    // Six voice commands got dropped, so expect six less function calls.
    for (int i = 0; i < (SYNTH_CTRL_BUFFER_SIZE - 1); i ++)
    {
        voice_start_Expect(m_voices[DEFAULT_CHANNEL], DEFAULT_DATA);
    }

    // Operator command called at end.
    oscillator_config_set_bitmask_Expect(
            m_osc_cfgs[DEFAULT_CHANNEL], DEFAULT_DATA);

    send_complete_command();
}

/******************************************************************************
 * Voice Commands
 ******************************************************************************/

void test_command_voice_start(void)
{
    m_cmd_callback(
            COMMON_CMD_TYPE_VOICE,
            DEFAULT_CHANNEL,
            COMMON_CMD_VOICE_START_NOTE,
            DEFAULT_DATA);

    m_cmd_callback(
            COMMON_CMD_TYPE_VOICE,
            LAST_VOICE,
            COMMON_CMD_VOICE_START_NOTE,
            1500);

    voice_start_Expect(m_voices[DEFAULT_CHANNEL], DEFAULT_DATA);
    voice_start_Expect(m_voices[LAST_VOICE], 1500);

    send_complete_command();
}

void test_command_voice_release(void)
{
    m_cmd_callback(
            COMMON_CMD_TYPE_VOICE,
            DEFAULT_CHANNEL,
            COMMON_CMD_VOICE_RELEASE,
            DEFAULT_DATA);

    m_cmd_callback(
            COMMON_CMD_TYPE_VOICE,
            LAST_VOICE,
            COMMON_CMD_VOICE_RELEASE,
            DEFAULT_DATA);

    voice_release_Expect(m_voices[DEFAULT_CHANNEL]);
    voice_release_Expect(m_voices[LAST_VOICE]);

    send_complete_command();
}

void test_command_voice_stop(void)
{
    m_cmd_callback(
            COMMON_CMD_TYPE_VOICE,
            DEFAULT_CHANNEL,
            COMMON_CMD_VOICE_STOP,
            DEFAULT_DATA);

    m_cmd_callback(
            COMMON_CMD_TYPE_VOICE,
            LAST_VOICE,
            COMMON_CMD_VOICE_STOP,
            DEFAULT_DATA);

    voice_stop_Expect(m_voices[DEFAULT_CHANNEL]);
    voice_stop_Expect(m_voices[LAST_VOICE]);

    send_complete_command();
}

/******************************************************************************
 * Modulation Commands
 ******************************************************************************/

void test_command_modulation_set(void)
{
    m_cmd_callback(
            COMMON_CMD_TYPE_MODULATION,
            DEFAULT_MOD_CHANNEL,
            COMMON_CMD_MODULATION_SET,
            0x0100);

    m_cmd_callback(
            COMMON_CMD_TYPE_MODULATION,
            MID_MOD_CHANNEL,
            COMMON_CMD_MODULATION_SET,
            0xCAFE);

    m_cmd_callback(
            COMMON_CMD_TYPE_MODULATION,
            LAST_MOD_CHANNEL,
            COMMON_CMD_MODULATION_SET,
            0x7FFF);

    send_complete_command();

    TEST_ASSERT_EQUAL_HEX16(
            0x0100, m_mod_matrix[FIRST_OPERATOR][FIRST_OPERATOR]);
    TEST_ASSERT_EQUAL_HEX16(
            0xCAFE, m_mod_matrix[LAST_OPERATOR][FIRST_OPERATOR]);
    TEST_ASSERT_EQUAL_HEX16(
            0x7FFF, m_mod_matrix[LAST_OPERATOR][LAST_OPERATOR]);
}

/******************************************************************************
 * Operator Commands
 ******************************************************************************/

void test_command_operator_set_harmonic(void)
{
    m_cmd_callback(
            COMMON_CMD_TYPE_OPERATOR,
            FIRST_OPERATOR,
            COMMON_CMD_OPERATOR_SET_HARMONIC,
            DEFAULT_DATA);

    m_cmd_callback(
            COMMON_CMD_TYPE_OPERATOR,
            LAST_OPERATOR,
            COMMON_CMD_OPERATOR_SET_HARMONIC,
            8);

    oscillator_config_set_harmonic_Expect(
            m_osc_cfgs[FIRST_OPERATOR], DEFAULT_DATA);
    oscillator_config_set_harmonic_Expect(
            m_osc_cfgs[LAST_OPERATOR], 8);

    send_complete_command();
}

void test_command_operator_set_bitmask(void)
{
    m_cmd_callback(
            COMMON_CMD_TYPE_OPERATOR,
            FIRST_OPERATOR,
            COMMON_CMD_OPERATOR_SET_BITMASK,
            DEFAULT_DATA);

    m_cmd_callback(
            COMMON_CMD_TYPE_OPERATOR,
            LAST_OPERATOR,
            COMMON_CMD_OPERATOR_SET_BITMASK,
            0xFFFF);

    oscillator_config_set_bitmask_Expect(
            m_osc_cfgs[FIRST_OPERATOR], DEFAULT_DATA);
    oscillator_config_set_bitmask_Expect(
            m_osc_cfgs[LAST_OPERATOR], 0xFFFF);

    send_complete_command();
}

void test_command_operator_set_output(void)
{
    m_cmd_callback(
            COMMON_CMD_TYPE_OPERATOR,
            FIRST_OPERATOR,
            COMMON_CMD_OPERATOR_SET_OUTPUT,
            0x0500);

    m_cmd_callback(
            COMMON_CMD_TYPE_OPERATOR,
            LAST_OPERATOR,
            COMMON_CMD_OPERATOR_SET_OUTPUT,
            0xCAFE);

    send_complete_command();

    TEST_ASSERT_EQUAL_HEX16(
            0x0500, m_mod_matrix[OUTPUT_MOD_ROW][FIRST_OPERATOR]);
    TEST_ASSERT_EQUAL_HEX16(
            0xCAFE, m_mod_matrix[OUTPUT_MOD_ROW][LAST_OPERATOR]);
}

void test_command_operator_set_attack(void)
{
    m_cmd_callback(
            COMMON_CMD_TYPE_OPERATOR,
            FIRST_OPERATOR,
            COMMON_CMD_OPERATOR_SET_ATTACK,
            DEFAULT_DATA);

    m_cmd_callback(
            COMMON_CMD_TYPE_OPERATOR,
            LAST_OPERATOR,
            COMMON_CMD_OPERATOR_SET_ATTACK,
            0xABABABAB);

    envelope_set_attack_Expect(
            m_env_cfgs[FIRST_OPERATOR], DEFAULT_DATA);
    envelope_set_attack_Expect(
            m_env_cfgs[LAST_OPERATOR], 0xABABABAB);

    send_complete_command();
}

void test_command_operator_set_decay(void)
{
    m_cmd_callback(
            COMMON_CMD_TYPE_OPERATOR,
            FIRST_OPERATOR,
            COMMON_CMD_OPERATOR_SET_DECAY,
            DEFAULT_DATA);

    m_cmd_callback(
            COMMON_CMD_TYPE_OPERATOR,
            LAST_OPERATOR,
            COMMON_CMD_OPERATOR_SET_DECAY,
            0xDDDDCCCC);

    envelope_set_decay_Expect(
            m_env_cfgs[FIRST_OPERATOR], DEFAULT_DATA);
    envelope_set_decay_Expect(
            m_env_cfgs[LAST_OPERATOR], 0xDDDDCCCC);

    send_complete_command();
}

void test_command_operator_set_sustain(void)
{
    m_cmd_callback(
            COMMON_CMD_TYPE_OPERATOR,
            FIRST_OPERATOR,
            COMMON_CMD_OPERATOR_SET_SUSTAIN,
            DEFAULT_DATA);

    m_cmd_callback(
            COMMON_CMD_TYPE_OPERATOR,
            LAST_OPERATOR,
            COMMON_CMD_OPERATOR_SET_SUSTAIN,
            0x0100);

    envelope_set_sustain_Expect(
            m_env_cfgs[FIRST_OPERATOR], DEFAULT_DATA);
    envelope_set_sustain_Expect(
            m_env_cfgs[LAST_OPERATOR], 0x0100);

    send_complete_command();
}

void test_command_operator_set_release(void)
{
    m_cmd_callback(
            COMMON_CMD_TYPE_OPERATOR,
            FIRST_OPERATOR,
            COMMON_CMD_OPERATOR_SET_RELEASE,
            DEFAULT_DATA);

    m_cmd_callback(
            COMMON_CMD_TYPE_OPERATOR,
            LAST_OPERATOR,
            COMMON_CMD_OPERATOR_SET_RELEASE,
            0xAAAAAAAA);

    envelope_set_release_Expect(
            m_env_cfgs[FIRST_OPERATOR], DEFAULT_DATA);
    envelope_set_release_Expect(
            m_env_cfgs[LAST_OPERATOR], 0xAAAAAAAA);

    send_complete_command();
}

/******************************************************************************
 * Invalid Commands
 ******************************************************************************/

void test_invalid_operator(void)
{
    m_cmd_callback(
            COMMON_CMD_TYPE_OPERATOR,
            LAST_OPERATOR + 1,
            COMMON_CMD_OPERATOR_SET_RELEASE,
            0xAAAAAAAA);

    // Expect no function calls.

    send_complete_command();
}

void test_invalid_voice(void)
{
    m_cmd_callback(
            COMMON_CMD_TYPE_VOICE,
            LAST_VOICE + 1,
            COMMON_CMD_VOICE_STOP,
            DEFAULT_DATA);

    // Expect no function calls.

    send_complete_command();
}

void test_invalid_modulation(void)
{
    m_cmd_callback(
            COMMON_CMD_TYPE_MODULATION,
            LAST_MOD_CHANNEL + 1,
            COMMON_CMD_MODULATION_SET,
            0xABCD);

    // Expect no function calls.

    send_complete_command();

    // The next space in the array after the mod matrix is the output row.
    TEST_ASSERT_EQUAL_INT(0, m_mod_matrix[OUTPUT_MOD_ROW][FIRST_OPERATOR]);
}

/******************************************************************************
 * Helper Functions
 ******************************************************************************/

static void cmd_parser_set_callback_stub(cmd_parser_cb_t cb, int num_calls)
{
    m_cmd_callback = cb;
}

static void voice_init_stub(
        voice_t *voice,
        envelope_config_t *env_configs,
        oscillator_config_t *osc_configs,
        void *mod_matrix,
        int num_calls)
{
    m_voices[num_calls] = voice;
    m_mod_matrix = mod_matrix;

    TEST_ASSERT_EQUAL_PTR(m_env_cfgs[0], env_configs);
    TEST_ASSERT_EQUAL_PTR(m_osc_cfgs[0], osc_configs);
}

static void envelope_config_init_stub(envelope_config_t *cfg, int num_calls)
{
    m_env_cfgs[num_calls] = cfg;
}

static void oscillator_config_init_stub(oscillator_config_t *cfg, int num_calls)
{
    m_osc_cfgs[num_calls] = cfg;
}

static void audio_init_stub(voice_t *voices, uint16_t num_voices, int num_calls)
{
    TEST_ASSERT_EQUAL_PTR(m_voices[0], voices);
    TEST_ASSERT_EQUAL_UINT16(SYNTH_CTRL_NUM_VOICES, num_voices);
}

static void send_complete_command(void)
{
    m_cmd_callback(
            COMMON_CMD_TYPE_GLOBAL,
            DEFAULT_CHANNEL,
            COMMON_CMD_GLOBAL_COMPLETE,
            DEFAULT_DATA);
}
