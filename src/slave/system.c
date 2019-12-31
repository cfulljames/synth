#include <xc.h>
#include <libpic30.h>

#include "system.h"

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

    //Clock switch to 200MHz CPU freq (100MIPs) from FRC+PLL (ex: ((8MHz * 200) / 4) / 2 = 200MHz)
    // Configure PLL prescaler, both PLL postscalers, and PLL feedback divider
    CLKDIVbits.PLLPRE = 1;      // N1=1
    PLLFBDbits.PLLFBDIV = 200;  // M = 200 (ex: FVCO = 1600MHz = 8MHz * 200)
    PLLDIVbits.POST1DIV = 4;    // N2=4 (1600MHz / 4, followed by fixed / 2 = 200MHz)
    PLLDIVbits.POST2DIV = 1;    // N3=1
    // Initiate Clock Switch to FRC with PLL (NOSC = 0b001)
    __builtin_write_OSCCONH(0x01);
    if(OSCCONbits.COSC != OSCCONbits.NOSC)
    {
        __builtin_write_OSCCONL((OSCCON & 0x7F) | 0x01);    //Assert OSWEN and make sure CLKLOCK is clear, to initiate the switching operation
        // Wait for clock switch to finish
        while(OSCCONbits.COSC != OSCCONbits.NOSC);
    }

    //Configures AUX PLL for 500MHz output, from FRC input
    //Use FRC at 8MHz for clock input to AUX PLL.
    ACLKCON1 = 0x0101;      //APLLPRE = 1:1 mode, FRC as input

    //Note: If you uncomment the below, the code assumes that an 8MHz crystal is connected and used as the input to the AUX PLL
    //ACLKCON1 = 0x0001;      //APLLPRE = 1:1 mode, primary oscillator as input

    APLLFBD1 = 125;         //8MHz * 125x = 1.00GHz
    APLLDIV1 = 0x0221;      //AFPLLO = 1GHz / 2 / 1 = 500MHz, with AFVCODIV = (1Ghz VCO) / 2 = 500MHz

    ACLKCON1bits.APLLEN = 1;        //Enable the AUX PLL now
}
