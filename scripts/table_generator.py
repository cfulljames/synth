#!/usr/bin/env python3
"""
Data table generator.

This module is used by the table generator scripts to generate the C and
assembly source code for the table data.
"""

import sys
import argparse

def generate_table(arr_values, arr_type, arr_name):
    """Generate the data table source files.

    This will run argparse to get the command line arguments that specify the
    source file paths.

    The general command line structure is:
        python3 <script_name> <c file> <h file> [<inc file>]

    Params:
        arr_values: List of strings to be written in the body of the array.
        arr_type: The C variable type of the elements in the array.
        arr_name: The name of the C array.
    """
    args = _parse_args()

    # Write values to C file
    with open(args.cfile, 'w') as c_file:
        c_file.write('#include "{}"\n'.format(args.hfile))
        c_file.write('{} {}[] = {{\n'.format(arr_type, arr_name));

        # Convert numbers to two's complement hex and add to file as .word
        for value in arr_values:
            c_file.write('    {},\n'.format(value))

        c_file.write('};\n')

    # Generate C header
    with open(args.hfile, 'w') as c_header:
        c_header.write('#include <stdint.h>\n')
        c_header.write('extern {} {}[];\n'.format(arr_type, arr_name))

    # Generate assembly header (optional)
    if args.incfile:
        with open(args.incfile, 'w') as asm_header:
            asm_header.write('    .extern _sine_table\n')

def _parse_args():
    """Parse the file paths from the command line arguments."""
    parser = argparse.ArgumentParser()
    parser.add_argument('cfile')
    parser.add_argument('hfile')
    parser.add_argument('incfile', nargs='?')
    return parser.parse_args()

