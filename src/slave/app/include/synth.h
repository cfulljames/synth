#ifndef __SYNTH_H__
#define __SYNTH_H__

/*
 * Initialize the synthesis engine.
 */
void synth_init(void);


/*
 * Start the synthesis engine loop.
 *
 * This function does not return.
 */
void synth_run(void);

#endif
