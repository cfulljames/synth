/*
 * Console Logger
 *
 * A small utility is provided for logging text to the console.  This includes
 * some functions for formatting data.
 */
#include <stdint.h>

/*
 * Log a string of text to the console.
 *
 * This will log the string as-is, and no additional characters will be added.
 * str must be a null-terminated string.
 */
void log_str(char *str);

/*
 * Log a string of text to the console with a newline character.
 *
 * This functions the same as log_str(), but prints an additional newline
 * character at the end.
 */
void log_line(char *str);

/*
 * Log the serial number.
 *
 * This will read the factory-programmed unique device identifier (UDID) out of
 * NVM and write log it via UART.
 */
void log_serial_number(void);

/*
 * Log two hexidecimal characters representing a single byte of data.
 */
void log_hex(uint8_t data);
