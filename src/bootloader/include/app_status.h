/*
 * The application status is used by the bootloader to determine whether to run
 * the application.
 *
 * This flag is shared with the application, and is used to communicate the
 * requested bootloader behavior.  The flag is set to RUN by default, so the
 * bootloader will always attempt to run the application on boot.  Before
 * jumping to the application, the flag is set to DO_NOT_RUN by the bootloader
 * so that if the application is invalid or crashes immediately, the bootloader
 * will remain active after the reset to wait for external input, preventing a
 * boot loop.
 *
 * The following sequences illustrate this behavior:
 *
 * Valid application already loaded (typical case):
 * 1. app status is RUN by default
 * 2. app status is set to DO_NOT_RUN by bootloader
 * 3. application is launched
 * 4. app status is set to RUN by application after initialization succeeds
 * 5. if application causes an unexpected reset, it will be run again
 *
 * Invalid application (or application with broken startup) loaded:
 * 1. app status is RUN by default
 * 2. app status is set to DO_NOT_RUN by bootloader
 * 3. application is launched
 * 4. a reset occurs before application can modify app status
 * 5. bootloader remains active to prevent boot loop
 *
 * Bootloader request from application:
 * 1. app status is RUN by default
 * 2. app status is set to DO_NOT_RUN by bootloader
 * 3. application is launched
 * 4. app status is set to RUN by application after initialization success
 * 5. user requests bootloader mode
 * 6. application sets app status to DO_NOT_RUN and triggers reset
 * 7. bootloader remains active as requested by user
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
 * This will return RUN if:
 * - The application is marked as valid in flash.
 * AND
 * - The shared app_status variable is set to RUN.
 *
 * Otherwise it will return DO_NOT_RUN.
 */
app_status_t app_status_get(void);

/*
 * Request to set the status of the application.
 *
 * The request may fail in the event of a flash error, in which case the result
 * will be DO_NOT_RUN.
 */
app_status_t app_status_request(app_status_t status);

#endif
