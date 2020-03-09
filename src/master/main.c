#include "system.h"
#include "uart.h"

// System configuration will be performed at compile time when this file is
// included.
#include "sysconfig.h"

// Generated header containing note_freqs table declaration.
#include "notes.h"

// Generated header containing envelope rate table declaration.
#include "envtable.h"

#include "common_cmd.h"

#include <xc.h>
#include <stdio.h>
#include <stdbool.h>

int main(void)
{
    system_init();
    uart_init();

    // Enable FIFO
    MSI1FIFOCSbits.WFEN = 1;

    printf("Master initialization complete.\n");

    // Configure LED pin (RE0) as output
    TRISEbits.TRISE0 = 0;

    while (1)
    {
        // Toggle LED pin.
        PORTEbits.RE0 = !PORTEbits.RE0;

        // Wait.
        for (volatile long i = 0; i < 1000000; i ++);
    }

    return 0;
}

#define NUM_VOICES (6U)


// TODO: Remove this and replace with proper MSI implementation
__attribute__((__interrupt__, auto_psv))
void _U1RXInterrupt(void)
{
    static int8_t voice_notes[NUM_VOICES];
    static uint8_t event_bytes = 0;
    static common_cmd_t cmd[3];
    static bool cc = false;
    static uint8_t voice = 0;

    // Clear interrupt flag.
    _U1RXIF = 0;

    // Keep reading as long as there is data in the buffer.
    while(!U1STAHbits.URXBE)
    {
        uint8_t rxbyte = U1RXREG;

        if (rxbyte & 0x80)
        {
            // Start of new MIDI event.
            event_bytes = 0;
            cmd[0].word_type = COMMON_CMD_WORD_TYPE_CMD;
            cmd[1].word_type = COMMON_CMD_WORD_TYPE_DATA;
            cmd[2].word_type = COMMON_CMD_WORD_TYPE_DATA;

            // Set command channel from MIDI channel.
            cmd[0].channel = rxbyte & 0x0F;

            cc = false;

            if ((rxbyte & 0xF0) == 0x90)
            {
                // Note on event.
                cmd[0].type = COMMON_CMD_TYPE_VOICE;
                cmd[0].command = COMMON_CMD_VOICE_START_NOTE;
                cmd[0].channel = voice;
            }
            else if ((rxbyte & 0xF0) == 0x80)
            {
                // Note off event.
                cmd[0].type = COMMON_CMD_TYPE_VOICE;
                cmd[0].command = COMMON_CMD_VOICE_RELEASE;
            }
            else if ((rxbyte & 0xF0) == 0xB0)
            {
                // Control change event.
                cc = true;
            }
        }
        else
        {
            uint32_t data = 0;

            // Continuing previous event.
            switch (event_bytes++)
            {
                case 0:
                    if (cc)
                    {
                        // First byte is CC number.
                        switch (rxbyte)
                        {
                            case 1:
                                cmd[0].type = COMMON_CMD_TYPE_OPERATOR;
                                cmd[0].command = COMMON_CMD_OPERATOR_SET_ATTACK;
                                break;
                            case 2:
                                cmd[0].type = COMMON_CMD_TYPE_OPERATOR;
                                cmd[0].command = COMMON_CMD_OPERATOR_SET_DECAY;
                                break;
                            case 3:
                                cmd[0].type = COMMON_CMD_TYPE_OPERATOR;
                                cmd[0].command = COMMON_CMD_OPERATOR_SET_SUSTAIN;
                                break;
                            case 4:
                                cmd[0].type = COMMON_CMD_TYPE_OPERATOR;
                                cmd[0].command = COMMON_CMD_OPERATOR_SET_RELEASE;
                                break;
                            case 5:
                                cmd[0].type = COMMON_CMD_TYPE_MODULATION;
                                cmd[0].command = COMMON_CMD_MODULATION_SET;
                                break;
                            case 8:
                                cmd[0].type = COMMON_CMD_TYPE_OPERATOR;
                                cmd[0].command = COMMON_CMD_OPERATOR_SET_OUTPUT;
                                break;
                            case 9:
                                cmd[0].type = COMMON_CMD_TYPE_OPERATOR;
                                cmd[0].command = COMMON_CMD_OPERATOR_SET_HARMONIC;
                                break;
                            case 10:
                                cmd[0].type = COMMON_CMD_TYPE_OPERATOR;
                                cmd[0].command = COMMON_CMD_OPERATOR_SET_BITMASK;
                                break;
                            default:
                                break;
                        }
                    }
                    else
                    {
                        // First data byte - note number.
                        if (cmd[0].command == COMMON_CMD_VOICE_RELEASE)
                        {
                            // Note off.

                            // Find voice playing this note.
                            bool found = false;
                            for (int8_t i = 0; i < NUM_VOICES; i ++)
                            {
                                if (voice_notes[i] == rxbyte)
                                {
                                    cmd[0].channel = i;
                                    voice_notes[i] = -1;
                                    found = true;
                                }
                            }

                            if (!found)
                            {
                                // No voice playing this note.
                                break;
                            }
                        }
                        else
                        {
                            // Note On
                            voice_notes[voice] = rxbyte;
                            data = note_freqs[rxbyte];
                            cmd[0].data_msb = data >> 30;
                            cmd[1].data = (data >> 15) & 0x7FFF;
                            cmd[2].data = data & 0x7FFF;
                            if (++voice >= NUM_VOICES)
                            {
                                voice = 0;
                            }
                        }

                        for (uint8_t i = 0; i < COMMON_CMD_NUM_WORDS; i ++)
                        {
                            // Wait for space to become available in the FIFO.
                            while (_WFFULL);

                            // Write next word to FIFO.
                            MWSRFDATA = cmd[i].value;
                        }
                    }
                    break;
                case 1:
                    if (cc)
                    {
                        if (cmd[0].type == COMMON_CMD_TYPE_OPERATOR)
                        {
                            switch (cmd[0].command)
                            {
                                case COMMON_CMD_OPERATOR_SET_ATTACK:
                                case COMMON_CMD_OPERATOR_SET_DECAY:
                                case COMMON_CMD_OPERATOR_SET_RELEASE:
                                    data = env_table[rxbyte];
                                    break;
                                case COMMON_CMD_OPERATOR_SET_SUSTAIN:
                                    data = (uint16_t)rxbyte << 3;
                                    break;
                                case COMMON_CMD_OPERATOR_SET_HARMONIC:
                                    data = (rxbyte >> 4) + 1;
                                    break;
                                case COMMON_CMD_OPERATOR_SET_BITMASK:
                                    data = 0xFFFF << (rxbyte >> 3);
                                    break;
                                case COMMON_CMD_OPERATOR_SET_OUTPUT:
                                    data = (uint16_t)rxbyte << 7;
                                    break;
                                default:
                                    break;
                            }
                        }
                        else if (cmd[0].type == COMMON_CMD_TYPE_MODULATION)
                        {
                            switch (cmd[0].command)
                            {
                                case COMMON_CMD_MODULATION_SET:
                                    data = (uint16_t)rxbyte << 3;
                                    break;
                                default:
                                    break;
                            }
                        }
                        cmd[0].data_msb = data >> 30;
                        cmd[1].data = (data >> 15) & 0x7FFF;
                        cmd[2].data = data & 0x7FFF;

                        for (uint8_t i = 0; i < COMMON_CMD_NUM_WORDS; i ++)
                        {
                            // Wait for space to become available in the FIFO.
                            while (_WFFULL);

                            // Write next word to FIFO.
                            MWSRFDATA = cmd[i].value;
                        }
                    }
                    else
                    {
                        // Do nothing.
                    }
                    break;

                default:
                    // Do nothing.
                    break;
            }
        }
    }
}
