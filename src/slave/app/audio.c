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
}
__attribute__((space(ymemory)))
int16_t mod_matrix[VOICE_OPERATORS_PER_VOICE+1][VOICE_OPERATORS_PER_VOICE] = {
    {0x0000, 0x0000, 0x0000, 0x0000},
    {0x0000, 0x0000, 0x0000, 0x0000},
    {0x0000, 0x0000, 0x0000, 0x0000},
    {0x0000, 0x0000, 0x0000, 0x0000},
    {0x0000, 0x0000, 0x0000, 0x0000},
};

__attribute__((space(xmemory)))
voice_t voice;
oscillator_config_t osc_cfg[VOICE_OPERATORS_PER_VOICE];
envelope_config_t env_cfg[VOICE_OPERATORS_PER_VOICE];

void cmd_callback(
        uint16_t type, uint16_t channel, uint16_t command, uint32_t data)
{
    uint16_t in;
    uint16_t out;
    switch (type)
    {
        case COMMON_CMD_TYPE_GLOBAL:
            break;
        case COMMON_CMD_TYPE_OPERATOR:
            switch (command)
            {
                case COMMON_CMD_OPERATOR_SET_HARMONIC:
                    oscillator_config_set_harmonic(&osc_cfg[channel], data);
                    break;
                case COMMON_CMD_OPERATOR_SET_OUTPUT:
                    mod_matrix[VOICE_OPERATORS_PER_VOICE][channel] = data;
                    break;
                case COMMON_CMD_OPERATOR_SET_ATTACK:
                    envelope_set_attack(&env_cfg[channel], data);
                    break;
                case COMMON_CMD_OPERATOR_SET_DECAY:
                    envelope_set_decay(&env_cfg[channel], data);
                    break;
                case COMMON_CMD_OPERATOR_SET_SUSTAIN:
                    envelope_set_sustain(&env_cfg[channel], data);
                    break;
                case COMMON_CMD_OPERATOR_SET_RELEASE:
                    envelope_set_release(&env_cfg[channel], data);
                    break;
            }
            break;
        case COMMON_CMD_TYPE_VOICE:
            switch (command)
            {
                case COMMON_CMD_VOICE_START_NOTE:
                    voice_start(&voice, data);
                    break;
                case COMMON_CMD_VOICE_RELEASE:
                    voice_release(&voice);
                    break;
                default:
                    break;
            }
            break;
        case COMMON_CMD_TYPE_MODULATION:
            switch (command)
            {
                case COMMON_CMD_MODULATION_SET:
                    out = channel >> 2;
                    in = channel & 0x03;
                    mod_matrix[out][in] = data;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void audio_run(void)
{
    cmd_parser_init();
    cmd_parser_set_callback(cmd_callback);

    for (int8_t i = 0; i < VOICE_OPERATORS_PER_VOICE; i ++)
    {
        oscillator_config_init(&osc_cfg[i]);
        //oscillator_config_set_harmonic(&osc_cfg[i], 1);
    }

    for (int8_t i = 0; i < VOICE_OPERATORS_PER_VOICE; i ++)
    {
        envelope_config_init(&env_cfg[i]);
        //envelope_set_attack(&env_cfg[i], 0x00100000);
        //envelope_set_release(&env_cfg[i], 0x00010000);
        //envelope_set_sustain(&env_cfg[i], 1023);
    }

    voice_init(&voice, env_cfg, osc_cfg, mod_matrix);

    TRISEbits.TRISE1 = 0;

    while (1)
    {
        cmd_parser_update();

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
