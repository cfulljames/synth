#include "msi.h"

void msi_init(void)
{
    // Enable FIFO.
    SI1FIFOCSbits.SRFEN = 1;
}
