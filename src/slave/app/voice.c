#include "voice.h"

static void start_new_note(voice_t *voice, oscillator_freq_t freq);
static void stop_current_note(voice_t *voice);

void voice_init(
        voice_t *voice,
        envelope_config_t *env_configs,
        oscillator_config_t *osc_configs,
        int16_t mod_matrix[][VOICE_OPERATORS_PER_VOICE])
{
    for (uint8_t i = 0; i < VOICE_OPERATORS_PER_VOICE; i ++)
    {
        envelope_init(&voice->envelopes[i], &env_configs[i]);
        oscillator_init(&voice->oscillators[i], &osc_configs[i]);
        voice->outputs[i] = 0;
    }
    voice->mod_matrix = mod_matrix;
    voice->transition_note_freq = 0;
    voice->envelopes_active = 0;
}

void voice_start(voice_t *voice, oscillator_freq_t freq)
{
    if (0 == voice->envelopes_active)
    {
        // Operators are all currently silenced.
        start_new_note(voice, freq);
    }
    else
    {
        // At least one operator actively playing; close gates to avoid making
        // a pop when switching notes.
        stop_current_note(voice);
        voice->transition_note_freq = freq;
    }
}

voice_output_t voice_update(voice_t *voice)
{
    int16_t voice_output = 0;
    voice->envelopes_active = 0;
    for (uint8_t i = 0; i < VOICE_OPERATORS_PER_VOICE; i ++)
    {

#ifdef TEST // Use C implementation for testing.

        int32_t phase_offset_accumulator = 0;
        int32_t offset;
        for (uint8_t j = 0; j < VOICE_OPERATORS_PER_VOICE; j ++)
        {
            offset = (int32_t)voice->outputs[j] * voice->mod_matrix[i][j];
            phase_offset_accumulator += offset;
        }

        int16_t phase_offset = phase_offset_accumulator >> 15;

#else // Use asm implementation when building for target.

        int16_t phase_offset;
        int16_t *mod = voice->mod_matrix[i];

        __asm__(
                // Clear accumulator A and prefetch first output and mod value.
                "clr A, [%[out]]+=2, w4, [%[mod]]+=2, w5\n"

                // Multiply and accumulate outputs and mods
                "mac w4*w5, A, [%[out]]+=2, w4, [%[mod]]+=2, w5\n"
                "mac w4*w5, A, [%[out]]+=2, w4, [%[mod]]+=2, w5\n"
                "mac w4*w5, A\n"

                // Store result in phase_offset
                "sac A, #0, %[phase_offset]\n"

                // Outputs
                : [phase_offset] "=r" (phase_offset)

                // Inputs
                : [out] "x" (voice->outputs), [mod] "y" (mod)

                // Clobbers
                : "A", "w4", "w5"
           );

#endif

        envelope_level_t level = envelope_update(&voice->envelopes[i]) << 5;
        oscillator_output_t out = oscillator_update(&voice->oscillators[i], phase_offset);
        voice->outputs[i] = ((int32_t)level * out) >> 15;
        voice_output += ((int32_t)voice->outputs[i] * voice->mod_matrix[3][i]) >> 15;

        // OR all of the envelope levels together so that this value is only
        // false when all envelope levels are zero.
        voice->envelopes_active |= level;
    }

    if (voice->transition_note_freq && !voice->envelopes_active)
    {
        // All operators silenced, ready to move to new note.
        start_new_note(voice, voice->transition_note_freq);
        voice->transition_note_freq = 0;
    }

    return voice_output;
}

void voice_release(voice_t *voice)
{
    for (uint8_t i = 0; i < VOICE_OPERATORS_PER_VOICE; i ++)
    {
        envelope_close_gate(&voice->envelopes[i]);
    }
}

void voice_stop(voice_t *voice)
{
    stop_current_note(voice);
}

__attribute__((always_inline))
static void stop_current_note(voice_t *voice)
{
    for (uint8_t i = 0; i < VOICE_OPERATORS_PER_VOICE; i ++)
    {
        envelope_close_gate_fast(&voice->envelopes[i]);
    }
}

__attribute__((always_inline))
static void start_new_note(voice_t *voice, oscillator_freq_t freq)
{
    for (uint8_t i = 0; i < VOICE_OPERATORS_PER_VOICE; i ++)
    {
        envelope_open_gate(&voice->envelopes[i]);
        oscillator_set_frequency(&voice->oscillators[i], freq);
        oscillator_reset_phase(&voice->oscillators[i]);
    }
}
