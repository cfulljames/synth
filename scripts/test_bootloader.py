#!/usr/bin/env python3
import binascii
import enum
import os
import time
import struct
import unittest
import bootloader_utils

class MessageType(enum.IntEnum):
    RESPONSE = 0x00
    ERASE = 0x01
    WRITE_ROW = 0x02
    VERIFY = 0x03
    RUN = 0x04
    REQUEST_DEVICE_INFO = 0x05
    DEVICE_INFO = 0x06
    WRITE_DWORD = 0x07

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

    # Message-Specific Statuses
    VERIFICATION_FAIL = 0x20

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

def pack_erase_message(start, end):
    fmt = ">LL"
    return pack_msg(MessageType.ERASE, struct.pack(fmt, start, end))

def pack_verify_message(start, end, crc):
    fmt = ">LLL"
    return pack_msg(MessageType.VERIFY, struct.pack(fmt, start, end, crc))

def pack_write_dword_message(start, data_l, data_h):
    fmt = ">LLL"
    return pack_msg(
        MessageType.WRITE_DWORD, struct.pack(fmt, start, data_l, data_h))

def pack_write_row_message(start, data):
    return pack_msg(MessageType.WRITE_ROW, struct.pack(">L", start) + data)

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
        packet_data = pack_erase_message(start=0x800, end=0x2000)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.ADDR_OUT_OF_RANGE)
        self.assertEqual(expected, self.blser.read_packet())

    def test_end_address_too_high(self):
        packet_data = pack_erase_message(start=0x1800, end=0x16000)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.ADDR_OUT_OF_RANGE)
        self.assertEqual(expected, self.blser.read_packet())

    def test_start_address_bad_alignment(self):
        packet_data = pack_erase_message(start=0x2400, end=0x15800)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.BAD_ALIGNMENT)
        self.assertEqual(expected, self.blser.read_packet())

    def test_end_address_bad_alignment(self):
        packet_data = pack_erase_message(start=0x1800, end=0x12400)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.BAD_ALIGNMENT)
        self.assertEqual(expected, self.blser.read_packet())

#    def test_erase_success(self):
#        # TODO: Add write/verify steps.
#        packet_data = pack_erase_message(start=0x1000, end=0x15800)
#        self.blser.write_packet(packet_data)
#
#        expected = pack_response_msg(ResponseCode.OK)
#        self.assertEqual(expected, self.blser.read_packet())

class TestVerify(BootloaderTestCase):

    def test_message_too_short(self):
        # Requires 13 bytes, but only 12 are sent.
        packet_data = bytes([MessageType.VERIFY] + [1] * 11)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.DATA_TOO_SHORT)
        self.assertEqual(expected, self.blser.read_packet())

    def test_message_too_long(self):
        # Requires 13 bytes, but 14 are sent.
        packet_data = bytes([MessageType.VERIFY] + [1] * 13)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.DATA_TOO_LONG)
        self.assertEqual(expected, self.blser.read_packet())

    def test_end_address_too_high(self):
        packet_data = pack_verify_message(start=0x1800, end=0x1000002, crc=0x0)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.ADDR_OUT_OF_RANGE)
        self.assertEqual(expected, self.blser.read_packet())

    def test_start_address_bad_alignment(self):
        packet_data = pack_verify_message(start=0x1801, end=0x1000000, crc=0x0)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.BAD_ALIGNMENT)
        self.assertEqual(expected, self.blser.read_packet())

    def test_end_address_bad_alignment(self):
        packet_data = pack_verify_message(start=0x1800, end=0x0FFFFFF, crc=0x0)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.BAD_ALIGNMENT)
        self.assertEqual(expected, self.blser.read_packet())

    def test_start_address_too_high(self):
        packet_data = pack_verify_message(start=0x1800, end=0x1000, crc=0x0)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.ADDR_OUT_OF_RANGE)
        self.assertEqual(expected, self.blser.read_packet())

class TestEraseVerify(BootloaderTestCase):

    def test_erase_verify_one_page(self):
        # Erase a section of flash so that it has a known CRC
        packet_data = pack_erase_message(start=0x1000, end=0x1800)
        self.blser.write_packet(packet_data)

        # Check the erase response
        expected = pack_response_msg(ResponseCode.OK)
        self.assertEqual(expected, self.blser.read_packet())

        # Calculate the expected CRC for erased flash
        data = b'\x00\xFF\xFF\xFF' * 0x400
        expected_crc = binascii.crc32(data)

        # Send the verify command
        packet_data = pack_verify_message(
            start=0x1000, end=0x1800, crc=expected_crc)
        self.blser.write_packet(packet_data)

        # Check the response
        expected = pack_response_msg(ResponseCode.OK)
        self.assertEqual(expected, self.blser.read_packet())

    def test_erase_verify_all_app_flash(self):
        # Erase a section of flash so that it has a known CRC
        packet_data = pack_erase_message(start=0x1000, end=0x15800)
        self.blser.write_packet(packet_data)

        # Check the erase response
        expected = pack_response_msg(ResponseCode.OK)
        self.assertEqual(expected, self.blser.read_packet())

        # Calculate the expected CRC for erased flash
        data = b'\x00\xFF\xFF\xFF' * (0x14800 >> 1)
        expected_crc = binascii.crc32(data)

        # Send the verify command
        packet_data = pack_verify_message(
            start=0x1000, end=0x15800, crc=expected_crc)
        self.blser.write_packet(packet_data)

        # Check the response
        expected = pack_response_msg(ResponseCode.OK)
        self.assertEqual(expected, self.blser.read_packet(3))

