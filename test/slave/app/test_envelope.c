#include "unity.h"
#include "envelope.h"

envelope_config_t m_cfg;
envelope_t m_env;

#define DEFAULT_ATTACK 13
#define DEFAULT_DECAY 17
#define DEFAULT_SUSTAIN 512
#define DEFAULT_RELEASE 12

#define FAST_RELEASE_RATE 10

#define ENVELOPE_RATE_FROM_FLOAT(f) \
    ((envelope_rate_t)((float)(f) * (1 << (ENVELOPE_RATE_SHIFT))))

#define FLOAT_FROM_ENVELOPE_RATE(r) \
    ((float)(envelope_rate_t)(r) / (1 << (ENVELOPE_RATE_SHIFT)))

void setUp(void)
{
    envelope_config_init(&m_cfg);
    envelope_init(&m_env, &m_cfg);
    envelope_set_attack(&m_cfg, ENVELOPE_RATE_FROM_FLOAT(DEFAULT_ATTACK));
    envelope_set_decay(&m_cfg, ENVELOPE_RATE_FROM_FLOAT(DEFAULT_DECAY));
    envelope_set_sustain(&m_cfg, DEFAULT_SUSTAIN);
    envelope_set_release(&m_cfg, ENVELOPE_RATE_FROM_FLOAT(DEFAULT_RELEASE));
}

void tearDown(void)
{
}

/******************************************************************************
 * Initialization
 ******************************************************************************/

void test_level_zero_after_init(void)
{
    envelope_level_t level = envelope_update(&m_env);
    TEST_ASSERT_EQUAL_INT(0, level);
}

/******************************************************************************
 * Attack
 ******************************************************************************/

void test_level_increase_during_attack(void)
{
    envelope_open_gate(&m_env);
    envelope_level_t level = envelope_update(&m_env);
    TEST_ASSERT_EQUAL_INT(DEFAULT_ATTACK, level);
}

void test_level_almost_max_at_end_of_attack(void)
{
    envelope_level_t level;
    envelope_open_gate(&m_env);
    int num_steps = (ENVELOPE_LEVEL_MAX / DEFAULT_ATTACK);
    for (int i = 0; i < num_steps; i ++)
    {
        level = envelope_update(&m_env);
    }
    uint16_t expected = (ENVELOPE_LEVEL_MAX / DEFAULT_ATTACK) * DEFAULT_ATTACK;
    TEST_ASSERT_EQUAL_INT(expected, level);
}

void test_start_new_attack_after_previous_release(void)
{
    envelope_level_t level;
    envelope_open_gate(&m_env);

    envelope_update(&m_env);
    envelope_update(&m_env);

    // Switch to release by closing gate
    envelope_close_gate(&m_env);
    envelope_update(&m_env);

    // Stop at zero
    envelope_update(&m_env);
    envelope_update(&m_env);

    // Start new attack
    envelope_open_gate(&m_env);
    level = envelope_update(&m_env);

    uint16_t expected = DEFAULT_ATTACK;
    TEST_ASSERT_EQUAL_INT(expected, level);
}

/******************************************************************************
 * Decay
 ******************************************************************************/

void test_level_max_at_start_of_decay(void)
{
    envelope_level_t level;
    envelope_open_gate(&m_env);
    int num_steps = (ENVELOPE_LEVEL_MAX / DEFAULT_ATTACK) + 1;
    for (int i = 0; i < num_steps; i ++)
    {
        level = envelope_update(&m_env);
    }
    TEST_ASSERT_EQUAL_INT(ENVELOPE_LEVEL_MAX, level);
}

void test_level_decrease_in_decay(void)
{
    envelope_level_t level;
    envelope_open_gate(&m_env);
    int num_steps = (ENVELOPE_LEVEL_MAX / DEFAULT_ATTACK) + 2;
    for (int i = 0; i < num_steps; i ++)
    {
        level = envelope_update(&m_env);
    }
    uint16_t expected = ENVELOPE_LEVEL_MAX - DEFAULT_DECAY;
    TEST_ASSERT_EQUAL_INT(expected, level);
}

/******************************************************************************
 * Sustain
 ******************************************************************************/

