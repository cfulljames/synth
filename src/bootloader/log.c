#include "log.h"
#include "uart.h"
#include "xc.h"

// Number of bits to shift the upper nibble of a byte into the lower nibble.
#define UPPER_NIBBLE_SHIFT 4U

// Bitmask for isolating the lower nibble of a byte.
#define LOWER_NIBBLE_MASK 0x0F

// Number of bits to shift the upper byte of a word into the lower byte.
#define UPPER_BYTE_SHIFT 8U

// Bitmask for isolating the lower byte of a word.
#define LOWER_BYTE_MASK 0x00FF

// Number of 16-bit words per instruction space in flash.
#define WORDS_PER_INSTRUCTION 2U

// Number of instructions used to store the UDID.
#define UDID_LENGTH 5U

// The page number of the UDID.
#define UDID_PAGE 0x80

// The address offset of the least-significant double-word of the UDID.
#define UDID_FIRST_ADDRESS 0x1200

// The address of the most-significant double-word of the UDID.
#define UDID_LAST_ADDRESS \
    (UDID_FIRST_ADDRESS + (WORDS_PER_INSTRUCTION * (UDID_LENGTH - 1U)))

// Convert a 4-bit nibble to its corresponding hex character.
#define NIBBLE_TO_HEX_CHAR(x) ((x) < 0xA ? (x) + '0' : (x - 0xA) + 'a')

void log_str(char *str)
{
    uart_write_string(str);
}

void log_line(char *str)
{
    uart_write_string(str);
    uart_write('\n');
}

void log_serial_number(void)
{
    log_str("Serial Number: ");

    TBLPAG = UDID_PAGE;
    for (int address = UDID_LAST_ADDRESS;
            address >= UDID_FIRST_ADDRESS;
            address -= WORDS_PER_INSTRUCTION)
    {
        // Read 3 bytes of serial number from device.  Lower word contains two
        // bytes, upper word contains one.
        uint16_t lower_word = __builtin_tblrdl(address);
        uint16_t upper_word = __builtin_tblrdh(address);

        // Log bytes via UART.
        log_hex(upper_word & LOWER_BYTE_MASK);
        log_hex(lower_word >> UPPER_BYTE_SHIFT);
        log_hex(lower_word & LOWER_BYTE_MASK);

        // Log dashes between three-byte sections.
        if (address != UDID_FIRST_ADDRESS)
        {
            uart_write('-');
        }
    }
    uart_write('\n');
}

void log_hex(uint8_t data)
{
    uint8_t lower_nibble = data & LOWER_NIBBLE_MASK;
    uint8_t upper_nibble = data >> UPPER_NIBBLE_SHIFT;
    uart_write(NIBBLE_TO_HEX_CHAR(upper_nibble));
    uart_write(NIBBLE_TO_HEX_CHAR(lower_nibble));
}
