#include "system.h"
#include "uart.h"
#include "dac.h"
#include "audio.h"
#include "synth.h"

#include <xc.h>
#include <stdio.h>

int main(void)
{
    system_init();
    uart_init();
    dac_init();
    audio_init();
    synth_init();

    // Enable FIFO
    SI1FIFOCSbits.SRFEN = 1;

    printf("Slave initialization complete.\n");

    audio_run();

    return 0;
}