void test_level_stops_decreasing_in_sustain(void)
{
    envelope_level_t level;
    envelope_open_gate(&m_env);
    int num_attack_steps = ENVELOPE_LEVEL_MAX / DEFAULT_ATTACK;
    int num_decay_steps =
        (ENVELOPE_LEVEL_MAX - DEFAULT_SUSTAIN) / DEFAULT_DECAY + 1;
    int total_steps = num_attack_steps + num_decay_steps + 1;

    for (int i = 0; i < total_steps; i ++)
    {
        level = envelope_update(&m_env);
    }
    uint16_t expected = DEFAULT_SUSTAIN;
    TEST_ASSERT_EQUAL_INT(expected, level);
    level = envelope_update(&m_env);
    TEST_ASSERT_EQUAL_INT(expected, level);
}

void test_level_stops_decreasing_with_zero_sustain(void)
{
    envelope_level_t level;
    int sustain = 0;
    envelope_set_sustain(&m_cfg, sustain);
    envelope_open_gate(&m_env);
    int num_attack_steps = ENVELOPE_LEVEL_MAX / DEFAULT_ATTACK;
    int num_decay_steps = ENVELOPE_LEVEL_MAX / DEFAULT_DECAY + 1;
    int total_steps = num_attack_steps + num_decay_steps + 1;

    for (int i = 0; i < total_steps; i ++)
    {
        level = envelope_update(&m_env);
    }
    uint16_t expected = sustain;
    TEST_ASSERT_EQUAL_INT(expected, level);
    envelope_update(&m_env);
    envelope_update(&m_env);
    envelope_update(&m_env);
    level = envelope_update(&m_env);
    TEST_ASSERT_EQUAL_INT(expected, level);
}

void test_level_stops_decreasing_with_max_sustain(void)
{
    envelope_level_t level;
    int sustain = ENVELOPE_LEVEL_MAX;
    envelope_set_sustain(&m_cfg, sustain);
    envelope_open_gate(&m_env);
    int num_attack_steps = ENVELOPE_LEVEL_MAX / DEFAULT_ATTACK;

    // Should immediately transition to sustain.
    int num_decay_steps = 1;
    int total_steps = num_attack_steps + num_decay_steps + 1;

    for (int i = 0; i < total_steps; i ++)
    {
        level = envelope_update(&m_env);
    }
    uint16_t expected = sustain;
    TEST_ASSERT_EQUAL_INT(expected, level);
    envelope_update(&m_env);
    envelope_update(&m_env);
    envelope_update(&m_env);
    level = envelope_update(&m_env);
    TEST_ASSERT_EQUAL_INT(expected, level);
}

/******************************************************************************
 * Release
 ******************************************************************************/

void test_level_starts_decreasing_in_release_from_sustain(void)
{
    envelope_level_t level;
    envelope_open_gate(&m_env);
    int num_attack_steps = ENVELOPE_LEVEL_MAX / DEFAULT_ATTACK;
    int num_decay_steps =
        (ENVELOPE_LEVEL_MAX - DEFAULT_SUSTAIN) / DEFAULT_DECAY + 1;
    int total_steps = num_attack_steps + num_decay_steps + 1;

    // Get into sustain state
    for (int i = 0; i < total_steps; i ++)
    {
        envelope_update(&m_env);
    }

    // Switch to release by closing gate
    envelope_close_gate(&m_env);
    level = envelope_update(&m_env);

    uint16_t expected = DEFAULT_SUSTAIN - DEFAULT_RELEASE;
    TEST_ASSERT_EQUAL_INT(expected, level);
}

void test_level_starts_decreasing_in_release_from_attack(void)
{
    envelope_level_t level;
    envelope_open_gate(&m_env);

    envelope_update(&m_env);
    envelope_update(&m_env);

    // Switch to release by closing gate
    envelope_close_gate(&m_env);
    level = envelope_update(&m_env);

    uint16_t expected = (2 * DEFAULT_ATTACK) - DEFAULT_RELEASE;
    TEST_ASSERT_EQUAL_INT(expected, level);
}

