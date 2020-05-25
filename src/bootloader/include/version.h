/*
 * Bootloader version numbers.
 */
#ifndef VERSION_H
#define VERSION_H

#include <stdint.h>

typedef struct version_s {
    uint8_t major;
    uint8_t minor;
    uint16_t patch;
} version_t;

extern const version_t bootloader_version;

#endif
