#include <xc.h>
#include <libpic30.h>

#include "system.h"

// Oscillator configuration register values for using the primary (external)
// oscillator.
#define OSCCON_XOSC_PRIMARY_OSC_NO_PLL 0x2
#define OSCCON_XOSC_PRIMARY_OSC_WITH_PLL 0x3

static void clock_init(void);

void system_init(void)
{
    clock_init();
}

static void clock_init(void)
{
    if(OSCCONbits.COSC != OSCCON_XOSC_PRIMARY_OSC_NO_PLL)
    {
        // Not currently on primary (external) oscillator with PLL disabled;
        // switch to this configuration while configuring the PLL.

        // Set NSOC (lowest 3 bits of OSCCONH)
        __builtin_write_OSCCONH(OSCCON_XOSC_PRIMARY_OSC_NO_PLL);
        // Clear CLKLOCK and assert OSWEN = 1 to initiate switchover
        __builtin_write_OSCCONL((OSCCON & 0x7E) | 0x01);
        // Wait for switch over to complete.
        while(OSCCONbits.COSC != OSCCONbits.NOSC);
    }

    /*
     * PLL Frequency:
     * 1600MHz = 8MHz / 1 (N1) * 200 (M)
     *
     * Final clock frequency (Fosc):
     * 200MHz = 1600MHz / 4 (N2), followed by / 1 (N3), followed by / 2 (fixed)
     */

    // Configure PLL prescaler, both PLL postscalers, and PLL feedback divider
    CLKDIVbits.PLLPRE = 1;      // N1
    PLLFBDbits.PLLFBDIV = 200;  // M
    PLLDIVbits.POST1DIV = 4;    // N2
    PLLDIVbits.POST2DIV = 1;    // N3

    // Set NOSC (lowest 3 bits of OSCCONH)
    __builtin_write_OSCCONH(OSCCON_XOSC_PRIMARY_OSC_WITH_PLL);
    if(OSCCONbits.COSC != OSCCONbits.NOSC)
    {
        // Initiate Clock Switch to PLL

        // Assert OSWEN and make sure CLKLOCK is clear, to initiate the
        // switching operation
        __builtin_write_OSCCONL((OSCCON & 0x7E) | 0x01);
        // Wait for clock switch to finish
        while(OSCCONbits.COSC != OSCCONbits.NOSC);
    }

    /*
     * Configure AUX PLL for 500MHz output, from FRC input
     * Use FRC at 8MHz for clock input to AUX PLL.
     *
     * PLL Frequency:
     * 1.0 GHz = 8 MHz * 125
     *
     * Final output frequency:
     * 500 MHz = 1.0 GHz / 2 , followed by / 1
     */

    // APLLPRE = 1:1 mode, FRC as input
    ACLKCON1 = 0x0101;

    //ACLKCON1 = 0x0001;      //APLLPRE = 1:1 mode, primary oscillator as input

    APLLFBD1 = 125;
    APLLDIV1 = 0x0221;

    // Enable the AUX PLL now
    ACLKCON1bits.APLLEN = 1;
}
