#include "oscillator.h"
#include "sine.h"

#define INDEX_SHIFT (16U)
#define INDEX_MASK (0x07FF)

void oscillator_config_init(oscillator_config_t *cfg)
{
    cfg->harmonic = 1;
}

void oscillator_config_set_harmonic(oscillator_config_t *cfg, uint8_t harmonic)
{
    cfg->harmonic = harmonic;
}

void oscillator_init(oscillator_t *osc, oscillator_config_t *cfg)
{
    osc->freq = 0;
    osc->accumulator = 0;
    osc->config = cfg;
}

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

void oscillator_set_frequency(oscillator_t *osc, oscillator_freq_t freq)
{
    osc->freq = freq;
}

void oscillator_reset_phase(oscillator_t *osc)
{
    osc->accumulator = 0;
}
