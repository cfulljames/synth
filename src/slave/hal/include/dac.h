#ifndef __DAC_H__
#define __DAC_H__

#include <xc.h>

/*
 * Initialize the DAC.
 */
void dac_init(void);

/*
 * Set the DAC output voltage.
 *
 * Only the lowest 12 bits are used.
 */
static inline void dac_set(uint16_t val)
{
    DAC1DATH = val;
}

#endif