void test_level_starts_decreasing_in_release_from_decay(void)
{
    envelope_level_t level;
    envelope_open_gate(&m_env);
    int num_attack_steps = ENVELOPE_LEVEL_MAX / DEFAULT_ATTACK;

    // Get into sustain state
    for (int i = 0; i < num_attack_steps; i ++)
    {
        level = envelope_update(&m_env);
    }

    // First step of decay should be at max
    envelope_update(&m_env);

    // Switch to release by closing gate
    envelope_close_gate(&m_env);
    level = envelope_update(&m_env);

    uint16_t expected = ENVELOPE_LEVEL_MAX - DEFAULT_RELEASE;
    TEST_ASSERT_EQUAL_INT(expected, level);
}

void test_returns_to_idle_after_release(void)
{
    envelope_level_t level;
    envelope_open_gate(&m_env);

    envelope_update(&m_env);
    envelope_update(&m_env);

    // Switch to release by closing gate
    envelope_close_gate(&m_env);
    envelope_update(&m_env);

    // Stop at zero
    envelope_update(&m_env);
    level = envelope_update(&m_env);

    uint16_t expected = 0;
    TEST_ASSERT_EQUAL_INT(expected, level);
}

/******************************************************************************
 * Fast Release
 ******************************************************************************/

void test_fast_release_from_attack(void)
{
    envelope_level_t level;
    envelope_open_gate(&m_env);

    envelope_update(&m_env);
    envelope_update(&m_env);

    // Switch to fast release
    envelope_close_gate_fast(&m_env);
    level = envelope_update(&m_env);

    uint16_t expected = 2 * DEFAULT_ATTACK - FAST_RELEASE_RATE;
    TEST_ASSERT_EQUAL_INT(expected, level);
}

void test_fast_release_ends_at_zero(void)
{
    envelope_level_t level;
    envelope_open_gate(&m_env);

    envelope_update(&m_env);
    envelope_update(&m_env);

    // Switch to fast release
    envelope_close_gate_fast(&m_env);
    envelope_update(&m_env);
    envelope_update(&m_env);

    // Should hit zero
    level = envelope_update(&m_env);

    uint16_t expected = 0;
    TEST_ASSERT_EQUAL_INT(expected, level);
}

void test_fast_release_stays_in_idle(void)
{
    envelope_level_t level;
    envelope_open_gate(&m_env);

    envelope_update(&m_env);
    envelope_update(&m_env);

    // Switch to fast release
    envelope_close_gate_fast(&m_env);
    envelope_update(&m_env);
    envelope_update(&m_env);

    // Should hit zero
    envelope_update(&m_env);

    // Start new attack
    level = envelope_update(&m_env);

    uint16_t expected = 0;
    TEST_ASSERT_EQUAL_INT(expected, level);
}

void test_fast_release_continues_after_gate_off(void)
{
    envelope_level_t level;
    envelope_open_gate(&m_env);

    envelope_update(&m_env);
    envelope_update(&m_env);

    // Switch to fast release
    envelope_close_gate_fast(&m_env);
    envelope_update(&m_env);

    // Close gate again
    envelope_close_gate(&m_env);
    level = envelope_update(&m_env);

    uint16_t expected = (2 * DEFAULT_ATTACK) - (2 * FAST_RELEASE_RATE);
    TEST_ASSERT_EQUAL_INT(expected, level);
}

void test_open_gate_causes_fast_release(void)
{
    envelope_level_t level;
    envelope_open_gate(&m_env);

    envelope_update(&m_env);
    envelope_update(&m_env);

    // Switch to fast release by opening gate again
    envelope_open_gate(&m_env);
    level = envelope_update(&m_env);

    uint16_t expected = 2 * DEFAULT_ATTACK - FAST_RELEASE_RATE;
    TEST_ASSERT_EQUAL_INT(expected, level);
}

void test_fast_release_to_attack_when_gate_open(void)
{
    envelope_level_t level;
    envelope_open_gate(&m_env);
    envelope_update(&m_env);
    envelope_update(&m_env);

    // Switch to fast release by opening gate again
    envelope_open_gate(&m_env);
    envelope_update(&m_env);
    envelope_update(&m_env);

    // Should hit zero
    envelope_update(&m_env);

    // Start new attack
    level = envelope_update(&m_env);

    uint16_t expected = DEFAULT_ATTACK;
    TEST_ASSERT_EQUAL_INT(expected, level);
}
