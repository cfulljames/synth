/*
 * Envelope Generator
 *
 * The envelope generator can be used to control the level of a parameter over
 * time.  Two basic events are used to control the state of the envelope:
 * opening and closing the "gate".  Normally, these events correspond to
 * pressing and releasing a key on a keyboard, respectively.
 *
 * This envelope generator has the following adjustable parameters:
 *
 * - Attack:
 *      The rate at which the level increases immediately after the gate is
 *      opened.
 *
 * - Decay:
 *      The rate at which the level decreases after reaching the maximum value
 *      during the attack stage.
 *
 * - Sustain:
 *      When the level reaches this value during the decay stage, it will stop
 *      decreasing and maintain this value until the gate is shut.
 *
 * - Release:
 *      The rate at which the level decreases after the gate is shut.
 *
 *
 *                | .           .     .                    .     .
 *                | *<- Gate Open     .       Gate Close ->*     .
 *                | .           .     .                    .     .
 *                | .           .     .                    .     .
 *                | .          /\     .                    .     .
 *                | .         / .\    .                    .     .
 *                | .        /  . \   .                    .     .
 *                | .       /   .  \  .                    .     .
 *        output  | .      /    .   \ .                    .     .
 *        level   | .     /     .    \_____________________.     .
 *                | .    /      .     .                    \     .
 *                | .   /       .     .                    .\    .
 *                | .  /        .     .                    . \   .
 *                | . /         .     .                    .  \  .
 *                | ./          .     .                    .   \ .
 *                | /           .     .                    .    \.
 *                +------------------------------------------------
 *                  .           .     . time               .     .
 *                  .           .     .                    .     .
 *                  <----------><-----><------------------><----->
 *   Active State:     Attack    Decay       Sustain       Release
 *
 *                            Figure 1: Envelope Output
 *
 *
 * Note: While the state is shown on the time axis above, the parameters are not
 * set in units of time.  See the functions below for details.
 *
 *
 * Fast Release:
 *
 * This implementation also features a "fast release" feature.  When the gate is
 * closed using the "fast" option or opened two times in succession without
 * closing in between, the fast release will be triggered.  The fast release is
 * the same as the normal release stage, but uses a fixed rate instead of the
 * given release parameter.  This is useful, for example, to re-trigger a note
 * without having to wait for the release stage to complete with a low rate, or
 * to abruptly switch to a new tone without creating an audible click when all
 * available voices are already active and a new key is pressed.
 */

#ifndef _ENVELOPE_H
#define _ENVELOPE_H

#include <stdint.h>
#include <stdbool.h>

// Type used for the level output and sustain parameter.  This is a normal
// integer.  Only the lowest (32 - ENVELOPE_RATE_SHIFT) bits are used.
typedef uint16_t envelope_level_t;

// The max value of the level type.
#define ENVELOPE_LEVEL_MAX (UINT16_MAX >> (ENVELOPE_RATE_SHIFT - 16U))

// Type used for the rate of change parameters.  This is a fixed point value.
// The value of ENVELOPE_RATE_SHIFT is used to control the number of bits used
// to represent the fractional portion; e.g. if ENVELOPE_RATE_SHIFT is 16,
// there will be 16 fractional bits and 16 integer bits.
typedef uint32_t envelope_rate_t;

// The number of bits in a rate used for the fractional portion.
#define ENVELOPE_RATE_SHIFT (22U)

// The max value of the rate type.
#define ENVELOPE_RATE_MAX (ENVELOPE_LEVEL_MAX << ENVELOPE_RATE_SHIFT)

/******************************************************************************
 * Configuration
 ******************************************************************************/

