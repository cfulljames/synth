/*
 * Synthesizer Voice
 *
 * The voice synthesizes the audio for a single note.  Multiple voices can be
 * used simultaneously for polyphonic operation (each voice is used for a
 * separate note), or for a "unison" mode (each voice plays the same note but
 * with slight pitch offsets).
 */
#ifndef _VOICE_H
#define _VOICE_H

#include "envelope.h"
#include "oscillator.h"

#include <stdint.h>

// The number of operators used by each voice.
#define VOICE_OPERATORS_PER_VOICE (3)

// Type of the samples returned by the update function.
typedef int16_t voice_output_t;

// Internal state of the voice.  Do not modify these directly; use the functions
// below.
typedef struct voice_s {

    // Envelope instances for each operator.
    envelope_t envelopes[VOICE_OPERATORS_PER_VOICE];

    // Oscillator instances for each operator.
    oscillator_t oscillators[VOICE_OPERATORS_PER_VOICE];

    // Modulation matrix.  Each row represents the input to an operator from all
    // three operators.
    int16_t (*mod_matrix)[VOICE_OPERATORS_PER_VOICE];

    // Outputs from the previous update, used to calculate modulation inputs on
    // the next update.
    oscillator_output_t outputs[VOICE_OPERATORS_PER_VOICE];

    // When a note is started before the previous one had stopped, this holds
    // the new note frequency until the previous note finishes.
    oscillator_freq_t transition_note_freq;

    // Boolean value indicating whether any of the envelope outputs were still
    // active at the last update.
    envelope_level_t envelopes_active;

} voice_t;

/*
 * Initialize a voice.
 *
 * This should be called before using the voice structure with any of the
 * functions below.  Calling it on an already-initialized voice will
 * re-initialize it, but it may cause an audible pop.  Use the stop function
 * instead to avoid this.
 */
void voice_init(
        voice_t *voice,
        envelope_config_t *env_configs,
        oscillator_config_t *osc_configs,
        int16_t mod_matrix[][VOICE_OPERATORS_PER_VOICE]);

/*
 * Start playing a note with this voice.
 *
 * freq specifies the base frequency for the note.  If this voice is
 * already actively playing, the envelopes will be used to stop it before
 * starting switching to the new note.  This should be called after a MIDI Note
 * On event, for example.
 */
void voice_start(voice_t *voice, oscillator_freq_t freq);

/*
 * Release the current note.
 *
 * This will close the envelope gates for each operator, putting them into the
 * release phase.  This should be called after a MIDI Note Off event, for
 * example.
 */
void voice_release(voice_t *voice);

/*
 * Stop the current note.
 *
 * This will use the envelope fast release to stop the current note quickly but
 * without popping.  Useful for killing all notes after a "MIDI Panic" or
 * similar.
 */
void voice_stop(voice_t *voice);

/*
 * Update the voice and get the current output.
 *
 * This should be called periodically to get the output signal of the voice.
 * This will update the envelope and oscillator for each operator in the voice
 * and calculate the output.
 */
voice_output_t voice_update(voice_t *voice);

#endif // _VOICE_H
