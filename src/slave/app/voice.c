#include "voice.h"

static void start_new_note(voice_t *voice, oscillator_freq_t freq);
static void stop_current_note(voice_t *voice);

void voice_init(
        voice_t *voice,
        envelope_config_t *env_configs,
        oscillator_config_t *osc_configs,
#ifdef TEST
        void *mod_matrix
#else
        int16_t mod_matrix[][VOICE_OPERATORS_PER_VOICE]
#endif
        )
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

/*
 * For each sample, this function is called once for each voice.  Therefore, it
 * has to be VERY highly optimized.  The DSP sections have been written in
 * assembly.  Their C equivalents can be found in the corresponding #ifdef TEST
 * blocks for testing and reference.
 */
voice_output_t voice_update(voice_t *voice)
{
    uint16_t phase_offsets[VOICE_OPERATORS_PER_VOICE];
    int16_t voice_output = 0;
    voice->envelopes_active = 0;

    // Calculate phase offsets for modulation.

#ifdef TEST // Use C implementation for testing.

    // This implementation is functionally equivalent to the assembly below.
    for (uint8_t i = 0; i < VOICE_OPERATORS_PER_VOICE; i ++)
    {
        int32_t phase_offset_accumulator = 0;
        int32_t offset;
        for (uint8_t j = 0; j < VOICE_OPERATORS_PER_VOICE; j ++)
        {
            offset = (int32_t)voice->outputs[j] * voice->mod_matrix[i][j];
            phase_offset_accumulator += offset;
        }

        phase_offsets[i] = phase_offset_accumulator >> 15;
    }

#else // Use asm implementation when building for target.

    __asm__(

            // Multiply and accumulate outputs and mods

            // Clear accumulator A and prefetch first output and mod value.
            "clr        A, [%[out]]+=2, w4, [%[mod]]+=2, w5\n"
            "mac w4*w5, A, [%[out]]+=2, w4, [%[mod]]+=2, w5\n"
            "mac w4*w5, A, [%[out]]+=2, w4, [%[mod]]+=2, w5\n"
            "mac w4*w5, A, [%[out]]-=6, w4, [%[mod]]+=2, w5\n"
            "mac w4*w5, A\n"

            // Start next operator and writeback previous one
            "clr        B, [%[out]]+=2, w4, [%[mod]]+=2, w5, [%[phase_offsets]]+=2\n"
            "mac w4*w5, B, [%[out]]+=2, w4, [%[mod]]+=2, w5\n"
            "mac w4*w5, B, [%[out]]+=2, w4, [%[mod]]+=2, w5\n"
            "mac w4*w5, B, [%[out]]-=6, w4, [%[mod]]+=2, w5\n"
            "mac w4*w5, B\n"

            // Start next operator and writeback previous one
            "clr        A, [%[out]]+=2, w4, [%[mod]]+=2, w5, [%[phase_offsets]]+=2\n"
            "mac w4*w5, A, [%[out]]+=2, w4, [%[mod]]+=2, w5\n"
            "mac w4*w5, A, [%[out]]+=2, w4, [%[mod]]+=2, w5\n"
            "mac w4*w5, A, [%[out]]-=6, w4, [%[mod]]+=2, w5\n"
            "mac w4*w5, A\n"

            // Start next operator and writeback previous one
            "clr        B, [%[out]]+=2, w4, [%[mod]]+=2, w5, [%[phase_offsets]]+=2\n"
            "mac w4*w5, B, [%[out]]+=2, w4, [%[mod]]+=2, w5\n"
            "mac w4*w5, B, [%[out]]+=2, w4, [%[mod]]+=2, w5\n"
            "mac w4*w5, B, [%[out]]-=6, w4, [%[mod]]+=2, w5\n"
            "mac w4*w5, B\n"

            // Store result from last operator
            "sac        B, #0,                               [%[phase_offsets]]\n"

            // Outputs
            :   // None

            // Inputs
            :   // Operators outputs from previous cycle
                [out] "x" (voice->outputs),

                // Modulation levels
                [mod] "y" (voice->mod_matrix),

                // Pointer to output array
                [phase_offsets] "v" (phase_offsets)

            // Clobbers
            :   "A", "B", "w4", "w5"

            );

#endif

    __attribute__((space(xmemory)))
    static envelope_level_t env_levels[VOICE_OPERATORS_PER_VOICE];

    __attribute__((space(ymemory)))
    static oscillator_output_t osc_outputs[VOICE_OPERATORS_PER_VOICE];

    // Calculate envelope outputs (unrolled)
    env_levels[0] = envelope_update(&voice->envelopes[0]) << 5;
    env_levels[1] = envelope_update(&voice->envelopes[1]) << 5;
    env_levels[2] = envelope_update(&voice->envelopes[2]) << 5;
    env_levels[3] = envelope_update(&voice->envelopes[3]) << 5;

    // Calculate oscillator outputs (unrolled)
    osc_outputs[0] = oscillator_update(&voice->oscillators[0], phase_offsets[0]);
    osc_outputs[1] = oscillator_update(&voice->oscillators[1], phase_offsets[1]);
    osc_outputs[2] = oscillator_update(&voice->oscillators[2], phase_offsets[2]);
    osc_outputs[3] = oscillator_update(&voice->oscillators[3], phase_offsets[3]);

    // OR all of the envelope levels together so that this value is only
    // false when all envelope levels are zero.
    voice->envelopes_active = env_levels[0] | env_levels[1] | env_levels[2] | env_levels[3];

    // Calculate operator outputs and final voice output

    // Output levels are in the last row of the modulation matrix.
    int16_t *output_levels = voice->mod_matrix[VOICE_OPERATORS_PER_VOICE];

#ifdef TEST

    // This is functionally equivalent to the assembly below.
    for (uint8_t i = 0; i < VOICE_OPERATORS_PER_VOICE; i ++)
    {
        voice->outputs[i] = ((int32_t)env_levels[i] * osc_outputs[i]) >> 15;
        voice_output += ((int32_t)voice->outputs[i] * output_levels[i]) >> 15;
    }

#else

    __asm__(

            // Copy operator outputs pointer to x prefetch register for later
            // use.
            "mov %[out], w9\n"

            ////////////////////////////////////////////////////////////////////
            // Calculate Operator Outputs
            ////////////////////////////////////////////////////////////////////

            // Multiply oscillator and envelope outputs to calculate operator
            // outputs.

            // Writeback previous results while performing next calculations.
            // Note: The final output is written back when setting up the voice
            // output calculation below.
            "clr        A, [%[env]]+=2, w4, [%[osc]]+=2, w5             \n"
            "mpy w4*w5, A                                               \n"

            "clr        B, [%[env]]+=2, w4, [%[osc]]+=2, w5, [%[out]]+=2\n"
            "mpy w4*w5, B                                               \n"

            "clr        A, [%[env]]+=2, w4, [%[osc]]+=2, w5, [%[out]]+=2\n"
            "mpy w4*w5, A                                               \n"

            "clr        B, [%[env]]+=2, w4, [%[osc]]+=2, w5, [%[out]]+=2\n"
            "mpy w4*w5, B                                               \n"

            ////////////////////////////////////////////////////////////////////
            // Calculate Final Voice Output
            ////////////////////////////////////////////////////////////////////

            // Multiply each output by the corresponding output level value in
            // the mod matrix and sum to calculate voice output.

            // Writeback last operator output while setting up accumulator and
            // prefetches.
            "clr        A, [w9]+=2,     w4, [%[mod]]+=2, w5, [%[out]]+=2\n"
            "mac w4*w5, A, [w9]+=2,     w4, [%[mod]]+=2, w5             \n"
            "mac w4*w5, A, [w9]+=2,     w4, [%[mod]]+=2, w5             \n"
            "mac w4*w5, A, [w9],        w4, [%[mod]],    w5             \n"
            "mac w4*w5, A\n"

            // Store final voice output result in last slot of outputs array.
            "sac        A, #0,                               [%[out]]   \n"

            // Outputs
            :   // None

            // Inputs
            :   // Pointer to envelope outputs array
                [env] "x" (env_levels),

                // Pointer to oscillator outputs array
                [osc] "y" (osc_outputs),

                // Pointer to operator output array
                [out] "v" (voice->outputs),

                // Pointer to output row of mod matrix
                [mod] "y" (output_levels)

            // Clobbers
            :   "A", "B", "w4", "w5", "w9"

            );

    // Final voice output is stored in the last slot in the outputs array for
    // algorithm efficiency.
    voice_output = voice->outputs[VOICE_OPERATORS_PER_VOICE];

#endif

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
