"""Generate the MDB flash script.

MDB always runs from the users home directory (at least on macOS), so giving it
the correct path names is a pain.  This script generates a command file that can
be run in MDB to flash the program and start it, leaving MDB running.

Usage:
    python gen_mdb_flash.py <elf file> <command file>
"""

import os
import sys

if len(sys.argv) != 3:
    sys.stderr.write('Error: Invalid file name(s).')
    sys.exit(1)

DEVICE = 'dspic33ch128mp508'
HWTOOL = 'sk'
ELF_FILE = sys.argv[1]
PROG_PATH = os.path.abspath(ELF_FILE)

SCRIPT_CONTENTS = '''\
echo MDB Started
device {dev}
hwtool {hwtool}
program {path}
echo Running Program...
run
'''.format(dev=DEVICE, hwtool=HWTOOL, path=PROG_PATH)

with open(sys.argv[2], 'w') as cmdfile:
    cmdfile.write(SCRIPT_CONTENTS)
