/*
 * Consistent Overhead Byte Stuffing
 *
 * This module provides a simple implementation of the COBS encoding algorithm.
 * COBS provides a simple, elegant way to remove all zero bytes from a stream of
 * data, adding only a small amount of overhead.  This allows a zero byte to be
 * used as a packet end marker, which can significantly reduce the probability
 * of framing errors.
 */
#ifndef COBS_H
#define COBS_H

#include <stdint.h>

/*
 * Calculate the maximum number of bytes needed to store the COBS encoding for n
 * bytes of input data.  When using the encode function, this should be used to
 * calculate the necessary minimum output buffer size.
 */
#define COBS_MAX_DATA_LENGTH_AFTER_ENCODE(n) ((n) + (((n) + 253) / 254))

/*
 * Calculate the maximum number of bytes needed to store the decoded data for n
 * bytes of encoded COBS data.  When using the decode function, this should be
 * used to calculate the necessary minimum output buffer size.
 */
#define COBS_MAX_DATA_LENGTH_AFTER_DECODE(n) ((n) - 1)

/*
 * Value returned by cobs_decode() signaling that the decoding failed due to
 * invalid input data.
 */
#define COBS_DECODE_FAIL (-1L)

/*
 * Encode data using COBS.
 *
 * Params:
 *   data - The array of data to be encoded.
 *   length - The length of the data array before encoding.
 *   output - Array used to store the encoded result.
 *
 * Returns:
 *   The length (in bytes) of the encoded data.
 */
int32_t cobs_encode(const uint8_t *data, int32_t length, uint8_t *output);

/*
 * Decode COBS data.
 *
 * Params:
 *   data - The array of COBS-encoded data to be decoded.
 *   length - The length of the data array before decoding.
 *   output - Array used to store the decoded result.
 *
 * Returns:
 *   The length (in bytes) of the decoded data.
 */
int32_t cobs_decode(const uint8_t *data, int32_t length, uint8_t *output);

#endif // COBS_H
