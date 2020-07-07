#!/usr/bin/env python3
import binascii
import os
import time
import struct
import unittest
import bootloader_utils
from bootloader_utils import MessageType, ResponseCode

def open_serial_port():
    if not 'SYNTH_SERIAL_PORT' in os.environ:
        raise KeyError('Environment variable SYNTH_SERIAL_PORT not set.')
    ser =  bootloader_utils.BootloaderSerial(os.environ['SYNTH_SERIAL_PORT'])
    return ser

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
        self.blser.assert_response(ResponseCode.PACKET_CRC_FAIL)

    def test_packet_with_invalid_cobs(self):
        send = bytes.fromhex('01 ff ff ff ff 00')
        self.blser.write(send)
        self.blser.assert_response(ResponseCode.COBS_DECODE_FAIL)

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
        self.blser.assert_response(ResponseCode.TIMEOUT)

    def test_packet_too_short(self):
        MIN_PACKET_LEN = 5
        for packet_len in range(1, MIN_PACKET_LEN):
            send = (b'\01' * packet_len) + b'\x00'
            self.blser.write(send)
            self.blser.assert_response(ResponseCode.PACKET_TOO_SHORT)

    def test_packet_too_long(self):
        MAX_DATA_LENGTH = 2048
        packet_data = b'\xcc' * (MAX_DATA_LENGTH + 1)
        self.blser.write_packet(packet_data)
        self.blser.assert_response(ResponseCode.PACKET_TOO_LONG)

class TestMessageErrors(BootloaderTestCase):

    def test_invalid_message_type(self):
        send = b'\xFF'
        self.blser.write_packet(send)
        self.blser.assert_response(ResponseCode.INVALID_TYPE)

class TestDeviceInfoMessage(BootloaderTestCase):

    def test_device_info_extra_bytes_in_request(self):
        request = bytes([MessageType.REQUEST_DEVICE_INFO, 0xAB])
        self.blser.write_packet(request)
        self.blser.assert_response(ResponseCode.DATA_TOO_LONG)

    def test_valid_request(self):
        # This will raise an exception if device info is not received
        # successfully.
        self.blser.get_device_info()

class TestEraseFlash(BootloaderTestCase):

    def test_message_too_short(self):
        # Requires 9 bytes, but only 8 are sent.
        packet_data = bytes.fromhex('01 00 00 10 00 00 00 18')
        self.blser.write_packet(packet_data)
        self.blser.assert_response(ResponseCode.DATA_TOO_SHORT)

    def test_message_too_long(self):
        # Requires 9 bytes, but 10 are sent.
        packet_data = bytes.fromhex('01 00 00 10 00 00 00 18 00 FF')
        self.blser.write_packet(packet_data)
        self.blser.assert_response(ResponseCode.DATA_TOO_LONG)

    def test_start_address_too_low(self):
        self.blser.send_erase(
            start=0x800, end=0x2000, response=ResponseCode.ADDR_OUT_OF_RANGE)

    def test_end_address_too_high(self):
        self.blser.send_erase(
            start=0x1800, end=0x16000, response=ResponseCode.ADDR_OUT_OF_RANGE)

    def test_start_address_bad_alignment(self):
        self.blser.send_erase(
            start=0x2400, end=0x15800, response=ResponseCode.BAD_ALIGNMENT)

    def test_end_address_bad_alignment(self):
        self.blser.send_erase(
            start=0x1800, end=0x12400, response=ResponseCode.BAD_ALIGNMENT)

class TestVerify(BootloaderTestCase):

    def test_message_too_short(self):
        # Requires 13 bytes, but only 12 are sent.
        packet_data = bytes([MessageType.VERIFY] + [1] * 11)
        self.blser.write_packet(packet_data)
        self.blser.assert_response(ResponseCode.DATA_TOO_SHORT)

    def test_message_too_long(self):
        # Requires 13 bytes, but 14 are sent.
        packet_data = bytes([MessageType.VERIFY] + [1] * 13)
        self.blser.write_packet(packet_data)
        self.blser.assert_response(ResponseCode.DATA_TOO_LONG)

    def test_end_address_too_high(self):
        self.blser.send_verify(
            start=0x1800, end=0x1000002, crc=0x0,
            response=ResponseCode.ADDR_OUT_OF_RANGE)

    def test_start_address_bad_alignment(self):
        self.blser.send_verify(
            start=0x1801, end=0x1000000, crc=0x0,
            response=ResponseCode.BAD_ALIGNMENT)

    def test_end_address_bad_alignment(self):
        self.blser.send_verify(
            start=0x1800, end=0x0FFFFFF, crc=0x0,
            response=ResponseCode.BAD_ALIGNMENT)

    def test_start_address_too_high(self):
        self.blser.send_verify(
            start=0x1800, end=0x1000, crc=0x0,
            response=ResponseCode.ADDR_OUT_OF_RANGE)

class TestEraseVerify(BootloaderTestCase):

    def test_erase_verify_one_page(self):
        # Erase a section of flash so that it has a known CRC
        self.blser.send_erase(start=0x1000, end=0x1800)

        # Calculate the expected CRC for erased flash
        data = b'\x00\xFF\xFF\xFF' * 0x400
        expected_crc = binascii.crc32(data)

        # Send the verify command
        self.blser.send_verify(
            start=0x1000, end=0x1800, crc=expected_crc)

    def test_erase_verify_all_app_flash(self):
        # Erase a section of flash so that it has a known CRC
        self.blser.send_erase(start=0x1000, end=0x15800)

        # Calculate the expected CRC for erased flash
        data = b'\x00\xFF\xFF\xFF' * (0x14800 >> 1)
        expected_crc = binascii.crc32(data)

        # Send the verify command
        self.blser.send_verify(start=0x1000, end=0x15800, crc=expected_crc)

