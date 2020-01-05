#include "dac.h"

void dac_init(void)
{
    // Enable the global DAC module.
    DACCTRL1Lbits.DACON = 1;

    // Enable DAC 1
    DAC1CONLbits.DACEN = 1;

    // Route DAC 1 voltage to DACOUT
    DAC1CONLbits.DACOEN = 1;
}
