/*
 * General Purpose Input/Output
 */
#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>

/*
 * Get the status of the bootloader button.
 *
 * This is checked in main.c to determine if the bootloader should wait for
 * commands or run the app.
 */
bool gpio_get_button_status(void);

#endif
