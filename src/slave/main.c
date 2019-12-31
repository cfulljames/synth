#include <xc.h>

#include "system.h"

int main(void)
{
    system_init();

    // Configure LED pin (RE0) as output
    TRISEbits.TRISE1 = 0;

    while (1)
    {
        // Toggle LED pin.
        PORTEbits.RE1 = !PORTEbits.RE1;

        // Wait.
        for (volatile long i = 0; i < 1000000; i ++);
    }

    return 0;
}
