"""Generate a sine table for use in the synth engine.

This will generate a C header file containing a single array (called
g_sine_table) and a macro (SINE_TABLE_LENGTH).  The values in the table are
normalized to the range INT16_MIN to INT16_MAX.

Usage:
    python gen_sine_table.py <output file>
"""
import sys
import math

# Number of entries in the table
SINE_TABLE_LENGTH = 2048

# Maximum value for table entries
MAX_VALUE = 0x7FFF

# Check command line arguments
if len(sys.argv) != 2:
    sys.stderr.write('Incorrect number of arguments.\n')
    sys.exit(1)

sine_values = []

# Calculate sine table values
for i in range(SINE_TABLE_LENGTH):
    radians = i * 2 * math.pi / SINE_TABLE_LENGTH
    sine_values.append(round(math.sin(radians) * MAX_VALUE))

# Write values to file
with open(sys.argv[1], 'w') as output_file:
    output_file.write('#ifndef __SINE_TABLE_H__\n')
    output_file.write('#define __SINE_TABLE_H__\n')
    output_file.write(
        '#define SINE_TABLE_LENGTH {}\n'.format(SINE_TABLE_LENGTH))
    output_file.write('const int16_t g_sine_table[] = {\n')

    for value in sine_values:
        output_file.write('    {},\n'.format(value))

    output_file.write('};\n')
    output_file.write('#endif')
