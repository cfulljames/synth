/*
 * Synthesis Engine Controller
 *
 * The synth controller is responsible for driving the main DSP application loop
 * and updating the synthesis engine whenever commands are received.
 */
#ifndef _SYNTH_CTRL_H
#define _SYNTH_CTRL_H

// The number of voices in the synthesis engine.
#define SYNTH_CTRL_NUM_VOICES (6)

// The number of commands to store in the buffer.
#define SYNTH_CTRL_BUFFER_SIZE (64)

/*
 * Initialize the synthesis controller.
 *
 * This must be called before using the update function.
 */
void synth_ctrl_init(void);

/*
 * Update the synthesis controller.
 *
 * This should be called in the main application loop.
 */
void synth_ctrl_update(void);

#endif // _SYNTH_CTRL_H
