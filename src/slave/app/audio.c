#include "audio.h"
#include "dac.h"
#include "timer.h"
#include "voice.h"
#include "envelope.h"
#include "oscillator.h"
#include "cmd_parser.h"
#include "common_cmd.h"

#include <stdio.h>
#include <stdbool.h>

// Flag signaling that the DAC is ready for a new sample.
volatile bool m_dac_ready;

// The next sample to write to the DAC.
volatile uint16_t m_sample;

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
    while (!m_dac_ready);

    // Turn the status LED on while processing.
    PORTEbits.RE1 = 1;

    m_sample = (sample + 0x8000) >> 4;
    m_dac_ready = false;
}

TIMER_SAMPLE_TIMER_ISR()
{
    TIMER_CLEAR_SAMPLE_TIMER_INTERRUPT_FLAG();

    // Write the next sample to the DAC.
    dac_set(m_sample);

    // Set the flag to indicate the sample was written.
    m_dac_ready = true;
}
