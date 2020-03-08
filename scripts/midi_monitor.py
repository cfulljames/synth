#!/usr/bin/env python3
"""MIDI Passthrough and Serial Monitoring Tool

This tool allows serial monitoring from the synth while also passing MIDI data
from connected devices to the synth over the UART connection.  A QuNeo can be
used to control the parameters with full support for LED feedback.

Usage:

    Find available MIDI device ports:
        midi_monitor.py --list-ports

    Run the monitor:
        midi_monitor.py <serial_port> <keyboard_port> <quneo_port>

    where:
        serial_port is the name of the serial device, e.g. COM1 or /dev/tty3.
        keyboard_port is the number of the midi port for the keyboard.  You can
            find the port number for your device by using the --list-ports flag.
        quneo_port is the number of the midi port for the QuNeo.  You can find
            the port number using --list-ports.
"""
import rtmidi
import sys
import argparse
import serial

###############################################################################
# QuNeo LED MIDI Definitions
###############################################################################

# Default channel: all controls, pads in drum mode
MIDI_DEFAULT_CHANNEL = 0
MIDI_H_SLIDER_CCS = [8, 9, 10, 11]
MIDI_V_SLIDER_CCS = [1, 2,  3,  4]
MIDI_LONG_SIDER_CC = 5
MIDI_PAD_DRUM_NOTES = [x * 2 for x in range(16)]
MIDI_PAD_DRUM_NOTES_GREEN = [x * 2 for x in range(16)]
MIDI_PAD_DRUM_NOTES_RED = [(x * 2) + 1 for x in range(16)]
MIDI_ROTARY_CCS = [6, 7]
MIDI_RHOMBUS_NOTE = 44
MIDI_RHOMBUS_NOTE_RED = 44
MIDI_RHOMBUS_NOTE_GREEN = 45
MIDI_DOWN_ARROW_NOTES = [47, 49]
MIDI_UP_ARROW_NOTES = [46, 48]
MIDI_LEFT_ARROW_NOTES = [42, 40, 38, 36]
MIDI_RIGHT_ARROW_NOTES = [43, 41, 39, 37]
MIDI_DIAMOND = 33
MIDI_SQUARE = 34
MIDI_TRIANGLE = 35

# Alternate channel: pads only, pads in corner mode
MIDI_ALT_CHANNEL = 1
MIDI_PAD_CORNER_NOTES_GREEN = [x * 2 for x in range(64)]
MIDI_PAD_CORNER_NOTES_RED = [(x * 2) + 1 for x in range(64)]

###############################################################################
# MIDI Protocol Constants
###############################################################################

MIDI_NOTE_ON = 0x90
MIDI_NOTE_OFF = 0x80
MIDI_CC = 0xB0
CHANNEL_MASK = 0x0F
STATUS_MASK = 0xF0

def main():
    args = parse_command_line()

    if args.list_ports:
        # List available MIDI device ports.
        midi = rtmidi.MidiIn()
        midi_ports = midi.get_ports()
        print('MIDI Ports:')
        for index, port in enumerate(midi_ports):
            print('    {:2d}: {}'.format(index, port))
        sys.exit(0)

    # Start the UART serial connection.
    ser = serial.Serial(args.serial_port, baudrate=115200)

    midi = MidiManager(ser, args.keyboard_midi_port, args.quneo_midi_port)

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
    parser.add_argument('keyboard_midi_port', nargs='?', type=int)
    parser.add_argument('quneo_midi_port', nargs='?', type=int)

    return parser.parse_args()

