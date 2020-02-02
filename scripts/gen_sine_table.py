"""Generate a sine table for use in the synth engine.

This will generate an assembly file containing the definition of the sine table.
This will have the label "_sine_table" and be stored in the program space.

Usage:
    python gen_sine_table.py <c file> <h file> <inc file>
"""
import sys
import math

# Number of entries in the table
SINE_TABLE_LENGTH = 2048

# Maximum value for table entries
MAX_VALUE = 0x7FFF

# Check command line arguments
if len(sys.argv) != 4:
    sys.stderr.write('Incorrect number of arguments.\n')
    sys.exit(1)

sine_values = []

# Calculate sine table values
for i in range(SINE_TABLE_LENGTH):
    radians = i * 2 * math.pi / SINE_TABLE_LENGTH
    sine_values.append(int(round(math.sin(radians) * MAX_VALUE)))

# Write values to file
with open(sys.argv[1], 'w') as c_file:
    c_file.write('#include "sine.h"\n')
    c_file.write('__attribute__((space(psv)))\n')
    c_file.write('const int16_t sine_table[] = {\n');

    # Convert numbers to two's complement hex and add to file as .word
    for value in sine_values:
        value_bytes = value.to_bytes(2, byteorder='big', signed=True)
        value_hex = value_bytes.hex()
        c_file.write('    0x{},\n'.format(value_hex))

    c_file.write('};\n')

# Generate C header
with open(sys.argv[2], 'w') as c_header:
    c_header.write('#include <stdint.h>\n')
    c_header.write('__attribute__((space(psv)))\n')
    c_header.write('extern const int16_t sine_table[];\n')

# Generate assembly header
with open(sys.argv[3], 'w') as asm_header:
    asm_header.write('    .extern _sine_table\n')
