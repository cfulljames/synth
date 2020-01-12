#ifndef __SYNTH_H__
#define __SYNTH_H__

/*
 * Initialize the synthesis engine.
 */
void synth_init(void);

/*
 * Run the main synthesis loop.
 *
 * This function does not return.
 */
void synth_run(void);

/*
 * This flag should be set from the DAC interrupt when the current sample has
 * been written to the output.
 */
extern volatile uint8_t synth_dac_ready;

/*
 * The current sample to be written to the DAC.
 */
extern uint16_t synth_sample;

#endif
