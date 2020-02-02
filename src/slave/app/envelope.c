#include "envelope.h"

#define ACCUMULATOR_MAX (UINT32_MAX)

#define RATE_SHIFT ENVELOPE_RATE_SHIFT

#define LEVEL_MAX ENVELOPE_LEVEL_MAX

#define FAST_RELEASE_RATE ((envelope_rate_t)10 << RATE_SHIFT)

void envelope_init(envelope_t *env)
{
    env->level = 0;
    env->attack = 0;
    env->attack_threshold = 0;
    env->decay = 0;
    env->sustain = 0;
    env->release = 0;
    env->state = ENVELOPE_STATE_IDLE;
    env->gate_open = false;
    env->accumulator = 0;
}

__attribute__((always_inline))
static inline void update_attack(envelope_t *env)
{
    env->accumulator += env->attack;
    env->level = env->accumulator >> RATE_SHIFT;
    if (env->accumulator > env->attack_threshold)
    {
        // End of the attack stage; transition to decay.
        env->state = ENVELOPE_STATE_DECAY;
        env->accumulator = (envelope_rate_t)LEVEL_MAX << RATE_SHIFT;
    }
}

__attribute__((always_inline))
static inline void update_decay(envelope_t *env)
{
    if (env->accumulator <= env->decay_threshold)
    {
        // End of the decay stage; transition to sustain.
        env->state = ENVELOPE_STATE_SUSTAIN;
        env->level = env->sustain;
    }
    else
    {
        // Still decaying.
        env->level = env->accumulator >> RATE_SHIFT;
        env->accumulator -= env->decay;
    }
}

__attribute__((always_inline))
static inline void update_release(envelope_t *env)
{
    if (env->accumulator < env->release)
    {
        // End of the release stage; transition to idle.
        env->level = 0;
        env->state = ENVELOPE_STATE_IDLE;
    }
    else
    {
        // Still releasing.
        env->accumulator -= env->release;
        env->level = env->accumulator >> RATE_SHIFT;
    }
}

__attribute__((always_inline))
static inline void update_fast_release(envelope_t *env)
{
    if (env->accumulator < FAST_RELEASE_RATE)
    {
        // End of the fast release stage.
        env->level = 0;
        env->accumulator = 0;
        if (env->gate_open)
        {
            env->state = ENVELOPE_STATE_ATTACK;
        }
        else
        {
            env->state = ENVELOPE_STATE_IDLE;
        }
    }
    else
    {
        // Still releasing.
        env->accumulator -= FAST_RELEASE_RATE;
        env->level = env->accumulator >> RATE_SHIFT;
    }
}

__attribute__((always_inline))
static inline void set_decay_threshold(envelope_t *env)
{
    uint16_t decay_level = (envelope_rate_t)env->decay >> RATE_SHIFT;
    if (ENVELOPE_LEVEL_MAX - decay_level < env->sustain)
    {
        env->decay_threshold = (envelope_rate_t)ENVELOPE_LEVEL_MAX << RATE_SHIFT;
    }
    else
    {
        env->decay_threshold = env->decay
            + ((envelope_rate_t)env->sustain << RATE_SHIFT);
    }
}

void envelope_update(envelope_t *env)
{
    switch (env->state)
    {
        case ENVELOPE_STATE_IDLE:
            // Do nothing.
            break;
        case ENVELOPE_STATE_ATTACK:
            update_attack(env);
            break;
        case ENVELOPE_STATE_DECAY:
            update_decay(env);
            break;
        case ENVELOPE_STATE_SUSTAIN:
            // Do nothing.
            break;
        case ENVELOPE_STATE_RELEASE:
            update_release(env);
            break;
        case ENVELOPE_STATE_FAST_RELEASE:
            update_fast_release(env);
            break;
    }
}

void envelope_set_attack(envelope_t *env, envelope_rate_t attack)
{
    env->attack = attack;
    env->attack_threshold = ACCUMULATOR_MAX - attack;
}

void envelope_set_decay(envelope_t *env, envelope_rate_t decay)
{
    env->decay = decay;
    set_decay_threshold(env);
}

void envelope_set_sustain(envelope_t *env, envelope_level_t sustain)
{
    env->sustain = sustain;
    set_decay_threshold(env);
}

void envelope_set_release(envelope_t *env, envelope_rate_t release)
{
    env->release = release;
}

void envelope_open_gate(envelope_t *env)
{
    env->gate_open = true;
    if (ENVELOPE_STATE_IDLE == env->state)
    {
        // Start attack immediately.
        env->state = ENVELOPE_STATE_ATTACK;
        env->accumulator = 0;
    }
    else
    {
        // Envelope is active; go through fast release before starting attack.
        env->state = ENVELOPE_STATE_FAST_RELEASE;
    }
}

void envelope_close_gate(envelope_t *env)
{
    env->gate_open = false;
    if (ENVELOPE_STATE_FAST_RELEASE != env->state)
    {
        // Start release.
        env->accumulator = (envelope_rate_t)env->level << RATE_SHIFT;
        env->state = ENVELOPE_STATE_RELEASE;
    }
}

void envelope_close_gate_fast(envelope_t *env)
{
    // Start fast release.
    env->gate_open = false;
    env->state = ENVELOPE_STATE_FAST_RELEASE;
}
