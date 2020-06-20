import bootloader_utils
import os
import time
import unittest

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

        expected = bytes.fromhex('00 02')
        self.assertEqual(expected, self.blser.read_packet())

    def test_packet_with_invalid_cobs(self):
        send = bytes.fromhex('01 ff ff ff ff 00')
        self.blser.write(send)

        expected = bytes.fromhex('00 03')
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
        expected = bytes.fromhex('00 01')
        self.assertEqual(expected, self.blser.read_packet(timeout=0.020))

    def test_packet_too_short(self):
        MIN_PACKET_LEN = 5
        for packet_len in range(1, MIN_PACKET_LEN):
            send = (b'\01' * packet_len) + b'\x00'
            self.blser.write(send)

            expected = bytes.fromhex('00 05')
            self.assertEqual(expected, self.blser.read_packet())

    def test_packet_too_long(self):
        MAX_DATA_LENGTH = 2048
        packet_data = b'\xcc' * (MAX_DATA_LENGTH + 1)
        self.blser.write_packet(packet_data)

        expected = bytes.fromhex('00 04')
        self.assertEqual(expected, self.blser.read_packet())

class TestMessageErrors(BootloaderTestCase):

    def test_invalid_message_type(self):
        send = b'\xFF'
        self.blser.write_packet(send)

        expected = bytes.fromhex('00 10')
        self.assertEqual(expected, self.blser.read_packet())

class TestDeviceInfoMessage(BootloaderTestCase):

    def test_device_info_extra_bytes_in_request(self):
        request = b'\x05\xab'
        self.blser.write_packet(request)

        self.assertEqual(b'\x00\x11', self.blser.read_packet())

    def test_valid_request(self):
        request = b'\x05'
        self.blser.write_packet(request)

        response = self.blser.read_packet()
        self.assertEqual(6, response[0])
        self.assertEqual(24, len(response))
