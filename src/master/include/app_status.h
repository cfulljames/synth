/*
 * Application status for bootloader.
 *
 * See src/bootloader/include/app_status.h for details.
 */
#ifndef APP_STATUS_H
#define APP_STATUS_H

// Application status indicator
typedef enum app_run_status_e {
    APP_STATUS_RUN = 0x00,
    APP_STATUS_DO_NOT_RUN = 0xA5,
} app_status_t;

/*
 * Get the current status of the application.
 *
 * Unlike the bootloader version of this function, this does not need to check
 * for application validity - the application is already running, so it must be
 * valid.
 */
app_status_t app_status_get(void);

/*
 * Set the status of the application to control the bootloader behavior.
 */
void app_status_set(app_status_t status);

#endif
