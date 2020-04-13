#include "system.h"

// This file is generated when the slave image is built and contains only the
// slave image pointer declaration.
#include "synth_slave.h"

#include <xc.h>
#include <libpic30.h>
#include <stdint.h>

// Oscillator configuration register values for using the primary (external)
// oscillator.
#define OSCCON_XOSC_PRIMARY_OSC_NO_PLL 0x2
#define OSCCON_XOSC_PRIMARY_OSC_WITH_PLL 0x3

// Output Pin Multiplexer Assignment Values
#define PINMUX_OUT_UART1_TX 0x01

static void interrupts_init(void);
static void clock_switch(uint8_t source);
static void clock_init(void);
static void pins_init(void);

void system_init(void)
{
    interrupts_init();
    clock_init();
    pins_init();

    // Program and start the slave core image.
    _program_slave(1,0,synth_slave);
    _start_slave();
}

static void interrupts_init(void)
{
    /*
     * The application provides its own interrupt vector table, which needs to
     * be activated here.  Otherwise the default IVT supplied with the
     * bootloader will be used.
     */
    INTCON2bits.AIVTEN = 1;
}

static void clock_init(void)
{
    // Use primary oscillator without PLL while making PLL adjustments.
    clock_switch(OSCCON_XOSC_PRIMARY_OSC_NO_PLL);

    /*
     * PLL Frequency:
     * 1440MHz = 8MHz / 1 (N1) * 180 (M)
     *
     * Final clock frequency (Fosc):
     * 180MHz = 1440MHz / 4 (N2), followed by / 1 (N3), followed by / 2 (fixed)
     */

    // Configure PLL prescaler, both PLL postscalers, and PLL feedback divider
    CLKDIVbits.PLLPRE = 1;      // N1=1
    PLLFBDbits.PLLFBDIV = 180;  // M = 180
    PLLDIVbits.POST1DIV = 4;    // N2=4
    PLLDIVbits.POST2DIV = 1;    // N3=1

    // Start using the PLL again now that it's ready.
    clock_switch(OSCCON_XOSC_PRIMARY_OSC_WITH_PLL);
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

    // Assign UART RX to RP58: Pin C10
    _U1RXR = 58;
    TRISCbits.TRISC10 = 1;
}
