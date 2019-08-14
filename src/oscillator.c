#include "oscillator.h"

#define PERIOD_MIN FIXED_POINT_FROM_INT(2)

#define AMPLITUDE_MAX (127U)

#define SAMPLE_MAX UINT16_MAX

void Oscillator_Init(struct Oscillator_s *osc)
{
    osc->sampleCount = 0;
    osc->period = FIXED_POINT_FROM_INT(0);
    osc->pulseWidth = 0;
    osc->amplitude = 0;
    osc->running = false;
    osc->accumulator = FIXED_POINT_FROM_INT(0);
}

void Oscillator_SetPeriod(struct Oscillator_s *osc, FixedPoint_t period)
{
    osc->period = period;
    osc->sampleCount = 0;
    osc->accumulator = FIXED_POINT_FROM_INT(0);

    // Set pulse width again to update tOn.
    Oscillator_SetPulseWidth(osc, osc->pulseWidth);
}

void Oscillator_SetAmplitude(struct Oscillator_s *osc, uint8_t amplitude)
{
    if (amplitude > AMPLITUDE_MAX)
    {
        osc->amplitude = AMPLITUDE_MAX;
    }
    else
    {
        osc->amplitude = amplitude;
    }
}

void Oscillator_SetPulseWidth(struct Oscillator_s *osc,
        enum PulseWidth_e pulseWidth)
{
    if (pulseWidth >= PULSE_WIDTH_NUM_SETTINGS)
    {
        pulseWidth = PULSE_WIDTH_OFF;
    }

    osc->pulseWidth = pulseWidth;
    osc->tOn = FIXED_POINT_TO_INT(osc->period) >> pulseWidth;
}

void Oscillator_Stop(struct Oscillator_s *osc)
{
    osc->running = false;
}

void Oscillator_Start(struct Oscillator_s *osc)
{
    osc->running = true;
    osc->sampleCount = 0;
    osc->accumulator = FIXED_POINT_FROM_INT(0);
}

int8_t Oscillator_GetNextSample(struct Oscillator_s *osc)
{
    if (!osc->running
            || osc->pulseWidth == PULSE_WIDTH_OFF
            || osc->period <= FIXED_POINT_FROM_INT(1))
    {
        return 0;
    }

    int8_t sample = osc->amplitude;

    if (osc->sampleCount >= osc->tOn)
    {
        sample = -sample;
    }

    osc->sampleCount++;

    if (osc->sampleCount >= FIXED_POINT_TO_INT(osc->period + osc->accumulator))
    {
        // Add the fractional portion of the period to carry over the leftover
        // partial sample.
        osc->accumulator += osc->period & FIXED_POINT_MASK;

        // Mask away any full samples from the accumulator.
        osc->accumulator &= FIXED_POINT_MASK;

        osc->sampleCount = 0;
    }


    return sample;
}
