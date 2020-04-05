#!/usr/bin/env python
"""Generate a MIDI note frequency table.

This will generate a C file containing the definition of the frequency table.
This will have the name "note_freqs" and be stored in the program space.

Usage:
    python gen_freq_table.py <c file> <h file>
"""
import sys
import math
import table_generator

# Number of entries in the table
MIDI_NOTES = 128

# Frequency in Hz of A above middle C
TUNING = 440.0
MIDDLE_A_NOTE_NUM = 69

# Number of notes in an octave (traditional western scale)
NOTES_PER_OCTAVE = 12

# Note frequency doubles with each octave
OCTAVE_SCALAR = 2

# Sample rate in samples per second
SAMPLE_RATE = 44101

# Sine table length
SINE_TABLE_LENGTH = 2048

# Variable type for C data structure
VAR_TYPE = '__attribute__((section(".text"))) const uint32_t'

# Name of the array in C
VAR_NAME = 'note_freqs'

freq_values = []
for note_num in range(MIDI_NOTES):

    # Calculate note frequency in Hz
    freq_ratio = (note_num - MIDDLE_A_NOTE_NUM) / NOTES_PER_OCTAVE
    freq_hz = (OCTAVE_SCALAR ** freq_ratio) * TUNING

    # Convert note frequency to sine table entries per frame.
    freq_ipf = freq_hz * (1 / SAMPLE_RATE) * SINE_TABLE_LENGTH

    # Convert to 16.16 fixed point
    freq_ipf_fixed = round(freq_ipf * (1 << 16))

    freq_values.append('{:#010x}'.format(freq_ipf_fixed))

table_generator.generate_table(freq_values, VAR_TYPE, VAR_NAME)
