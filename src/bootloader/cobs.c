#include "cobs.h"

#include <stdbool.h>
#include <string.h>

int32_t cobs_encode(const uint8_t *data, int32_t length, uint8_t *output)
{
    uint32_t input_index = 0;
    uint32_t last_zero_index = 0;
    uint32_t output_index = 1;

    for (input_index = 0; input_index < length; input_index ++)
    {
        if (output_index - last_zero_index == UINT8_MAX)
        {
            // Maximum control byte value reached.  Need to insert a new
            // overhead byte to avoid overflowing the counter.
            output[last_zero_index] = UINT8_MAX;
            last_zero_index = output_index;
            output_index ++;
        }

        uint8_t currentByte = data[input_index];
        if (currentByte == 0)
        {
            // Found a new zero byte.  Store the location of this byte in the
            // previous control byte and update the zero index.
            output[last_zero_index] = output_index - last_zero_index;
            last_zero_index = output_index;
        }

        // Copy the next byte of data to the output.
        output[output_index] = data[input_index];
        output_index ++;
    }

    // The last control byte should point to the end of the message.
    output[last_zero_index] = output_index - last_zero_index;

    return output_index;
}

int32_t cobs_decode(const uint8_t *data, int32_t length, uint8_t *output)
{
    uint32_t input_index = 1;
    uint32_t next_zero_index = data[0];
    uint32_t output_index = 0;
    bool drop_next_zero = next_zero_index == UINT8_MAX;

    for (input_index = 1; input_index < length; input_index ++)
    {
        uint8_t currentByte = data[input_index];
        if (input_index == next_zero_index)
        {
            // Reached a control byte.  Its value determines the position of the
            // next one.
            next_zero_index = input_index + currentByte;

            if (!drop_next_zero)
            {
                // This byte replaced a zero in the original data.
                output[output_index] = 0;
                output_index ++;
            }

            // If the current byte is UINT8_MAX, the next control byte does not
            // replace a zero from the original data; it was inserted as
            // overhead.
            drop_next_zero = currentByte == UINT8_MAX;
        }
        else
        {
            // Just a normal data byte.
            output[output_index] = data[input_index];
            output_index ++;
        }
    }

    if (input_index != next_zero_index)
    {
        // Last control byte doesn't point to the end of the message, which
        // means that the message content may be incorrect.  Output data is
        // invalid.
        output_index = COBS_DECODE_FAIL;
    }

    return output_index;
}
