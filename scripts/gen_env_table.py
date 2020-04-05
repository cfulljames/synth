#!/usr/bin/env python3
"""Generate a log table for use in the envelope rate parameters.

This will generate an assembly file containing the definition of the table.
This will have the name "env_table" and be stored in the program space.

Usage:
    python gen_env_table.py <c file> <h file>
"""
import sys
import math
import table_generator

# Number of entries in the table
ENV_TABLE_LENGTH = 128

MAX_VALUE = 0x0FFFFFFF
MIN_VALUE = 0x00001000

VAR_TYPE = '__attribute__((space(psv))) const uint32_t'
VAR_NAME = 'env_table'

# Calculate table values
values = []
for i in range(ENV_TABLE_LENGTH):
    scale = 10
    normalized = (i / ENV_TABLE_LENGTH) * scale
    a = (math.exp(normalized) - 1) / (math.exp(scale) - 1)
    value = (a * (MAX_VALUE - MIN_VALUE)) + MIN_VALUE
    values.append('{:#010x}'.format(round(value)))

table_generator.generate_table(values, VAR_TYPE, VAR_NAME)
