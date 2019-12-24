#ifndef _ENVELOPE_H
#define _ENVELOPE_H

#include <stdint.h>

enum EnvelopeState_e {
    ENVELOPE_STATE_IDLE,
    ENVELOPE_STATE_ATTACK,
    ENVELOPE_STATE_DECAY,
    ENVELOPE_STATE_SUSTAIN,
    ENVELOPE_STATE_RELEASE,
};

struct Envelope_s {
    uint8_t attack;
    uint8_t decay;
    uint8_t sustain;
    uint8_t release;
    uint8_t output;
    enum EnvelopeState_e state;
};

void Envelope_Init(struct Envelope_s *env);

uint8_t Envelope_GetNextOutput(struct Envelope_s *env);

void Envelope_Start(struct Envelope_s *env);

void Envelope_Stop(struct Envelope_s *env);

void Envelope_SetAttack(struct Envelope_s *env, uint8_t attack);

#endif // _ENVELOPE_H
