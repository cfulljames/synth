#include "audio.h"
#include "dac.h"
#include "synth.h"
#include "voice.h"
#include "envelope.h"
#include "oscillator.h"

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
}
__attribute__((space(ymemory)))
int16_t mod_matrix[VOICE_OPERATORS_PER_VOICE+1][VOICE_OPERATORS_PER_VOICE] = {
    {0x0000, 0x0100, 0x0000, 0x0000},
    {0x0000, 0x0000, 0x0100, 0x0000},
    {0x0000, 0x0000, 0x0000, 0x0100},
    {0x0000, 0x0000, 0x0000, 0x0100},
    {0x2000, 0x2000, 0x2000, 0x2000},
};

__attribute__((space(xmemory)))
voice_t voice;

void audio_run(void)
{
    oscillator_config_t osc_cfg[VOICE_OPERATORS_PER_VOICE];
    for (int8_t i = 0; i < VOICE_OPERATORS_PER_VOICE; i ++)
    {
        oscillator_config_init(&osc_cfg[i]);
        oscillator_config_set_harmonic(&osc_cfg[i], 1);
    }

    envelope_config_t env_cfg[VOICE_OPERATORS_PER_VOICE];
    for (int8_t i = 0; i < VOICE_OPERATORS_PER_VOICE; i ++)
    {
        envelope_config_init(&env_cfg[i]);
        envelope_set_attack(&env_cfg[i], 0x00100000);
        envelope_set_release(&env_cfg[i], 0x00010000);
        envelope_set_sustain(&env_cfg[i], 1023);
    }

    voice_init(&voice, env_cfg, osc_cfg, mod_matrix);

    TRISEbits.TRISE1 = 0;

    uint16_t rxdata[10];
    uint8_t rxindex = 0;
    uint32_t active_freq = 0;

    while (1)
    {
        // Check for new data from master.
        while (!SI1FIFOCSbits.SRFEMPTY)
        {
            rxdata[rxindex] = SRMWFDATA;
            if (rxindex == 2)
            {
                // Received all 3 command words

                uint32_t freq = *(uint32_t*)(rxdata+1);
                switch (rxdata[0])
                {
                    case 0x0090:
                        // Note On
                        voice_start(&voice, freq);
                        active_freq = freq;
                        break;
                    case 0x0080:
                        // Note Off
                        if (freq == active_freq)
                        {
                            voice_release(&voice);
                        }
                        break;
                    default:
                        // Do nothing.
                        break;
                }
                rxindex = 0;
            }
            else
            {
                rxindex ++;
            }
        }

        int16_t sample = voice_update(&voice);

        PORTEbits.RE1 = 0;
        while (!synth_dac_ready);
        PORTEbits.RE1 = 1;

        synth_sample = (sample + 0x8000) >> 4;
        synth_dac_ready = 0;
    }
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
