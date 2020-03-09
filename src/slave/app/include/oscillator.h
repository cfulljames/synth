/*
 * Oscillator
 *
 * The oscillator generates sine waves of a specified frequency.  A harmonic can
 * also be configured to allow for harmonic relationships between oscillators
 * without requiring a separate frequency to be supplied for each.
 */

#ifndef _OSCILLATOR_H
#define _OSCILLATOR_H

#include <stdint.h>

// Type for representing frequency of the oscillator.
typedef uint32_t oscillator_freq_t;

// Type for representing the oscillator output signal.
typedef int16_t oscillator_output_t;

/******************************************************************************
 * Configuration
 ******************************************************************************/

// Oscillator configuration that can be shared between multiple oscillators.  Do
// not modify directly.
typedef struct oscillator_config_s {
    uint8_t harmonic;
    uint16_t mask;
} oscillator_config_t;

/*
 * Initialize the oscillator configuration.
 *
 * This must be called before using the configuration with an oscillator.  This
 * configuration structure can be shared between multiple oscillators.
 */
void oscillator_config_init(oscillator_config_t *cfg);

/*
 * Set the oscillator harmonic.
 *
 * This allows for easy configuration of oscillators with different harmonics of
 * the same base frequency.
 */
void oscillator_config_set_harmonic(oscillator_config_t *cfg, uint8_t harmonic);

/*
 * Set the oscillator bitmask.
 *
 * This will be applied to the oscillator sample just before it is returned,
 * providing a bitcrusher or lo-fi effect.
 */
void oscillator_config_set_bitmask(oscillator_config_t *cfg, uint16_t mask);

/******************************************************************************
 * Running
 ******************************************************************************/

// Oscillator internal state.  Do not modify this structure directly.
typedef struct oscillator_s {
    oscillator_freq_t freq;
    oscillator_freq_t accumulator;
    oscillator_config_t *config;
} oscillator_t;

/*
 * Initialize the oscillator.
 */
void oscillator_init(oscillator_t *osc, oscillator_config_t *cfg);

/*
 * Update the oscillator and get the next output.
 *
 * To enable FM synthesis, a phase offset can be provided.  If used, the value
 * should be given in units of sine table indices, otherwise set to zero.  If
 * the value is larger than the sine table length, it will wrap around; this
 * allows for very large phase offsets to be given.
 */
#ifdef TEST
#define STATIC_INLINE
#else
#define STATIC_INLINE __attribute__((always_inline)) static inline
#endif
STATIC_INLINE oscillator_output_t oscillator_update(oscillator_t *osc, int16_t phase);

#ifndef TEST
// The function is defined in a header to allow it to be inlined.
#include "oscillator_update.h"
#endif

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