class MidiManager():
    def __init__(self, ser, keyboard_midi_port, quneo_midi_port):
        self.ser = ser
        self.key_midi = rtmidi.MidiIn()
        self.quneo_midi_in = rtmidi.MidiIn()
        self.quneo_midi_out = rtmidi.MidiOut()
        self.visible_op = -1
        self.visible_mod = -1

        self.operators = []
        for i in range(4):
            self.operators.append(OperatorLEDManager(i, self.quneo_midi_out))

        # Start the keyboard MIDI port.
        self.key_midi.set_callback(self.on_keyboard_midi)
        self.key_midi.open_port(keyboard_midi_port)

        # Start the QuNeo MIDI port.
        self.quneo_midi_in.set_callback(self.on_quneo_midi)
        self.quneo_midi_in.open_port(quneo_midi_port)
        self.quneo_midi_out.open_port(quneo_midi_port)

    def on_keyboard_midi(self, midi, _):
        """Callback for received MIDI events."""
        data, time = midi
        self.send_to_synth(data)

    def on_quneo_midi(self, midi, _):
        """Callback for received MIDI events."""
        data, time = midi
        status = data[0] & STATUS_MASK
        channel = data[0] & CHANNEL_MASK
        if status == MIDI_NOTE_ON:
            note, velocity = data[1:]
            if note in MIDI_PAD_DRUM_NOTES:
                self.on_pad_pressed(note)

            elif note in MIDI_DOWN_ARROW_NOTES:
                self.on_down_pressed(note)

        elif status == MIDI_NOTE_OFF:
            # Do nothing.
            pass

        elif status == MIDI_CC:
            cc, value = data[1:]
            if cc == MIDI_LONG_SIDER_CC:
                self.on_long_slider_move(value)

            elif cc in MIDI_H_SLIDER_CCS:
                self.on_hslider_move(cc, value)

            elif cc in MIDI_V_SLIDER_CCS:
                self.on_vslider_move(cc, value)

    def on_long_slider_move(self, value):
        if self.visible_op < 0 or self.visible_mod < 0:
            return

        op = self.operators[self.visible_op]
        op.modlevels[self.visible_mod] = value
        op.update_leds(self.visible_mod)

        # Send message to synth.
        channel = (4 * self.visible_op) + self.visible_mod
        message = [MIDI_CC | channel, MIDI_LONG_SIDER_CC, value]
        self.send_to_synth(message)

    def on_hslider_move(self, cc, value):
        if self.visible_op < 0 or self.visible_mod < 0:
            return

        op = self.operators[self.visible_op]
        index = MIDI_H_SLIDER_CCS.index(cc)
        op.hsliders[index] = value
        op.update_leds(self.visible_mod)

        # Send message to synth.
        channel = self.visible_op
        message = [MIDI_CC | channel, cc, value]
        self.send_to_synth(message)

    def on_vslider_move(self, cc, value):
        if self.visible_op < 0 or self.visible_mod < 0:
            return

        op = self.operators[self.visible_op]
        index = MIDI_V_SLIDER_CCS.index(cc)
        op.vsliders[index] = value
        op.update_leds(self.visible_mod)

        # Send message to synth.
        channel = self.visible_op
        message = [MIDI_CC | channel, cc, value]
        self.send_to_synth(message)

    def on_pad_pressed(self, note):
        pad = int(note / 2)
        x = pad % 4
        y = int(pad / 4)

        self.visible_op = y
        self.visible_mod = x

        operator = y
        self.visible_mod = x
        for op in range(4):
            if op == y:
                self.operators[y].update_leds(x)
            else:
                self.operators[op].update_leds(-1)

    def on_down_pressed(self, note):
        if self.visible_op < 0 or self.visible_mod < 0:
            return
        op = self.operators[self.visible_op]
        op.modlevels[self.visible_mod] = 0
        op.update_leds(self.visible_mod)
        channel = (4 * self.visible_op) + self.visible_mod
        message = [MIDI_CC | channel, MIDI_LONG_SIDER_CC, 0]
        self.send_to_synth(message)

    def send_to_synth(self, data):
        # Write MIDI event to synth over UART.
        self.ser.write(bytes(data))

class OperatorLEDManager():
    def __init__(self, num, midiout):
        self.num = num
        self.midiout = midiout
        self.hsliders = [0, 0, 0, 0]
        self.vsliders = [0, 0, 0, 0]
        self.modlevels = [0, 0, 0, 0]

    def update_leds(self, visible_mod):

        status = MIDI_NOTE_ON | MIDI_DEFAULT_CHANNEL
        # Clear all pad LEDs for this operator.
        for pad in range(4):
            abspad = pad + (self.num * 4)
            green_pad = MIDI_PAD_DRUM_NOTES_GREEN[abspad]
            red_pad = MIDI_PAD_DRUM_NOTES_RED[abspad]
            green_msg = [status, green_pad, 0]
            red_msg = [status, red_pad, 0]
            self.midiout.send_message(green_msg)
            self.midiout.send_message(red_msg)

        if visible_mod >= 0:
            # This operator is visible
            midi_maps = [
                (self.hsliders, MIDI_H_SLIDER_CCS),
                (self.vsliders, MIDI_V_SLIDER_CCS),
            ]
            for values, ccs in midi_maps:
                for v, cc in zip(values, ccs):
                    message = [MIDI_CC, cc, v]
                    self.midiout.send_message(message)

            # Activate all green leds on current pad to indicate selection.
            currentpad = self.num * 4 + visible_mod
            green_note = MIDI_PAD_DRUM_NOTES_GREEN[currentpad]
            level = 127
            message = [status, green_note, level]
            self.midiout.send_message(message)

            # Set long slider position for visible mod.
            modvalue = self.modlevels[visible_mod]
            modcc = MIDI_LONG_SIDER_CC
            message = [MIDI_CC, modcc, modvalue]
            self.midiout.send_message(message)

        # Highlight corners of all mod pads.
        corners = [self.num * 16 + (2 * x) for x in range(4)]
        mod_pads_green = [MIDI_PAD_CORNER_NOTES_GREEN[x] for x in corners]
        mod_pads_red = [MIDI_PAD_CORNER_NOTES_RED[x] for x in corners]
        for value, green_note, red_note in zip(
                self.modlevels, mod_pads_green, mod_pads_red):

            if value > 0:
                status = MIDI_NOTE_ON
            else:
                status = MIDI_NOTE_OFF
            red_val, green_val = self.get_red_green_for_value(value)

            status |= MIDI_ALT_CHANNEL
            green_msg = [status, green_note, green_val]
            red_msg = [status, red_note, red_val]

            self.midiout.send_message(green_msg)
            self.midiout.send_message(red_msg)

    def get_red_green_for_value(self, value):

        if value < 64:
            green_val = value + 64
            red_val = 0
        else:
            green_val = 2 * (127 - value)
            red_val = value / 2

        return red_val, green_val

if __name__ == '__main__':
    main()
