#!/usr/bin/env python3
import enum
import os
import time
import struct
import unittest
import bootloader_utils

class MessageType(enum.IntEnum):
    RESPONSE = 0x00
    ERASE = 0x01
    WRITE = 0x02
    VERIFY = 0x03
    RUN = 0x04
    REQUEST_DEVICE_INFO = 0x05
    DEVICE_INFO = 0x06

class ResponseCode(enum.IntEnum):

    OK = 0x00

    # Serial Protocol Errors
    TIMEOUT = 0x01
    PACKET_CRC_FAIL = 0x02
    COBS_DECODE_FAIL = 0x03
    PACKET_TOO_LONG = 0x04
    PACKET_TOO_SHORT = 0x05

    # Message-Specific Errors
    INVALID_TYPE = 0x10
    DATA_TOO_LONG = 0x11
    DATA_TOO_SHORT = 0x12
    BAD_ALIGNMENT = 0x13
    ADDR_OUT_OF_RANGE = 0x14

    # General Errors
    INTERNAL_ERROR = 0xFF

def open_serial_port():
    if not 'SYNTH_SERIAL_PORT' in os.environ:
        raise KeyError('Environment variable SYNTH_SERIAL_PORT not set.')
    ser =  bootloader_utils.BootloaderSerial(os.environ['SYNTH_SERIAL_PORT'])
    return ser

def pack_msg(msg_type, data):
    return msg_type.to_bytes(1, byteorder='big') + data

def pack_response_msg(code):
    return pack_msg(MessageType.RESPONSE, code.to_bytes(1, byteorder='big'))

class BootloaderTestCase(unittest.TestCase):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.blser = None

    def setUp(self):
        self.blser = open_serial_port()

    def tearDown(self):
        self.blser.close()

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.addTypeEqualityFunc(bytes, self.assertBytesEqual)
        self.addTypeEqualityFunc(bytearray, self.assertBytesEqual)

    def assertBytesEqual(self, a, b, msg=None):
        if a == b:
            return

        standardMsg = '{} != {}'.format(a.hex(), b.hex())
        self.fail(self._formatMessage(msg, standardMsg))

class TestSerialErrors(BootloaderTestCase):

    def test_packet_with_invalid_crc(self):
        send = bytes.fromhex('01 ff ff ff ff')
        send = bootloader_utils.cobs_encode(send) + b'\0'
        self.blser.write(send)

        expected = pack_response_msg(ResponseCode.PACKET_CRC_FAIL)
        self.assertEqual(expected, self.blser.read_packet())

    def test_packet_with_invalid_cobs(self):
        send = bytes.fromhex('01 ff ff ff ff 00')
        self.blser.write(send)

        expected = pack_response_msg(ResponseCode.COBS_DECODE_FAIL)
        self.assertEqual(expected, self.blser.read_packet())

    def test_packet_timeout(self):
        send = bytes.fromhex('05 ff ff ff ff')
        self.blser.write(send)

        # No data should be received before the 50 ms timeout.
        with self.assertRaises(TimeoutError):
            self.blser.read_packet(timeout=0.040)

        # Write some more bytes to reset timeout
        self.blser.write(send)

        # No data should be received before the 50 ms timeout.
        with self.assertRaises(TimeoutError):
            self.blser.read_packet(timeout=0.040)

        # Should receive timeout error packet immediately after 50 ms has
        # elapsed.
        expected = pack_response_msg(ResponseCode.TIMEOUT)
        self.assertEqual(expected, self.blser.read_packet(timeout=0.020))

    def test_packet_too_short(self):
        MIN_PACKET_LEN = 5
        for packet_len in range(1, MIN_PACKET_LEN):
            send = (b'\01' * packet_len) + b'\x00'
            self.blser.write(send)

            expected = pack_response_msg(ResponseCode.PACKET_TOO_SHORT)
            self.assertEqual(expected, self.blser.read_packet())

    def test_packet_too_long(self):
        MAX_DATA_LENGTH = 2048
        packet_data = b'\xcc' * (MAX_DATA_LENGTH + 1)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.PACKET_TOO_LONG)
        self.assertEqual(expected, self.blser.read_packet())

class TestMessageErrors(BootloaderTestCase):

    def test_invalid_message_type(self):
        send = b'\xFF'
        self.blser.write_packet(send)

        expected = pack_response_msg(ResponseCode.INVALID_TYPE)
        self.assertEqual(expected, self.blser.read_packet())

class TestDeviceInfoMessage(BootloaderTestCase):

    def test_device_info_extra_bytes_in_request(self):
        request = bytes([MessageType.REQUEST_DEVICE_INFO, 0xAB])
        self.blser.write_packet(request)

        expected = pack_response_msg(ResponseCode.DATA_TOO_LONG)
        self.assertEqual(expected, self.blser.read_packet())

    def test_valid_request(self):
        request = bytes([MessageType.REQUEST_DEVICE_INFO])
        self.blser.write_packet(request)

        response = self.blser.read_packet()
        self.assertEqual(6, response[0])
        self.assertEqual(24, len(response))

class TestEraseFlash(BootloaderTestCase):

    def _pack_erase_message(self, start, end):
        fmt = ">LL"
        return pack_msg(MessageType.ERASE, struct.pack(fmt, start, end))

    def test_message_too_short(self):
        # Requires 9 bytes, but only 8 are sent.
        packet_data = bytes.fromhex('01 00 00 10 00 00 00 18')
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.DATA_TOO_SHORT)
        self.assertEqual(expected, self.blser.read_packet())

    def test_message_too_long(self):
        # Requires 9 bytes, but 10 are sent.
        packet_data = bytes.fromhex('01 00 00 10 00 00 00 18 00 FF')
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.DATA_TOO_LONG)
        self.assertEqual(expected, self.blser.read_packet())

    def test_start_address_too_low(self):
        packet_data = self._pack_erase_message(start=0x800, end=0x2000)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.ADDR_OUT_OF_RANGE)
        self.assertEqual(expected, self.blser.read_packet())

    def test_end_address_too_high(self):
        packet_data = self._pack_erase_message(start=0x1800, end=0x16000)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.ADDR_OUT_OF_RANGE)
        self.assertEqual(expected, self.blser.read_packet())

    def test_start_address_bad_alignment(self):
        packet_data = self._pack_erase_message(start=0x2400, end=0x15800)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.BAD_ALIGNMENT)
        self.assertEqual(expected, self.blser.read_packet())

    def test_end_address_bad_alignment(self):
        packet_data = self._pack_erase_message(start=0x1800, end=0x12400)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.BAD_ALIGNMENT)
        self.assertEqual(expected, self.blser.read_packet())

#    def test_erase_success(self):
#        # TODO: Add write/verify steps.
#        packet_data = self._pack_erase_message(start=0x1000, end=0x15800)
#        self.blser.write_packet(packet_data)
#
#        expected = pack_response_msg(ResponseCode.OK)
#        self.assertEqual(expected, self.blser.read_packet())
