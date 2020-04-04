/*
 * Hardware Timer Configuration
 *
 * A hardware timer is used as the audio sample timer to trigger an interrupt
 * ~44101 times per second.  This is the closest to 44.1 kHz that the 100 MHz
 * clock can produce with an integer divider.
 */
#ifndef _TIMER_H
#define _TIMER_H

#include "xc.h"

// Sample timer interrupt service routine function signature.
#define TIMER_SAMPLE_TIMER_ISR() \
        __attribute__((__interrupt__, auto_psv)) void _CCT1Interrupt(void)

// Clear the timer interrupt flag.  This should be called in the ISR.
#define TIMER_CLEAR_SAMPLE_TIMER_INTERRUPT_FLAG() _CCT1IF = 0

// Sample rate in samples per second.
#define SAMPLE_RATE (44101)

/*
 * Initialize the audio sample timer.
 *
 * After initialization, the ISR will be called once for every sample.
 */
void timer_init(void);

#endif
