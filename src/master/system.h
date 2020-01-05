#ifndef __SYSTEM_H__
#define __SYSTEM_H__

// Clock frequencies configured by this module
#define CLOCK_FREQ_FOSC 180000000UL
#define CLOCK_FREQ_FOSC_2 90000000UL

/*
 * Perform low-level system initialization, including clock configuration and
 * starting the slave core.
 */
void system_init(void);

#endif
