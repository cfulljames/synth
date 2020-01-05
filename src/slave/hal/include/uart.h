#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>

#define DEBUG_UART_BAUDRATE 115200

/*
 * Initialize the UART peripherals.
 */
void uart_init(void);

/*
 * Write a byte over the debug UART.
 */
void uart_write(uint8_t tx_data);

#endif
