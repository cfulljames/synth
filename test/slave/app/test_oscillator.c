#include "unity.h"
#include "oscillator.h"
#include "sine.h"

oscillator_t m_osc;

#define DEFAULT_PHASE_OFFSET (0)
#define DEFAULT_FREQUENCY (1U << 16)
#define SINE_TABLE_LENGTH (2048)

void setUp(void)
{
    oscillator_init(&m_osc);
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
