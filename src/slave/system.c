#include <xc.h>
#include <libpic30.h>

#include "system.h"

// Oscillator configuration register values for using the primary (external)
// oscillator.
#define OSCCON_XOSC_PRIMARY_OSC_NO_PLL 0x2
#define OSCCON_XOSC_PRIMARY_OSC_WITH_PLL 0x3

// Output Pin Multiplexer Assignment Values
#define PINMUX_OUT_UART1_TX 0x01

static void clock_switch(uint8_t source);
static void clock_init(void);
static void pins_init(void);

void system_init(void)
{
    clock_init();
    pins_init();
}

static void clock_init(void)
{
    // Use primary oscillator without PLL while making PLL adjustments.
    clock_switch(OSCCON_XOSC_PRIMARY_OSC_NO_PLL);

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

    // Start using the PLL again now that it's ready.
    clock_switch(OSCCON_XOSC_PRIMARY_OSC_WITH_PLL);

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

static void clock_switch(uint8_t source)
{
    // Set NOSC (lowest 3 bits of OSCCONH)
    __builtin_write_OSCCONH(source);
    if(OSCCONbits.COSC != OSCCONbits.NOSC)
    {
        // Assert OSWEN and make sure CLKLOCK is clear, to initiate the
        // switching operation
        __builtin_write_OSCCONL((OSCCON & 0x7E) | 0x01);
        // Wait for clock switch to finish
        while(OSCCONbits.COSC != OSCCONbits.NOSC);
    }
}

static void pins_init(void)
{
    // Debug UART
    _RP59R = PINMUX_OUT_UART1_TX;
}
