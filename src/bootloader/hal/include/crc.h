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
 * Reset the CRC peripheral to its default state.
 */
void crc_deinit(void);

/*
 * Calculate the CRC for the given data.
 *
 * This always re-seeds the CRC, so any previous result is overwritten.
 */
void crc_calculate(const uint8_t *data, uint32_t length);

/*
 * Seed the CRC to start a new calculation.
 *
 * This should be called at the beginning of each new set of data.
 */
void crc_seed(void);

/*
 * Get the CRC calculation result.
 *
 * This should be called at the end of each set of data to retrieve the
 * calculated CRC value.
 */
uint32_t crc_get_result(void);

#endif
