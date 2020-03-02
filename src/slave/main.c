#include "system.h"
#include "uart.h"
#include "dac.h"
#include "msi.h"
#include "audio.h"

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
    audio_init();

    printf("Slave initialization complete.\n");

    // Main application loop
    audio_run();

    return 0;
}
