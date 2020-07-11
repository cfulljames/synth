#!/usr/bin/env python3
import bootloader_utils
import os
import binascii

def main():
    blser = open_serial_port()
    SCRIPT_DIR = os.path.dirname(__file__)
    HEX_PATH = os.path.join(
        SCRIPT_DIR, '..', 'build', 'src', 'master', 'synth_master.hex')
    code = bootloader_utils.ApplicationCode(HEX_PATH)

    # Erase pages
    print('Erasing flash', end='')
    for page in code.pages:
        print('.', end='', flush=True)
        blser.send_erase(start=page, end=page+0x800)

    # Write/verify rows
    print('\nWriting application', end='')
    for row in code.rows:
        print('.', end='', flush=True)
        blser.send_write_row(
            start=row.address, data=row.data)

        # Replace MSB of each word with 0
        for index in range(len(row.data)):
            if index % 4 == 3:
                row.data[index] = 0
        crc = binascii.crc32(row.data)

        blser.send_verify(
            start=row.address, end=row.address+0x100, crc=crc)

    # Run application
    blser.send_run()

    print('\nWrite Success!')

def open_serial_port():
    if not 'SYNTH_SERIAL_PORT' in os.environ:
        raise KeyError('Environment variable SYNTH_SERIAL_PORT not set.')
    ser =  bootloader_utils.BootloaderSerial(os.environ['SYNTH_SERIAL_PORT'])
    return ser

if __name__ == '__main__':
    main()
