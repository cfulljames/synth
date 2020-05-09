/*
 * Timer
 *
 * This timer module is used to control the serial receive timeout timer.
 */
#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>

/*
 * Initialize the timer.
 */
void timer_init(void);

/*
 * Restore the timer peripheral to its default state.
 */
void timer_deinit(void);

/*
 * Stop the timer and reset the counter.
 */
void timer_reset(void);

/*
 * Start the timer.
 */
void timer_start(void);

/*
 * Check to see if the timer has expired.
 */
bool timer_check_expired(void);

#endif
