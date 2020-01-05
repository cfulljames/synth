#include "system.h"
#include "uart.h"
#include "dac.h"

#include <xc.h>
#include <stdio.h>

int main(void)
{
    system_init();
    uart_init();
    dac_init();

    printf("Slave initialization complete.\n");

    while (1)
    {
        // Wait.
        for (uint16_t i = 0; i < 0x0800; i ++)
        {
            for (volatile uint16_t j = 0; j < 100; j ++);
            dac_set(i);
        }
    }

    return 0;
}
