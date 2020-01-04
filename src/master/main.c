#include <xc.h>

#include "system.h"

// System configuration will be performed at compile time when this file is
// included.
#include "sysconfig.h"

int main(void)
{
    system_init();

    // Configure LED pin (RE0) as output
    TRISEbits.TRISE0 = 0;

    while (1)
    {
        // Toggle LED pin.
        PORTEbits.RE0 = !PORTEbits.RE0;

        // Wait.
        for (volatile long i = 0; i < 1000000; i ++);
    }

    return 0;
}
