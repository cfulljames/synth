#include "timer.h"
#include "xc.h"
#include <stdbool.h>

void timer_init(void)
{
    // Normal mode, no output.
    CCP1CON1Lbits.MOD = 0;

    // 16-bit mode.
    CCP1CON1Lbits.T32 = 0;

    // Use Fosc/2 as clock source.
    CCP1CON1Lbits.CLKSEL = 0;

    // Use 1:16 prescaler.
    CCP1CON1Lbits.TMRPS = 2;

    // Set timer period. 4 MHz / 16 / 12500 = 20 Hz (50 ms)
    CCP1PRL = 12500 - 1;
}

void timer_deinit(void)
{
    // Reset all registers to default states.
    CCP1CON1L = 0;
    CCP1PRL = 0;
    CCP1TMRL = 0;
    _CCT1IF = 0;
}

void timer_reset(void)
{
    // Disable the timer.
    CCP1CON1Lbits.CCPON = 0;

    // Clear the interrupt flag.
    _CCT1IF = 0;

    // Reset the timer counter.
    CCP1TMRL = 0;
}

void timer_start(void)
{
    // Enable the timer.
    CCP1CON1Lbits.CCPON = 1;
}

bool timer_check_expired(void)
{
    // If the interrupt flag has been set, the timer rolled over at least once.
    return _CCT1IF;
}
