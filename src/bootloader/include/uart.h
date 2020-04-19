/*
 * UART Driver
 *
 * A UART peripheral is used for communicating with the host PC to download the
 * new firmware.
 */
#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>

#define UART_BAUDRATE 115200

/*
 * Initialize the UART peripherals.
 */
void uart_init(void);

/*
 * Restore the UART peripherals to their default state.
 */
void uart_deinit(void);

/*
 * Write a byte over the UART.
 */
void uart_write(uint8_t tx_data);

/*
 * Write a line of text over the UART.
 *
 * str must be null-terminated.  A carriage return and newline will be added at
 * the end.
 */
void uart_puts(char *str);

/*
 * Wait for the UART to finish transmitting.
 */
void uart_flush_tx(void);

#endif
