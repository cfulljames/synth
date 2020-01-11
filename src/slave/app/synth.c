#include "synth.h"
#include "dac.h"

// This file is generated using a python script.  It just contains a sine table,
// g_sine_table, and a size macro SINE_TABLE_LENGTH.
#include "sine.h"

#include <stdio.h>

void synth_init(void)
{
    // Configure Capture Compare Timer 1 for use as the sample timer.
    // (~44100 Hz)

    // Clear interrupt flag
    _CCT1IF = 0;

    // Enable interrupt
    _CCT1IE = 1;

    // Set priority
    _CCT1IP = 1;

    // Use Fosc as clock source.
    CCP1CON1Lbits.CLKSEL = 2;

    // Set timer period.  200 MHz / 4535 = 44101 Hz
    CCP1PRL = 4535;

    // Enable the sample timer
    CCP1CON1Lbits.CCPON = 1;
}

uint16_t sample;
volatile uint8_t ready = 0;

void synth_run(void)
{
    uint32_t index = 0;
    uint32_t freq = (uint32_t)(((2048.0 * 32.70) / 44101.0) * 65536);
    while (1)
    {
        // Wait for DAC to write the current sample.
        while (!ready);

        // Compute the next sample.
        sample = (g_sine_table[(index >> 16) & 0x07FF] + 0x8000) >> 4;
        index += freq;

        // Clear the flag.
        ready = 0;
    }
}

__attribute__((__interrupt__, auto_psv))
void _CCT1Interrupt(void)
{
    // Clear interrupt flag.
    _CCT1IF = 0;

    // Write the next sample to the DAC.
    dac_set(sample);

    // Set the flag to indicate the sample was written.
    ready = 1;
}
