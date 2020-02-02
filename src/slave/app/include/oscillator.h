#ifndef _OSCILLATOR_H
#define _OSCILLATOR_H

#include <stdint.h>

// Type for representing frequency of the oscillator.
typedef uint32_t oscillator_freq_t;

// Type for representing the oscillator output signal.
typedef int16_t oscillator_output_t;

// Oscillator internal state.  Do not modify this structure directly.
typedef struct oscillator_s {
    oscillator_freq_t freq;
    oscillator_freq_t accumulator;
} oscillator_t;

/*
 * Initialize the oscillator.
 */
void oscillator_init(oscillator_t *osc);

/*
 * Update the oscillator and get the next output.
 *
 * To enable FM synthesis, a phase offset can be provided.  If used, the value
 * should be given in units of sine table indices, otherwise set to zero.  If
 * the value is larger than the sine table length, it will wrap around; this
 * allows for very large phase offsets to be given.
 */
oscillator_output_t oscillator_update(oscillator_t *osc, int16_t phase);

/*
 * Set the frequency of the oscillator.
 *
 * Frequency should be given in units of "indices per sample", which can be
 * calculated by using the following formula:
 *
 * Freq (ips) = Freq (Hz) * (1 / Sample Rate) * (Sine Table Length)
 *
 * Where:
 *  - Sample Rate is in samples per second.
 *  - Sine table length is the number of entries in the sine table.
 */
void oscillator_set_frequency(oscillator_t *osc, oscillator_freq_t freq);

/*
 * Reset the phase of the oscillator.
 */
void oscillator_reset_phase(oscillator_t *osc);

#endif // _OSCILLATOR_H
