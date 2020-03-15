#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "voice.h"

/*
 * Initialize the audio engine.
 */
void audio_init(voice_t *voices, uint16_t num_voices);


/*
 * Update the audio engine.
 *
 * This should be called in the main application loop.
 */
void audio_update(void);

#endif
