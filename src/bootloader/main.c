#include "sysconfig.h"
#include "system.h"
#include "uart.h"
#include "log.h"
#include "crc.h"
#include "xc.h"

int main()
{
    // Initialize peripherals needed for the bootloader.
    system_init();
    uart_init();

    log_line("Bootloader Started");
    log_serial_number();

    //uint8_t data[] = {0x12, 0x13, 0x14, 0x80, 0x95, 0x00, 0x11};
    __attribute__((aligned(4)))
    uint8_t data[] = {'1', '2', '3', '4', '5', '6', '7', '8'};
    uint16_t length = sizeof(data);

    crc_init();
    uint32_t result = crc_calculate(data, length);

    log_str("Result: ");
    for (int i = 3; i >= 0; i --)
    {
        log_hex((result >> (i * 8)) & 0xFF);
    }
    log_line("");

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
