#include "sysconfig.h"
#include "system.h"
#include "timer.h"
#include "uart.h"
#include "crc.h"
#include "serial.h"
#include "xc.h"

int main()
{
    // Initialize peripherals needed for the bootloader.
    system_init();
    uart_init();
    timer_init();
    crc_init();

    // Initialize bootloader application layer (no de-init required)
    serial_init();

    while (1)
    {
        // The heart of the bootloader program is this serial update loop.  This
        // checks for new command messages received over UART and handles them
        // accordingly.
        serial_update();
    }

    // Deinitialize peripherals used by the bootloader to put them back in the
    // default state when the application launches.
    crc_deinit();
    timer_deinit();
    uart_deinit();
    system_deinit();

    // Jump to application.  The __APPLICATION_CODE_BASE symbol is defined in
    // the linker script, and points to the first instruction of the __reset
    // function in the application code.
    __asm__("goto __APPLICATION_CODE_BASE");
}
