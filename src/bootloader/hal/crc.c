#include "crc.h"
#include "xc.h"

// Standard CRC32 polynomial.  You can find this value in the datasheet, on
// Wikipedia on the CRC page, etc.
#define CRC_POLYNOMIAL 0x04C11DB7UL

// Starting value for the CRC XOR registers.
#define CRC_SEED 0xFFFFFFFFUL

static uint32_t reverse_bits(uint32_t data);

void crc_init(void)
{
    // Reset CRC configuration.
    CRCCON1 = 0;
    CRCCON2 = 0;

    // Set mode to "alternate".  This simplifies a couple of things: first, the
    // seed can be entered as a "direct" value (i.e. without modification), and
    // second, the result is available immediately after processing the last
    // byte of data instead of requiring additional dummy words to be supplied.
    CRCCON1bits.MOD = 1;

    // Enable the CRC.
    CRCCON1bits.CRCEN = 1;

    // Interrupt on CRC complete.
    CRCCON1bits.CRCISEL = 0;

    // Use little endian.
    CRCCON1bits.LENDIAN = 1;

    // Process data 8 bits at a time.
    CRCCON2bits.DWIDTH = 8 - 1;

    // Use a 32 bit polynomial.
    CRCCON2bits.PLEN = 32 - 1;

    // Ready... Set... GO!
    CRCCON1bits.CRCGO = 1;

    // Set the CRC polynomial.
    CRCXORL = CRC_POLYNOMIAL & 0x0000FFFF;
    CRCXORH = CRC_POLYNOMIAL >> 16;
}

void crc_deinit(void)
{
    CRCCON1 = 0;
    CRCCON2 = 0;
    CRCXORL = 0;
    CRCXORH = 0;
    CRCWDATL = 0;
    CRCWDATH = 0;
}

uint32_t crc_calculate(const uint8_t *data, uint32_t length)
{
    // Set the CRC starting value.
    CRCWDATL = CRC_SEED & 0x0000FFFF;
    CRCWDATH = CRC_SEED >> 16;

    // Calculate the CRC for all but the last byte of data.  The last byte
    // requires some special care to ensure we get the result at the right time,
    // so we'll do that separately below.
    while (length-- > 1)
    {
        // Wait for space to become available in the buffer.
        while (CRCCON1bits.CRCFUL);

        // Write one byte of data to the CRC data buffer.
        *(uint8_t*)&CRCDATL = *data++;
    }

    // Wait for space to become available in the buffer for the last byte.
    while (CRCCON1bits.CRCFUL);

    // Stop the CRC temporarily.  This ensures that when the interrupt flag is
    // set below, it's because of the new data, and not some old data that was
    // already being processed.
    CRCCON1bits.CRCGO = 0;

    // Clear the interrupt flag.
    _CRCIF = 0;

    // Write the last byte of data.
    *(uint8_t*)&CRCDATL = *data++;

    // Start the CRC back up and wait for the finished flag to be set.
    CRCCON1bits.CRCGO = 1;
    while (!_CRCIF);

    // Get the result.  Since the data was processed in little endian order, the
    // value needs to be reversed.
    uint32_t result = ((uint32_t)CRCWDATH << 16) | CRCWDATL;
    result = ~reverse_bits(result);
    return result;
}

/*
 * Reverse the order of bits in a 32 bit value.
 *
 * This is used to reverse the output of the CRC peripheral.  This isn't
 * strictly necessary, but it makes the result match the output of Python's
 * binascii.crc32 function.
 */
static uint32_t reverse_bits(uint32_t data)
{
    uint32_t input_mask = 0x80000000;
    uint32_t output_mask = 0x00000001;
    uint32_t result = 0;
    for (int i = 0; i < 32; i ++)
    {
        if (data & input_mask)
        {
            result |= output_mask;
        }
        input_mask >>= 1;
        output_mask <<= 1;
    }

    return result;
}
