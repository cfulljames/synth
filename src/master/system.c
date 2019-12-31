#include <xc.h>
#include <libpic30.h>

#include "system.h"

// This file is generated when the slave image is built and contains only the
// slave image pointer declaration.
#include "synth_slave.h"

void system_init(void)
{
    //Switch to FRC (no divider, no PLL), assuming we aren't already running from that.
    if(OSCCONbits.COSC != 0b000)
    {
        __builtin_write_OSCCONH(0x00);  //NOSC = 0b000 = FRC without divider or PLL
        __builtin_write_OSCCONL((OSCCON & 0x7E) | 0x01);  //Clear CLKLOCK and assert OSWEN = 1 to initiate switchover
        //Wait for switch over to complete.
        while(OSCCONbits.COSC != OSCCONbits.NOSC);
    }

    // Configure PLL prescaler, both PLL postscalers, and PLL feedback divider
    CLKDIVbits.PLLPRE = 1;      // N1=1
    PLLFBDbits.PLLFBDIV = 160;  // M = 160 (ex: FVCO = 1280MHz = 8MHz * 160)
    PLLDIVbits.POST1DIV = 4;    // N2=4 (1280MHz / 4, followed by /2 (N3), followed by fixed / 2 = 80MHz)
    PLLDIVbits.POST2DIV = 2;    // N3=2
    // Initiate Clock Switch to FRC with PLL (NOSC = 0b001)
    __builtin_write_OSCCONH(0x01);
    if(OSCCONbits.COSC != OSCCONbits.NOSC)
    {
        __builtin_write_OSCCONL((OSCCON & 0x7F) | 0x01);    //Assert OSWEN and make sure CLKLOCK is clear, to initiate the switching operation
        // Wait for clock switch to finish
        while(OSCCONbits.COSC != OSCCONbits.NOSC);
    }

    // Program and start the slave core image.
    _program_slave(1,0,synth_slave);
    _start_slave();
}