class TestWriteDoubleWord(BootloaderTestCase):

    def test_message_too_short(self):
        # Requires 13 bytes, but only 12 are sent.
        packet_data = bytes([MessageType.WRITE_DWORD] + [1] * 11)
        self.blser.write_packet(packet_data)

        self.blser.assert_response(ResponseCode.DATA_TOO_SHORT)

    def test_message_too_long(self):
        # Requires 13 bytes, but 14 are sent.
        packet_data = bytes([MessageType.WRITE_DWORD] + [1] * 13)
        self.blser.write_packet(packet_data)

        self.blser.assert_response(ResponseCode.DATA_TOO_LONG)

    def test_start_address_too_high(self):
        self.blser.send_write_dword(
            start=0x15800, data_l=0x00ABCDEF, data_h=0x00CAFECAFE,
            response=ResponseCode.ADDR_OUT_OF_RANGE)

    def test_start_address_bad_alignment(self):
        self.blser.send_write_dword(
            start=0x1570A, data_l=0x00ABCDEF, data_h=0x00CAFECAFE,
            response=ResponseCode.BAD_ALIGNMENT)

    def test_start_address_too_low(self):
        self.blser.send_write_dword(
            start=0x0F0C, data_l=0x00ABCDEF, data_h=0x00CAFECAFE,
            response=ResponseCode.ADDR_OUT_OF_RANGE)

    def test_erase_write_double_word_verify(self):

        # Erase a page of flash
        self.blser.send_erase(start=0x2000, end=0x2800)

        # Write a double-word to flash.
        self.blser.send_write_dword(
            start=0x2000, data_l=0x00ABCDEF, data_h=0x00CAFEFE)

        # Calculate the expected CRC for newly written flash.
        data = bytes.fromhex('00 AB CD EF 00 CA FE FE')
        expected_crc = binascii.crc32(data)

        # Send the verify command
        self.blser.send_verify(start=0x2000, end=0x2004, crc=expected_crc)

class TestWriteRow(BootloaderTestCase):

    def test_message_too_short(self):
        # Requires 517 bytes, but only 516 are sent.
        packet_data = bytes([MessageType.WRITE_ROW] + [1] * 515)
        self.blser.write_packet(packet_data)

        self.blser.assert_response(ResponseCode.DATA_TOO_SHORT)

    def test_message_too_long(self):
        # Requires 517 bytes, but 518 are sent.
        packet_data = bytes([MessageType.WRITE_ROW] + [1] * 517)
        self.blser.write_packet(packet_data)

        self.blser.assert_response(ResponseCode.DATA_TOO_LONG)

    def test_start_address_too_high(self):
        data = b'\x00' * 512
        self.blser.send_write_row(
            start=0x15800, data=data, response=ResponseCode.ADDR_OUT_OF_RANGE)

    def test_start_address_bad_alignment(self):
        data = b'\x00' * 512
        self.blser.send_write_row(
            start=0x01080, data=data, response=ResponseCode.BAD_ALIGNMENT)

    def test_start_address_too_low(self):
        data = b'\x00' * 512
        self.blser.send_write_row(
            start=0x00F00, data=data, response=ResponseCode.ADDR_OUT_OF_RANGE)

    def test_erase_write_row_verify(self):

        # Erase a section of flash
        self.blser.send_erase(start=0x4000, end=0x4800)

        # Write a row to flash
        data = b'\xEF\xCD\xAB\x00' * 128
        self.blser.send_write_row(start=0x4000, data=data)

        # Calculate the expected CRC for newly written flash.
        data = b'\x00\xAB\xCD\xEF' * 128 # Swap endianness for CRC
        expected_crc = binascii.crc32(data)

        # Send the verify command
        self.blser.send_verify(
            start=0x4000, end=0x4100, crc=expected_crc)

#class TestRun(BootloaderTestCase):
#
#    def test_program_and_run(self):
#        SCRIPT_DIR = os.path.dirname(__file__)
#        HEX_PATH = os.path.join(
#            SCRIPT_DIR, '..', 'build', 'src', 'master', 'synth_master.hex')
#        code = bootloader_utils.ApplicationCode(HEX_PATH)
#
#        # Erase pages
#        for page in code.pages:
#            print('.', end='', flush=True)
#            self.blser.send_erase(start=page, end=page+0x800)
#
#        # Write/verify rows
#        for row in code.rows:
#            print('.', end='', flush=True)
#            self.blser.send_write_row(
#                start=row.address, data=row.data)
#
#            row_words = [row.data[i:i+4] for i in range(0, 512, 4)]
#            row_words = [bytes(reversed(word)) for word in row_words]
#            row_data = bytearray(b''.join(row_words))
#            # Replace MSB of each  word with 0
#            for index in range(len(row_data)):
#                if index % 4 == 0:
#                    row_data[index] = 0
#            crc = binascii.crc32(row_data)
#
#            self.blser.send_verify(
#                start=row.address, end=row.address+0x100, crc=crc)
#
#        # Run application
#        self.blser.send_run()
#        print('')
