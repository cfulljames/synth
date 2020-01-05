#include "uart.h"
#include "system.h"

#include <xc.h>

#define UXMODEH_BCLKSEL_FOSC 0x2

void uart_init(void)
{
    // Configure Debug UART (UART 1) for 115200 baud TX only

    // Disable UART during initialization.
    U1MODEbits.UARTEN = 0;

    // Set to high-speed mode (BRG * 4).
    U1MODEbits.BRGH = 1;

    // Use Fosc as baud rate reference clock.
    U1MODEHbits.BCLKSEL = UXMODEH_BCLKSEL_FOSC;

    // Set baud rate.
    U1BRG = CLOCK_FREQ_FOSC / (DEBUG_UART_BAUDRATE * 4);
    U1BRGH = 0;

    // Enable UART TX
    U1MODEbits.UARTEN = 1;
    U1MODEbits.UTXEN = 1;
}

void uart_write(uint8_t tx_data)
{
    // Wait for space to be available in the buffer.
    while (U1STAHbits.UTXBF)
    {
        ;
    }

    U1TXREG = tx_data;
}


// Map standard library write function to UART to support printf() and friends.
__attribute__((__section__(".libc.write")))
int write(int handle, void *buffer, unsigned int len)
{
    unsigned int i;

    for (i = 0; i < len; i ++)
    {
        uart_write(*(uint8_t*)buffer++);
    }

    return len;
}
