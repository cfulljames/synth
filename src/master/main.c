#include "system.h"
#include "uart.h"

// System configuration will be performed at compile time when this file is
// included.
#include "sysconfig.h"

// Generated header containing note_freqs table declaration.
#include "notes.h"

#include <xc.h>
#include <stdio.h>

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

// TODO: Remove this and replace with proper MSI implementation
__attribute__((__interrupt__, auto_psv))
void _U1RXInterrupt(void)
{
    static uint8_t event_bytes = 0;

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

            // Forward command byte as-is
            MWSRFDATA = rxbyte;
        }
        else
        {
            uint16_t freq_word;

            // Continuing previous event.
            switch(event_bytes++)
            {
                case 0:
                    // First data byte - note number. Write frequency to slave.
                    freq_word = (uint16_t)note_freqs[rxbyte];
                    MWSRFDATA = freq_word;
                    freq_word = (uint16_t)(note_freqs[rxbyte] >> 16);
                    MWSRFDATA = freq_word;
                    break;
                default:
                    // Do nothing.
                    break;
            }
        }
    }
}
