/*
 * System Error Traps
 *
 * This file contains definitions for the trap ISRs.  There's no header file,
 * since these functions should not be called directly.  They are explicitly
 * referenced in the interrupt vector table configured in the linker script.
 */
#include "xc.h"

#define ISR __attribute__((__interrupt__, no_auto_psv))

void ISR _DefaultInterrupt(void)
{
    __asm__("break");
}

void ISR _OscillatorFail(void)
{
    INTCON1bits.OSCFAIL = 0;
    __asm__("break");
}

void ISR _AddressError(void)
{
    INTCON1bits.ADDRERR = 0;
    __asm__("break");
}

void ISR _StackError(void)
{
    INTCON1bits.STKERR = 0;
    __asm__("break");
}

void ISR _MathError(void)
{
    INTCON1bits.MATHERR = 0;
    __asm__("break");
}

