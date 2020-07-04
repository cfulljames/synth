#include "sysconfig.h"
#include "app_status.h"
#include "gpio.h"
#include "system.h"
#include "timer.h"
#include "uart.h"
#include "crc.h"
#include "serial.h"
#include "messages.h"
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
    messages_init();

    // Check the button status to determine if we should stay in bootloader
    // mode.
    if (gpio_get_button_status())
    {
        // The user held the button down during reset - hold the device in
        // bootloader mode to wait for serial commands.
        app_status_request(APP_STATUS_DO_NOT_RUN);
    }

    while (APP_STATUS_DO_NOT_RUN == app_status_get())
    {
        // The heart of the bootloader program is this serial update loop.  This
        // checks for new command messages received over UART and handles them
        // accordingly.
        serial_update();
    }

    // Set the app status back to DO_NOT_RUN so that if the application
    // immediately crashes and the device resets, the bootloader will not try to
    // run the app again.  This prevents a boot loop caused by a broken
    // application.
    app_status_request(APP_STATUS_DO_NOT_RUN);

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
