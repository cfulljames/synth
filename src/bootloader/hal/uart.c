#include "uart.h"

#include <xc.h>

#define UXMODEH_BCLKSEL_FOSC 0x2

#define CLOCK_FREQ_FOSC 8000000UL

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
    U1BRG = CLOCK_FREQ_FOSC / (UART_BAUDRATE * 4);
    U1BRGH = 0;

    // Enable UART TX
    U1MODEbits.UARTEN = 1;
    U1MODEbits.UTXEN = 1;
    U1MODEbits.URXEN = 1;
}

void uart_deinit(void)
{
    uart_flush_tx();

    U1MODEbits.UARTEN = 0;
    U1MODEbits.BRGH = 0;
    U1MODEHbits.BCLKSEL = 0;
    U1BRG = 0;
    U1BRGH = 0;

    U1MODEbits.UTXEN = 0;
    U1MODEbits.URXEN = 0;
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

uint8_t uart_read(uint8_t *rx_data)
{
    uint8_t received_bytes;
    if (U1STAHbits.URXBE)
    {
        // RX buffer is empty, no data available.
        received_bytes = 0;
    }
    else
    {
        // At least one byte in the buffer.
        received_bytes = 1;
        *rx_data = U1RXREG;
    }
    return received_bytes;
}

void uart_write_string(char *str)
{
    while (*str)
    {
        uart_write((uint8_t)*str);
        str ++;
    }
}

void uart_flush_rx(void)
{
    volatile uint8_t rx_data;
    while (!U1STAHbits.URXBE)
    {
        rx_data = U1RXREG;
    }
}

void uart_flush_tx(void)
{
    // Wait for TX buffer to empty.
    while(U1STAHbits.UTXBE == 0);

    // Wait for last byte to finish transmitting.
    while(U1STAbits.TRMT == 0);
}
