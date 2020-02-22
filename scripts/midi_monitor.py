#!/usr/bin/env python3
"""MIDI Passthrough and Serial Monitoring Tool

This tool allows serial monitoring from the synth while also passing MIDI data
from a connected device to the synth over the UART connection.

Usage:

    Find available MIDI device ports:
        midi_monitor.py --list-ports

    Run the monitor:
        midi_monitor.py <serial_port> <midi_port>

    where:
        serial_port is the name of the serial device, e.g. COM1 or /dev/tty3.
        midi_port is the number of the midi port.  You can find the port number
            for your device by using the --list-ports flag.
"""
import rtmidi
import sys
import argparse
import serial

def main():
    args = parse_command_line()

    midiin = rtmidi.MidiIn()

    if args.list_ports:
        # List available MIDI device ports.
        midi_ports = midiin.get_ports()
        print('MIDI Ports:')
        for index, port in enumerate(midi_ports):
            print('    {:2d}: {}'.format(index, port))
        sys.exit(0)

    # Start the UART serial connection.
    ser = serial.Serial(args.serial_port, baudrate=115200)

    # Start the MIDI port.
    midiin.set_callback(on_midi_received, ser)
    midiin.open_port(args.midi_port)

    # Main loop.
    while True:
        try:
            # Print received data from UART to stdout.
            sys.stdout.write(ser.read().decode('utf-8'))
        except KeyboardInterrupt:
            print('Exiting.')
            sys.exit(0)
        except UnicodeDecodeError:
            pass

def parse_command_line():
    """Parse the command line arguments."""
    parser = argparse.ArgumentParser()
    parser.add_argument('--list-ports', '-l', action='store_true',
                        help='list available MIDI ports and exit')
    parser.add_argument('serial_port', nargs='?')
    parser.add_argument('midi_port', nargs='?', type=int)

    return parser.parse_args()

def on_midi_received(midi, ser):
    """Callback for received MIDI events."""
    data, time = midi
    for byte in data:
        # Write MIDI event to synth over UART.
        ser.write(bytes([byte]))

if __name__ == '__main__':
    main()
