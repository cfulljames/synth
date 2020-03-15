#include "audio.h"
#include "dac.h"
#include "synth.h"
#include "voice.h"
#include "envelope.h"
#include "oscillator.h"
#include "cmd_parser.h"
#include "common_cmd.h"

#include <stdio.h>

void audio_init(void)
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

    // Turn the status LED off.
    TRISEbits.TRISE1 = 0;
}

void audio_update(voice_t *voices, uint8_t num_voices)
{

    int16_t sample = 0;
    for (int8_t i = 0; i < num_voices; i ++)
    {
        sample += voice_update(&voices[i]);
    }

    // Turn the status LED off while waiting.
    PORTEbits.RE1 = 0;
    while (!synth_dac_ready);

    // Turn the status LED on while processing.
    PORTEbits.RE1 = 1;

    synth_sample = (sample + 0x8000) >> 4;
    synth_dac_ready = 0;
}

__attribute__((__interrupt__, auto_psv))
void _CCT1Interrupt(void)
{
    // Clear interrupt flag.
    _CCT1IF = 0;

    // Write the next sample to the DAC.
    dac_set(synth_sample);

    // Set the flag to indicate the sample was written.
    synth_dac_ready = 1;
}
