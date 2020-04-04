#include "audio.h"
#include "dac.h"
#include "synth.h"
#include "timer.h"
#include "voice.h"
#include "envelope.h"
#include "oscillator.h"
#include "cmd_parser.h"
#include "common_cmd.h"

#include <stdio.h>

void audio_init(void)
{
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

TIMER_SAMPLE_TIMER_ISR()
{
    TIMER_CLEAR_SAMPLE_TIMER_INTERRUPT_FLAG();

    // Write the next sample to the DAC.
    dac_set(synth_sample);

    // Set the flag to indicate the sample was written.
    synth_dac_ready = 1;
}
