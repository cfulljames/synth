#include "sysconfig.h"

__attribute__((__interrupt__, auto_psv, naked))
void _DefaultInterrupt(void)
{
    __asm__("break");
}

int main()
{
    // Jump to application.  There is a GOTO instruction stored in the first
    // two words of the application interrupt vector table that will start
    // executing the app.
    __asm__("goto __APPLICATION_CODE_BASE");
}
