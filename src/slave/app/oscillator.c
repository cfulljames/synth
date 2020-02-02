#include "oscillator.h"
#include "sine.h"

#define INDEX_SHIFT (16U)
#define INDEX_MASK (0x07FF)

void oscillator_init(oscillator_t *osc)
{
    osc->freq = 0;
    osc->accumulator = 0;
}

oscillator_output_t oscillator_update(oscillator_t *osc, int16_t phase)
{
    // Remove the factional portion of the accumulator.
    uint16_t base_index = osc->accumulator >> INDEX_SHIFT;

    // Add the phase offset and mask to get the sine table index.
    uint16_t index = (phase + base_index) & INDEX_MASK;

    oscillator_output_t value = sine_table[index];
    osc->accumulator += osc->freq;
    return value;
}

void oscillator_set_frequency(oscillator_t *osc, oscillator_freq_t freq)
{
    osc->freq = freq;
}

void oscillator_reset_phase(oscillator_t *osc)
{
    osc->accumulator = 0;
}