class TestWriteDoubleWord(BootloaderTestCase):

    def test_message_too_short(self):
        # Requires 13 bytes, but only 12 are sent.
        packet_data = bytes([MessageType.WRITE_DWORD] + [1] * 11)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.DATA_TOO_SHORT)
        self.assertEqual(expected, self.blser.read_packet())

    def test_message_too_long(self):
        # Requires 13 bytes, but 14 are sent.
        packet_data = bytes([MessageType.WRITE_DWORD] + [1] * 13)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.DATA_TOO_LONG)
        self.assertEqual(expected, self.blser.read_packet())

    def test_start_address_too_high(self):
        packet_data = pack_write_dword_message(
            start=0x15800, data_l=0x00ABCDEF, data_h=0x00CAFECAFE)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.ADDR_OUT_OF_RANGE)
        self.assertEqual(expected, self.blser.read_packet())

    def test_start_address_bad_alignment(self):
        packet_data = pack_write_dword_message(
            start=0x1570A, data_l=0x00ABCDEF, data_h=0x00CAFECAFE)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.BAD_ALIGNMENT)
        self.assertEqual(expected, self.blser.read_packet())

    def test_start_address_too_low(self):
        packet_data = pack_write_dword_message(
            start=0x0F0C, data_l=0x00ABCDEF, data_h=0x00CAFECAFE)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.ADDR_OUT_OF_RANGE)
        self.assertEqual(expected, self.blser.read_packet())

    def test_erase_write_double_word_verify(self):

        # Erase a section of flash so that it has a known CRC
        packet_data = pack_erase_message(start=0x2000, end=0x2800)
        self.blser.write_packet(packet_data)

        # Check the erase response
        expected = pack_response_msg(ResponseCode.OK)
        self.assertEqual(expected, self.blser.read_packet())

        # Write a double-word to flash.
        packet_data = pack_write_dword_message(
            start=0x2000, data_l=0x00ABCDEF, data_h=0x00CAFEFE)
        self.blser.write_packet(packet_data)

        # Check the write response.
        expected = pack_response_msg(ResponseCode.OK)
        self.assertEqual(expected, self.blser.read_packet())

        # Calculate the expected CRC for newly written flash.
        data = bytes.fromhex('00 AB CD EF 00 CA FE FE')
        expected_crc = binascii.crc32(data)

        # Send the verify command
        packet_data = pack_verify_message(
            start=0x2000, end=0x2004, crc=expected_crc)
        self.blser.write_packet(packet_data)

        # Check the response
        expected = pack_response_msg(ResponseCode.OK)
        self.assertEqual(expected, self.blser.read_packet())

class TestWriteRow(BootloaderTestCase):

    def test_message_too_short(self):
        # Requires 517 bytes, but only 516 are sent.
        packet_data = bytes([MessageType.WRITE_ROW] + [1] * 515)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.DATA_TOO_SHORT)
        self.assertEqual(expected, self.blser.read_packet())

    def test_message_too_long(self):
        # Requires 517 bytes, but 518 are sent.
        packet_data = bytes([MessageType.WRITE_ROW] + [1] * 517)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.DATA_TOO_LONG)
        self.assertEqual(expected, self.blser.read_packet())

    def test_start_address_too_high(self):
        data = b'\x00' * 512
        packet_data = pack_write_row_message(start=0x15800, data=data)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.ADDR_OUT_OF_RANGE)
        self.assertEqual(expected, self.blser.read_packet())

    def test_start_address_bad_alignment(self):
        data = b'\x00' * 512
        packet_data = pack_write_row_message(start=0x01080, data=data)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.BAD_ALIGNMENT)
        self.assertEqual(expected, self.blser.read_packet())

    def test_start_address_too_low(self):
        data = b'\x00' * 512
        packet_data = pack_write_row_message(start=0x00F00, data=data)
        self.blser.write_packet(packet_data)

        expected = pack_response_msg(ResponseCode.ADDR_OUT_OF_RANGE)
        self.assertEqual(expected, self.blser.read_packet())

    def test_erase_write_row_verify(self):

        # Erase a section of flash
        packet_data = pack_erase_message(start=0x4000, end=0x4800)
        self.blser.write_packet(packet_data)

        # Check the erase response
        expected = pack_response_msg(ResponseCode.OK)
        self.assertEqual(expected, self.blser.read_packet())

        # Write a row to flash
        data = b'\xEF\xCD\xAB\x00' * 128
        packet_data = pack_write_row_message(start=0x4000, data=data)
        self.blser.write_packet(packet_data)

        # Check the write response.
        expected = pack_response_msg(ResponseCode.OK)
        self.assertEqual(expected, self.blser.read_packet())

        # Calculate the expected CRC for newly written flash.
        data = b'\x00\xAB\xCD\xEF' * 128 # Swap endianness for CRC
        expected_crc = binascii.crc32(data)

        # Send the verify command
        packet_data = pack_verify_message(
            start=0x4000, end=0x4100, crc=expected_crc)
        self.blser.write_packet(packet_data)

        # Check the response
        expected = pack_response_msg(ResponseCode.OK)
        self.assertEqual(expected, self.blser.read_packet())

class TestRun(BootloaderTestCase):

    def test_run(self):
        self.blser.write_packet(bytes([MessageType.RUN]))
        expected = pack_response_msg(ResponseCode.OK)
        self.assertEqual(expected, self.blser.read_packet())
