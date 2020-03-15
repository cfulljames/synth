#include "system.h"
#include "uart.h"
#include "dac.h"
#include "msi.h"
#include "synth_ctrl.h"

#include <xc.h>
#include <stdio.h>

int main(void)
{
    // Hardware abstraction layer initialization
    system_init();
    uart_init();
    dac_init();
    msi_init();

    // Application initialization
    synth_ctrl_init();

    printf("Slave initialization complete.\n");

    // Main application loop
    while (1)
    {
        synth_ctrl_update();
    }

    return 0;
}
