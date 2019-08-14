#ifndef _OSCILLATOR_H
#define _OSCILLATOR_H

#include <stdint.h>
#include <stdbool.h>
#include "fixed_point.h"


//! Pulse width settings.
enum PulseWidth_e {
    PULSE_WIDTH_OFF = 0,
    PULSE_WIDTH_50 = 1,
    PULSE_WIDTH_25 = 2,
    PULSE_WIDTH_12 = 3,

    PULSE_WIDTH_NUM_SETTINGS,
};

/*!
 * Oscillator instance.  Do not modify any of these members directly; use the
 * functions below instead.
 */
struct Oscillator_s {

    //! The period of the waveform.
    FixedPoint_t period;

    //! Flag indicating that the oscillator is running.
    bool running;

    //! The amplitude of the oscillator (divided by 2).
    uint8_t amplitude;

    //! The pulse width of the oscillator.
    enum PulseWidth_e pulseWidth;

    //! The on time of the oscillator.
    uint16_t tOn;

    //! Counter to keep track of how many samples have elapsed.
    uint16_t sampleCount;

    //! Accumulation of leftover partial samples from previous periods.
    FixedPoint_t accumulator;
};

/*!
 * Initialize an oscillator.
 *
 * This will set the parameters to their default values.  It should be called
 * before performing any other operations with the oscillator.
 */
void Oscillator_Init(struct Oscillator_s *osc);

/*!
 * Set the oscillator's period in samples.
 */
void Oscillator_SetPeriod(struct Oscillator_s *osc, FixedPoint_t period);

/*!
 * Set the oscillator's amplitude.
 *
 * The amplitude should be a value from 0 - 127.
 * The actual value will be set to 127 if a larger amplitude is given.
 */
void Oscillator_SetAmplitude(struct Oscillator_s *osc, uint8_t amplitude);

/*!
 * Set the pulse width of the oscillator.
 */
void Oscillator_SetPulseWidth(struct Oscillator_s *osc,
        enum PulseWidth_e pulseWidth);

/*!
 * Stop the oscillator.
 *
 * If the oscillator was running, calling this stops the counters from
 * incrementing and causes the oscillator to silence its output.  If it wasn't
 * running, this has no effect.
 */
void Oscillator_Stop(struct Oscillator_s *osc);

/*!
 * Start the oscillator.
 *
 * This must be called to allow the oscillator to output a signal.  If the
 * oscillator was already running, this will reset its phase.
 */
void Oscillator_Start(struct Oscillator_s *osc);

/*!
 * Get the next sample from the oscillator's output.
 *
 * This is where the magic happens.  This function returns the next sample in
 * the oscillator's signal.  Counters are automatically incremented, so calling
 * this function repeatedly will produce an oscillating waveform.
 */
int8_t Oscillator_GetNextSample(struct Oscillator_s *osc);

#endif // _OSCILLATOR_H
