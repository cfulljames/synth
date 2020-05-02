/*
 * Cyclic Redundancy Check
 *
 * CRC implementation using the dsPIC's CRC hardware peripheral.  Uses the
 * "standard" CRC32 algorithm.  This should be compatible with Python's built in
 * binascii.crc32 function.
 *
 * This is used to calculate the checksum for UART messages, as well as to
 * verify the application after writing it to flash.
 */
#ifndef CRC_H
#define CRC_H

#include <stdint.h>

/*
 * Initialize the CRC peripheral.
 *
 * This must be called at the beginning of each calculation to clear out any
 * previous data.
 */
void crc_init(void);

/*
 * Calculate the CRC for the given data.
 *
 * This can be called multiple times in succession to do the calculation in
 * multiple chunks.  It returns the current CRC value at the end of the
 * calculation.
 */
uint32_t crc_calculate(uint8_t *data, uint32_t length);

#endif
