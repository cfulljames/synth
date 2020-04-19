#include "sysconfig.h"
#include "system.h"
#include "uart.h"
#include "log.h"
#include "xc.h"

int main()
{
    // Initialize peripherals needed for the bootloader.
    system_init();
    uart_init();

    log_line("Bootloader Started");
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
