#include "app_status.h"

// This must match the definition in src/bootloader/app_status.c
#define APP_STATUS_ADDRESS (0x3000)

#define APP_VALID_MARKER (0xA5C3)

__attribute__((persistent, address(APP_STATUS_ADDRESS)))
static app_status_t m_app_status;

app_status_t app_status_get(void)
{
    return m_app_status;
}

void app_status_set(app_status_t status)
{
    m_app_status = status;
}
