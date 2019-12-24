#include "envelope.h"

#define ENV_MAX UINT8_MAX

void Envelope_Init(struct Envelope_s *env)
{
    env->state = ENVELOPE_STATE_IDLE;
    env->attack = ENV_MAX;
    env->output = 0;
}

void Envelope_Start(struct Envelope_s *env)
{
    env->state = ENVELOPE_STATE_ATTACK;
    env->output = 0;
}

void Envelope_Stop(struct Envelope_s *env)
{
    env->state = ENVELOPE_STATE_IDLE;
}

void Envelope_SetAttack(struct Envelope_s *env, uint8_t attack)
{
    env->attack = attack;
}

uint8_t Envelope_GetNextOutput(struct Envelope_s *env)
{
    switch (env->state)
    {
        case ENVELOPE_STATE_ATTACK:
            if (ENV_MAX - env->output < env->attack)
            {
                // Max output reached; attack finished.
                env->output = ENV_MAX;
                env->state = ENVELOPE_STATE_SUSTAIN;
            }
            else
            {
                env->output += env->attack;
            }
            break;
    }
    return env->output;
}
