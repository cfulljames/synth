#include "envelope.h"
#include "envelope_update.h"

#define ACCUMULATOR_MAX (UINT32_MAX)

#define RATE_SHIFT ENVELOPE_RATE_SHIFT

void envelope_config_init(envelope_config_t *cfg)
{
    cfg->attack = 0;
    cfg->attack_threshold = 0;
    cfg->decay = 0;
    cfg->decay_threshold = 0;
    cfg->sustain = 0;
    cfg->release = 0;
}

void envelope_init(envelope_t *env, envelope_config_t *cfg)
{
    env->level = 0;
    env->state = ENVELOPE_STATE_IDLE;
    env->gate_open = false;
    env->accumulator = 0;
    env->cfg = cfg;
}

__attribute__((always_inline))
static inline void set_decay_threshold(envelope_config_t *cfg)
{
    uint16_t decay_level = (envelope_rate_t)cfg->decay >> RATE_SHIFT;
    if (ENVELOPE_LEVEL_MAX - decay_level < cfg->sustain)
    {
        cfg->decay_threshold = (envelope_rate_t)ENVELOPE_LEVEL_MAX << RATE_SHIFT;
    }
    else
    {
        cfg->decay_threshold = cfg->decay
            + ((envelope_rate_t)cfg->sustain << RATE_SHIFT);
    }
}

void envelope_set_attack(envelope_config_t *cfg, envelope_rate_t attack)
{
    cfg->attack = attack;
    cfg->attack_threshold = ACCUMULATOR_MAX - attack;
}

void envelope_set_decay(envelope_config_t *cfg, envelope_rate_t decay)
{
    cfg->decay = decay;
    set_decay_threshold(cfg);
}

void envelope_set_sustain(envelope_config_t *cfg, envelope_level_t sustain)
{
    cfg->sustain = sustain;
    set_decay_threshold(cfg);
}

void envelope_set_release(envelope_config_t *cfg, envelope_rate_t release)
{
    cfg->release = release;
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
