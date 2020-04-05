#!/usr/bin/env python3
"""Generate a sine table for use in the synth engine.

This will generate an assembly file containing the definition of the sine table.
This will have the name "sine_table" and be stored in the program space.

Usage:
    python gen_sine_table.py <c file> <h file> <inc file>
"""
import sys
import math
import table_generator

# Number of entries in the table
SINE_TABLE_LENGTH = 2048

# Maximum value for table entries
MAX_VALUE = 0x7FFF

# Variable type for C data structure
VAR_TYPE = '__attribute__((space(psv))) const int16_t'

# Name of the array in C
VAR_NAME = 'sine_table'

# Calculate sine table values
sine_values = []
for i in range(SINE_TABLE_LENGTH):
    radians = i * 2 * math.pi / SINE_TABLE_LENGTH
    value = int(round(math.sin(radians) * MAX_VALUE))
    sine_values.append('{:#06x}'.format(value))

table_generator.generate_table(sine_values, VAR_TYPE, VAR_NAME)
