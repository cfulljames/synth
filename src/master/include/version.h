/*
 * Application version numbers.
 */
#ifndef VERSION_H
#define VERSION_H

#include <stdint.h>

typedef struct version_s {
    uint8_t major;
    uint8_t minor;
    uint16_t patch;
} version_t;

extern volatile const version_t app_version;

#endif
