#include "oscillator.h"
#include "oscillator_update.h"

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

void oscillator_set_frequency(oscillator_t *osc, oscillator_freq_t freq)
{
    osc->freq = freq;
}

void oscillator_reset_phase(oscillator_t *osc)
{
    osc->accumulator = 0;
}
