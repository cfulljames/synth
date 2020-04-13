#!/usr/bin/env python3
"""Combine hex files to be loaded together.

This is used to concatenate the application and bootloader hex files so that the
app and bootloader can be flashed together.

This doesn't do any checking to ensure that the resulting file is valid, it only
joins the text contents of the files, omitting the EOF records.

Usage:
    python3 combine_hex.py file1 [file2 ...] output
"""

import argparse

parser = argparse.ArgumentParser()
parser.add_argument('input_files', nargs='+')
parser.add_argument('output_file')
args = parser.parse_args()

# Read lines from all input files, excluding EOF markers.
output_lines = []
for path in args.input_files:
    with open(path, 'r') as input_file:
        for line in input_file.readlines():
            if line[7:9] == '01':
                # End of file record
                break
            else:
                output_lines.append(line)

# Add EOF record.
output_lines.append(':00000001FF')

# Write output to new file.
with open(args.output_file, 'w') as output_file:
    for line in output_lines:
        output_file.write(line)
