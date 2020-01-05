#include "system.h"
#include "uart.h"
#include "dac.h"
#include "synth.h"

#include <xc.h>
#include <stdio.h>

int main(void)
{
    system_init();
    uart_init();
    dac_init();
    synth_init();

    printf("Slave initialization complete.\n");

    synth_run();

    return 0;
}
