#include "unity.h"
#include "oscillator.h"
#include "sine.h"

oscillator_t m_osc;
oscillator_config_t m_cfg;

#define DEFAULT_PHASE_OFFSET (0)
#define DEFAULT_FREQUENCY (1U << 16)
#define SINE_TABLE_LENGTH (2048)

void setUp(void)
{
    oscillator_config_init(&m_cfg);
    oscillator_init(&m_osc, &m_cfg);
    oscillator_set_frequency(&m_osc, DEFAULT_FREQUENCY);
}

void tearDown(void)
{
}

/******************************************************************************
 * Initialization
 ******************************************************************************/

void test_first_ouput_zero_after_init(void)
{
    TEST_ASSERT_EQUAL(0, oscillator_update(&m_osc, DEFAULT_PHASE_OFFSET));
}

/******************************************************************************
 * Frequency
 ******************************************************************************/

void test_second_output_zero_with_zero_freq(void)
{
    oscillator_set_frequency(&m_osc, 0);

    // First update
    oscillator_update(&m_osc, DEFAULT_PHASE_OFFSET);

    // Second update
    TEST_ASSERT_EQUAL(0, oscillator_update(&m_osc, DEFAULT_PHASE_OFFSET));
}

void test_second_sample_with_frequency_set(void)
{
    oscillator_output_t actual;

    oscillator_update(&m_osc, DEFAULT_PHASE_OFFSET);
    actual = oscillator_update(&m_osc, DEFAULT_PHASE_OFFSET);

    TEST_ASSERT_EQUAL(sine_table[1], actual);
}

/******************************************************************************
 * Update
 ******************************************************************************/

void test_wraps_around_to_beginning_of_table(void)
{
    oscillator_output_t actual;

    for (int i = 0; i <= SINE_TABLE_LENGTH; i ++)
    {
        oscillator_update(&m_osc, DEFAULT_PHASE_OFFSET);
    }
    actual = oscillator_update(&m_osc, DEFAULT_PHASE_OFFSET);

    TEST_ASSERT_EQUAL(sine_table[1], actual);
}

void test_phase_offset_is_applied(void)
{
    oscillator_output_t actual;

    actual = oscillator_update(&m_osc, 1);

    // Should be on the second table entry
    TEST_ASSERT_EQUAL(sine_table[1], actual);
}

void test_phase_offset_wraparound(void)
{
    oscillator_output_t actual;

    actual = oscillator_update(&m_osc, SINE_TABLE_LENGTH + 1);

    TEST_ASSERT_EQUAL(sine_table[1], actual);
}

void test_phase_offset_wraparound_negative(void)
{
    oscillator_output_t actual;

    actual = oscillator_update(&m_osc, -(SINE_TABLE_LENGTH - 2));

    TEST_ASSERT_EQUAL(sine_table[2], actual);
}

/******************************************************************************
 * Phase Reset
 ******************************************************************************/

void test_output_is_zero_after_reset_phase(void)
{
    oscillator_output_t actual;

    oscillator_update(&m_osc, DEFAULT_PHASE_OFFSET);
    oscillator_update(&m_osc, DEFAULT_PHASE_OFFSET);

    oscillator_reset_phase(&m_osc);
    actual = oscillator_update(&m_osc, DEFAULT_PHASE_OFFSET);

    TEST_ASSERT_EQUAL(0, actual);
}

/******************************************************************************
 * Harmonic
 ******************************************************************************/

void test_harmonic_multiplies_frequency(void)
{
    oscillator_output_t actual;

    oscillator_config_set_harmonic(&m_cfg, 4);
    oscillator_update(&m_osc, DEFAULT_PHASE_OFFSET);
    actual = oscillator_update(&m_osc, DEFAULT_PHASE_OFFSET);
    TEST_ASSERT_EQUAL(sine_table[4], actual);
}

/******************************************************************************
 * Bitmask
 ******************************************************************************/

void test_bitmask_applied_to_output(void)
{
    oscillator_output_t actual;

    oscillator_config_set_bitmask(&m_cfg, 0xF000);

    // Table entry 41 is the first one to have a bit set in the most significant
    // nibble.
    for (int i = 0; i < 41; i ++)
    {
        actual = oscillator_update(&m_osc, DEFAULT_PHASE_OFFSET);
    }
    TEST_ASSERT_EQUAL_HEX(0, actual);

    actual = oscillator_update(&m_osc, DEFAULT_PHASE_OFFSET);
    TEST_ASSERT_EQUAL_HEX(0x1000, actual);
}
