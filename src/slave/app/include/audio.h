#ifndef __AUDIO_H__
#define __AUDIO_H__

/*
 * Initialize the audio engine.
 */
void audio_init(void);


/*
 * Start the audio engine loop.
 *
 * This function does not return.
 */
void audio_run(void);

#endif
