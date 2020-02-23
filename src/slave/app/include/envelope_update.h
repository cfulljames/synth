/*
 * Envelope Update Function
 *
 * The envelope update function is run very frequently, so it is defined as
 * static and placed in a header file to optimize the voice update routine.
 */
#ifndef _ENVELOPE_UPDATE_H
#define _ENVELOPE_UPDATE_H

#include "envelope.h"

#define RATE_SHIFT ENVELOPE_RATE_SHIFT

#define LEVEL_MAX ENVELOPE_LEVEL_MAX

#define FAST_RELEASE_RATE ((envelope_rate_t)10 << RATE_SHIFT)

__attribute__((always_inline))
static inline void update_attack(envelope_t *env)
{
    env->accumulator += env->cfg->attack;
    env->level = env->accumulator >> RATE_SHIFT;
    if (env->accumulator > env->cfg->attack_threshold)
    {
        // End of the attack stage; transition to decay.
        env->state = ENVELOPE_STATE_DECAY;
        env->accumulator = (envelope_rate_t)LEVEL_MAX << RATE_SHIFT;
    }
}

__attribute__((always_inline))
static inline void update_decay(envelope_t *env)
{
    if (env->accumulator <= env->cfg->decay_threshold)
    {
        // End of the decay stage; transition to sustain.
        env->state = ENVELOPE_STATE_SUSTAIN;
        env->level = env->cfg->sustain;
    }
    else
    {
        // Still decaying.
        env->level = env->accumulator >> RATE_SHIFT;
        env->accumulator -= env->cfg->decay;
    }
}

__attribute__((always_inline))
static inline void update_release(envelope_t *env)
{
    if (env->accumulator < env->cfg->release)
    {
        // End of the release stage; transition to idle.
        env->level = 0;
        env->state = ENVELOPE_STATE_IDLE;
    }
    else
    {
        // Still releasing.
        env->accumulator -= env->cfg->release;
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

envelope_level_t envelope_update(envelope_t *env)
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

    return env->level;
}

#endif
