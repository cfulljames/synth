/*
 * Master-Slave Interface
 */
#ifndef _MSI_H
#define _MSI_H

#include <stdint.h>
#include <stdbool.h>

#include "common_types.h"

/*
 * Initialize the master-slave interface.
 *
 * This must be called before any of the below functions can be used.
 */
void msi_init(void);

/*
 * Get the next word from the MSI FIFO.
 *
 * If the FIFO is not empty, the next word is stored in data and true is
 * returned.  If it is empty, data is unmodified and false is returned.
 */
INLINE_ON_TARGET bool msi_get_fifo_data(uint16_t *data);

/******************************************************************************
 * Inline Function Definitions
 ******************************************************************************/

// These are only defined when building for the target.  For testing, they are
// mocked out using CMock.
#ifndef TEST

#include <xc.h>

bool msi_get_fifo_data(uint16_t *data)
{
    if (!SI1FIFOCSbits.SRFEMPTY)
    {
        *data = SRMWFDATA;
        return true;
    }
    else
    {
        return false;
    }
}

#endif // TEST

#endif // _MSI_H