// Envelope configuration.  Do not modify directly.  By separating these values
// into a separate structure, it allows multiple envelopes to share the same
// configuration.
typedef struct envelope_config_s {

    // The configured attack rate.
    envelope_rate_t attack;

    // The configured decay rate.
    envelope_rate_t decay;

    // The configured sustain level.
    envelope_level_t sustain;

    // The configured release level.
    envelope_rate_t release;

    // The level at which the envelope transitions from attack to decay.
    envelope_rate_t attack_threshold;

    // The level at which the envelope transitions from decay to sustain.
    envelope_rate_t decay_threshold;

} envelope_config_t;

/*
 * Initialize the envelope configuration structure with default parameters.
 *
 * This should be called before passing the envelope config structure to any of
 * the other functions.
 */
void envelope_config_init(envelope_config_t *cfg);

/*
 * Set the attack rate of the envelope.
 *
 * This is the amount that the level will increase with each update during the
 * attack stage.
 */
void envelope_set_attack(envelope_config_t *cfg, envelope_rate_t attack);

/*
 * Set the decay rate of the envelope.
 *
 * This is the amount that the level will decrease with each update during the
 * decay stage.
 */
void envelope_set_decay(envelope_config_t *cfg, envelope_rate_t decay);

/*
 * Set the sustain level of the envelope.
 *
 * This is the level at which the decay stage will stop decreasing.  Once this
 * level is reached, it will be maintained until the gate is closed.
 */
void envelope_set_sustain(envelope_config_t *cfg, envelope_level_t sustain);

/*
 * Set the release rate of the envelope.
 *
 * This is the amount that the level will decrease with each update during the
 * release stage.
 */
void envelope_set_release(envelope_config_t *cfg, envelope_rate_t release);

/******************************************************************************
 * Running
 ******************************************************************************/

// The internal state of the envelope.  Should not be used externally.
typedef enum envelope_state_e {
    ENVELOPE_STATE_IDLE,
    ENVELOPE_STATE_ATTACK,
    ENVELOPE_STATE_DECAY,
    ENVELOPE_STATE_SUSTAIN,
    ENVELOPE_STATE_RELEASE,
    ENVELOPE_STATE_FAST_RELEASE,
} envelope_state_t;

// The envelope data structure.  Do not modify this structure directly; instead,
// use the functions below to adjust parameters.
typedef struct envelope_s {

    // The current output level of the envelope.
    envelope_level_t level;

    // The current state of the envelope.
    envelope_state_t state;

    // Flag indicating the current state of the envelope gate.
    bool gate_open;

    // General purpose accumulator used for state transition timing.
    uint32_t accumulator;

    // The configuration for this envelope.
    envelope_config_t *cfg;

} envelope_t;

/*
 * Initialize the envelope structure with default parameters.
 *
 * This should be called before passing the envelope structure to any of the
 * other functions.
 */
void envelope_init(envelope_t *env, envelope_config_t *cfg);

/*
 * Update the envelope internal state and output level.
 *
 * Normally, this should be called periodically, e.g. once per sample.
 */
#ifdef TEST
#define STATIC_INLINE
#else
#define STATIC_INLINE __attribute__((always_inline)) static inline
#endif
STATIC_INLINE envelope_level_t envelope_update(envelope_t *env);

#ifndef TEST
// The function is defined in a header to allow it to be inlined.
#include "envelope_update.h"
#endif

/*
 * Open the envelope gate.
 *
 * If the envelope is idle, this will start a new attack stage.  Otherwise, it
 * will start the fast release stage.  When the fast release is complete, it
 * will then start the attack stage automatically as long as the gate is still
 * open.
 */
void envelope_open_gate(envelope_t *env);

/*
 * Close the envelop gate.
 *
 * If the envelope is idle or a release is already in progress, this will have
 * no effect.  Otherwise, it will start the release stage.
 */
void envelope_close_gate(envelope_t *env);

/*
 * Close the envelope gate and trigger a fast release.
 *
 * This is the same as envelope_close_gate(), except that it uses a fixed
 * release rate.  This will also override a normal release.
 */
void envelope_close_gate_fast(envelope_t *env);

#endif // _ENVELOPE_H
