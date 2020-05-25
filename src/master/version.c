#include "version.h"

__attribute__((space(prog), section(".app_version")))
volatile const version_t app_version = {0, 0, 1};
