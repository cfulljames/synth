#include "app_status.h"
#include "flash.h"
#include <stdint.h>
#include <stdbool.h>

#define APP_STATUS_ADDRESS (0x3000)

#define APP_VALID_MARKER (0xA5C3)

__attribute__((persistent, address(APP_STATUS_ADDRESS)))
static app_status_t m_app_status;

app_status_t app_status_get(void)
{
    uint32_t app_valid_word;
    app_status_t current_app_status;
    flash_status_t flash_status;

    flash_status = flash_read_word(APP_VALID_FIRST_ADDRESS, &app_valid_word);

    if (flash_status != FLASH_OK)
    {
        // Encountered a flash error; don't run the application.
        current_app_status = APP_STATUS_DO_NOT_RUN;
    }
    else
    {
        // Check to see if the app has been marked as valid.
        uint16_t app_valid_status = app_valid_word & 0x0000FFFF;
        bool app_is_valid = app_valid_status == APP_VALID_MARKER;

        // Use the read/requested status if the app is valid, otherwise always
        // return DO_NOT_RUN.
        current_app_status =
            app_is_valid ? m_app_status : APP_STATUS_DO_NOT_RUN;
    }

    return current_app_status;
}

app_status_t app_status_request(app_status_t status)
{
    if (status == APP_STATUS_RUN)
    {
        // Requesting to run the application; mark the app as valid.  It is the
        // caller's responsibility to ensure validity before updating the
        // status.
        flash_status_t status = flash_write_dword(
                APP_VALID_FIRST_ADDRESS, APP_VALID_MARKER, 0x00FFFFFF);

        // Only set the RUN status if the flash write was successful.
        m_app_status =
            (status == FLASH_OK) ? APP_STATUS_RUN : APP_STATUS_DO_NOT_RUN;
    }
    else
    {
        // Not requesting to run, just update the status.
        m_app_status = status;
    }

    return app_status_get();
}
