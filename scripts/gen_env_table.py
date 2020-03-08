#!/usr/bin/env python3
"""Generate a log table for use in the envelope rate parameters.

This will generate an assembly file containing the definition of the table.
This will have the name "env_table" and be stored in the program space.

Usage:
    python gen_env_table.py <c file> <h file>
"""
import sys
import math

# Number of entries in the table
ENV_TABLE_LENGTH = 128

MAX_VALUE = 0x0FFFFFFF
MIN_VALUE = 0x00001000

TYPE = 'uint32_t'
NAME = 'env_table'

# Check command line arguments
if len(sys.argv) != 3:
    sys.stderr.write('Incorrect number of arguments.\n')
    sys.exit(1)

values = []

# Calculate table values
for i in range(ENV_TABLE_LENGTH):
    scale = 10
    normalized = (i / ENV_TABLE_LENGTH) * scale
    a = (math.exp(normalized) - 1) / (math.exp(scale) - 1)
    value = (a * (MAX_VALUE - MIN_VALUE)) + MIN_VALUE
    values.append(round(value))

# Write values to file
with open(sys.argv[1], 'w') as c_file:
    c_file.write('#include "{}"\n'.format(sys.argv[2]))
    c_file.write('__attribute__((space(psv)))\n')
    c_file.write('const {} {}[] = {{\n'.format(TYPE, NAME));

    # Convert numbers to two's complement hex and add to file as .word
    for value in values:
        value_bytes = value.to_bytes(4, byteorder='big', signed=True)
        value_hex = value_bytes.hex()
        c_file.write('    0x{},\n'.format(value_hex))

    c_file.write('};\n')

# Generate C header
with open(sys.argv[2], 'w') as c_header:
    c_header.write('#include <stdint.h>\n')
    c_header.write('__attribute__((space(psv)))\n')
    c_header.write('extern const {} {}[];\n'.format(TYPE, NAME))
