#include "timer.h"

void timer_init(void)
{
    // Configure Capture Compare Timer 1 for use as the sample timer.
    // (~44100 Hz)

    // Clear interrupt flag
    _CCT1IF = 0;

    // Enable interrupt
    _CCT1IE = 1;

    // Set priority
    _CCT1IP = 1;

    // Use Fosc as clock source.
    CCP1CON1Lbits.CLKSEL = 2;

    // Set timer period.  200 MHz / 4535 = 44101 Hz
    CCP1PRL = 4535;

    // Enable the sample timer
    CCP1CON1Lbits.CCPON = 1;
}
