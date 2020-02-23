/*
 * Oscillator Update Function
 *
 * The oscillator update function is run very frequently, so it is defined as
 * static and placed in a header file to optimize the voice update routine.
 */
#ifndef _OSCILLATOR_UPDATE_H
#define _OSCILLATOR_UPDATE_H

#include "oscillator.h"
#include "sine.h"

#define INDEX_SHIFT (16U)
#define INDEX_MASK (0x07FF)

oscillator_output_t oscillator_update(oscillator_t *osc, int16_t phase)
{
    // Remove the factional portion of the accumulator.
    uint16_t base_index = osc->accumulator >> INDEX_SHIFT;

    // Add the phase offset and mask to get the sine table index.
    uint16_t index = (phase + base_index) & INDEX_MASK;

    oscillator_output_t value = sine_table[index];
    osc->accumulator += (osc->freq * osc->config->harmonic);
    return value;
}

#endif
