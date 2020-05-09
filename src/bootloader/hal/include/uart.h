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
 * str must be null-terminated.
 */
void uart_write_string(char *str);

/*
 * Wait for the UART to finish transmitting.
 */
void uart_flush_tx(void);

/*
 * Read a byte from the UART.
 *
 * If a byte is available, it is placed in rx_data and a value of 1 is returned.
 * Otherwise, rx_data is unmodified and the function returns 0.
 */
uint8_t uart_read(uint8_t *rx_data);

#endif
