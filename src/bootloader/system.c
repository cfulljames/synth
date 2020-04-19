#include "system.h"

#include "xc.h"

// Output Pin Multiplexer Assignment Values
#define PINMUX_OUT_UART1_TX 0x01

void system_init(void)
{
    // UART TX Pin
    _RP59R = PINMUX_OUT_UART1_TX;

    // Assign UART RX to RP58: Pin C10
    _U1RXR = 58;
    TRISCbits.TRISC10 = 1;
}

void system_deinit(void)
{
    _RP59R = 0;
    _U1RXR = 0;
    TRISCbits.TRISC10 = 0;
}
