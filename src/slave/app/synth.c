#include "synth.h"
#include "dac.h"

// This file is generated using a python script.  It just contains a sine table,
// g_sine_table, and a size macro SINE_TABLE_LENGTH.
#include "sine.h"

void synth_init(void)
{
}

void synth_run(void)
{
    while (1)
    {
        // Wait.
        for (uint16_t i = 0; i < SINE_TABLE_LENGTH; i ++)
        {
            for (volatile uint16_t j = 0; j < 100; j ++);
            dac_set((g_sine_table[i] + 0x8000) >> 4);
        }
    }
}
