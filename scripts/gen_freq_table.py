#!/usr/bin/env python
"""Generate a MIDI note frequency table.

This will generate a C file containing the definition of the frequency table.
This will have the name "note_freqs" and be stored in the program space.

Usage:
    python gen_freq_table.py <c file> <h file>
"""
import sys
import math

# Number of entries in the table
MIDI_NOTES = 128

# Frequency in Hz of A above middle C
TUNING = 440.0
MIDDLE_A_NOTE_NUM = 69

# Sample rate in samples per second
SAMPLE_RATE = 44101

# Sine table length
SINE_TABLE_LENGTH = 2048

# Check command line arguments
if len(sys.argv) != 3:
    sys.stderr.write('Incorrect number of arguments.\n')
    sys.exit(1)

freq_values = []

for note_num in range(MIDI_NOTES):

    # Calculate note frequency in Hz
    freq_hz = (2 ** ((note_num - MIDDLE_A_NOTE_NUM) / 12)) * TUNING

    # Convert note frequency to sine table entries per frame.
    freq_ipf = freq_hz * (1 / SAMPLE_RATE) * SINE_TABLE_LENGTH

    # Convert to 16.16 fixed point
    freq_ipf_fixed = round(freq_ipf * (1 << 16))

    freq_values.append(freq_ipf_fixed)

# Write values to file
with open(sys.argv[1], 'w') as c_file:
    c_file.write('#include "{}"\n'.format(sys.argv[2]))
    c_file.write('__attribute__((space(psv)))\n')
    c_file.write('const uint32_t note_freqs[] = {\n');

    # Add numbers to array.
    for value in freq_values:
        c_file.write('    {:#010x},\n'.format(value))

    c_file.write('};\n')

# Generate C header
with open(sys.argv[2], 'w') as c_header:
    c_header.write('#include <stdint.h>\n')
    c_header.write('__attribute__((space(psv)))\n')
    c_header.write('extern const uint32_t note_freqs[];\n')
