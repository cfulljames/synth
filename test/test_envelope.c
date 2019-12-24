#include "unity.h"
#include "envelope.h"

static void SkipTicks(uint16_t ticks);

struct Envelope_s m_env;

void setUp(void)
{
    Envelope_Init(&m_env);
}

void tearDown(void)
{
}

void test_OutputIsZeroAfterInit(void)
{
    uint8_t output = Envelope_GetNextOutput(&m_env);
    TEST_ASSERT_EQUAL_UINT(0, output);
}

void test_OutputIsMaxAfterStartWithDefaults(void)
{
    Envelope_Start(&m_env);
    uint8_t output = Envelope_GetNextOutput(&m_env);
    TEST_ASSERT_EQUAL_UINT(255, output);
}

void test_OutputStaysAtMaxAfterStartWithDefaults(void)
{
    Envelope_Start(&m_env);

    // Skip a sample
    Envelope_GetNextOutput(&m_env);

    uint8_t output = Envelope_GetNextOutput(&m_env);
    TEST_ASSERT_EQUAL_UINT(255, output);
}

void test_OutputReturnsToZeroAfterStopWithDefaults(void)
{
    Envelope_Start(&m_env);
    Envelope_Stop(&m_env);

    uint8_t output = Envelope_GetNextOutput(&m_env);
    TEST_ASSERT_EQUAL_UINT(0, output);
}

void test_OutputAttackAfterStart(void)
{
    Envelope_SetAttack(&m_env, 10);
    Envelope_Start(&m_env);

    uint8_t output = Envelope_GetNextOutput(&m_env);
    TEST_ASSERT_EQUAL_UINT(10, output);
}

void test_OutputIncreasesDuringAttack(void)
{
    Envelope_SetAttack(&m_env, 10);
    Envelope_Start(&m_env);

    // Skip tick
    Envelope_GetNextOutput(&m_env);

    uint8_t output = Envelope_GetNextOutput(&m_env);
    TEST_ASSERT_EQUAL_UINT(20, output);
}

/*
 * Skip over the specified number of ticks, discarding envelope output.
 */
static void SkipTicks(uint16_t ticks)
{
    for (uint16_t i = 0; i < ticks; i ++)
    {
        Envelope_GetNextOutput(&m_env);
    }
}

/*
 * TESTS
 *
 * Output is zero when idle
 * After start, output starts at attack
 * After max/attack ticks, output is max
 * Following tick is max - decay
 * Reaches sustain after (max-sustain)/decay ticks
 * Outputs sustain until stop
 * After stop, following tick is sustain - release
 * reaches 0 after sustain/release ticks
 * stays at zero until next start
 * returns to 0 immediately after reset
 *
 *
 * Defaults after init:
 *      env is idle (output zero)
 *      attack is 0
 *      decay is 0
 *      sustain is max
 *      release is 0
 */
