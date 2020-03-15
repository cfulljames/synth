#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "voice.h"

/*
 * Initialize the audio engine.
 */
void audio_init(void);


/*
 * Update the audio engine.
 *
 * This should be called in the main application loop.
 */
void audio_update(voice_t *voices, uint8_t num_voices);

#endif
