#include "sysconfig.h"
#include "system.h"
#include "uart.h"
#include "xc.h"

static void log_serial_number(void);
static void log_hex(uint8_t data);

int main()
{
    // Initialize peripherals needed for the bootloader.
    system_init();
    uart_init();

    uart_puts("Bootloader Started");
    log_serial_number();

    // Deinitialize peripherals used by the bootloader to put them back in the
    // default state when the application launches.
    uart_deinit();
    system_deinit();

    while (1);

    // Jump to application.  The __APPLICATION_CODE_BASE symbol is defined in
    // the linker script, and points to the first instruction of the __reset
    // function in the application code.
    __asm__("goto __APPLICATION_CODE_BASE");
}

#define UDID_LENGTH 5U
#define UDID_PAGE 0x80
#define UDID_OFFSET 0x1200
static void log_serial_number(void)
{
    uart_puts("Serial Number:");

    TBLPAG = UDID_PAGE;
    for (int i = UDID_LENGTH - 1; i >= 0; i --)
    {
        // Read 3 bytes of serial from device.
        uint16_t address = UDID_OFFSET + (2 * i);
        uint16_t lower_word = __builtin_tblrdl(address);
        uint16_t upper_word = __builtin_tblrdh(address);

        // Log bytes via UART.
        log_hex(upper_word & 0x00FF);
        log_hex(lower_word >> 8);
        log_hex(lower_word & 0x00FF);

        // Log dashes between three-byte sections.
        if (i != 0)
        {
            uart_write('-');
        }
    }
    uart_write('\n');
}

#define NIBBLE_TO_HEX_CHAR(x) ((x) < 0xA ? (x) + '0' : (x - 0xA) + 'a')

static void log_hex(uint8_t data)
{
    uint8_t lower_nibble = data & 0x0F;
    uint8_t upper_nibble = data >> 4;
    uart_write(NIBBLE_TO_HEX_CHAR(upper_nibble));
    uart_write(NIBBLE_TO_HEX_CHAR(lower_nibble));
}
