#include "gpio.h"
#include "xc.h"

bool gpio_get_button_status(void)
{
    return !PORTEbits.RE7;
}
