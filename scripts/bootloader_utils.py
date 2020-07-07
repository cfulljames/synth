import binascii
import enum
import serial
import struct

BOOTLOADER_BAUD = 115200

CRC_LEN = 4

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

class BootloaderSerial():
    """Bootloader serial connection.

    This can be used to interact with the bootloader via a serial port.
    """

    def __init__(self, port):
        self.ser = serial.Serial(port, BOOTLOADER_BAUD, timeout=0)

    def send_erase(self, start, end, response=ResponseCode.OK):
        fmt = ">LL"
        msg = self.pack_msg(MessageType.ERASE, struct.pack(fmt, start, end))
        self.write_packet(msg)
        self.assert_response(response)

    def send_verify(self, start, end, crc, response=ResponseCode.OK):
        fmt = ">LLL"
        msg = self.pack_msg(
            MessageType.VERIFY, struct.pack(fmt, start, end, crc))
        self.write_packet(msg)
        self.assert_response(response, timeout=2)

    def send_write_dword(self, start, data_l, data_h, response=ResponseCode.OK):
        fmt = ">LLL"
        msg = self.pack_msg(
            MessageType.WRITE_DWORD, struct.pack(fmt, start, data_l, data_h))
        self.write_packet(msg)
        self.assert_response(response)

    def send_write_row(self, start, data, response=ResponseCode.OK):
        msg = self.pack_msg(
            MessageType.WRITE_ROW, struct.pack(">L", start) + data)
        self.write_packet(msg)
        self.assert_response(response)

    def send_run(self, response=ResponseCode.OK):
        self.write_packet(bytes([MessageType.RUN]))
        self.assert_response(response)

    def get_device_info(self):
        request = bytes([MessageType.REQUEST_DEVICE_INFO])
        self.write_packet(request)
        response = self.read_packet()
        return DeviceInfo(response)

    def pack_msg(self, msg_type, data):
        return msg_type.to_bytes(1, byteorder='big') + data

    def assert_response(self, response, timeout=1):
        actual = self.read_packet(timeout)
        expected = self.pack_msg(
            MessageType.RESPONSE, response.to_bytes(1, byteorder='big'))
        assert expected == actual, (
            'Incorrect response message.  Expected {}, was {}'.format(
                expected, actual))

    def write_packet(self, data):
        """Format a packet using the bootlader framing protocol and send it over
        serial.
        """
        crc = binascii.crc32(data)
        data = data + crc.to_bytes(CRC_LEN, byteorder='big')
        data = cobs_encode(data) + b'\x00'
        cobs_decode(data[:-1])
        self.write(data)

    def read_packet(self, timeout=1):
        """Read and decode a packet over serial."""
        data = bytearray()
        while True:
            nextByte = self.read(timeout=timeout)
            if nextByte == b'':
                raise TimeoutError('No data received from bootloader.')
            elif nextByte == b'\0':
                break
            else:
                data.append(nextByte[0])

        data = cobs_decode(data)

        encoded_crc = int.from_bytes(data[-CRC_LEN:], byteorder='big')
        actual_crc = binascii.crc32(data[:-CRC_LEN])

        if encoded_crc != actual_crc:
            raise CRCFailError('Actual message CRC did not match encoded CRC.')

        return data[:-CRC_LEN]

    def write(self, data):
        """Write data to the serial port."""
        self.ser.write(data)

    def read(self, length=1, timeout=1):
        """Read data from the serial port.
        length: The maximum number of bytes to read.
        timeout: The maximum time to wait.
        """
        self.ser.timeout = timeout
        return self.ser.read(length)

    def close(self):
        self.ser.close()

class FlashRowData:
    """A row of data to be written to the device via the bootloader."""

    def __init__(self, address, data):
        self.address = address
        self.data = data

class ApplicationCode:
    """Application code to be loaded onto the device using the bootloader.

    The code is read from a hex file and converted into a loadable format.
    """
    DATA = 0x00
    END_OF_FILE = 0x01
    EXTENDED_LINEAR_ADDRESS = 0x04

    def __init__(self, hex_path):
        # The flash pages that need to be erased before loading the app.
        self.pages = []

        # List of rows of data (FlashRowData objects) to be loaded.
        self.rows = []

        self.load_hex_file(hex_path)

    def load_hex_file(self, hex_path):
        """Read data from the given hex file."""

        with open(hex_path, 'r') as hex_file:
            hex_lines = hex_file.readlines()

        all_data = []

        end_of_file = False
        extended_address = 0
        for line in hex_lines:
            line = line.strip()
            if end_of_file:
                # Extra line(s) after the last line marker.
                raise ValueError('Invalid hex file.')
            line_bytes = self.bytes_from_line(line)
            self.verify(line_bytes)
            record_type, address, data = self.split_bytes(line_bytes)

            if record_type == self.END_OF_FILE:
                # End of file
                end_of_file = True

            elif record_type == self.EXTENDED_LINEAR_ADDRESS:
                extended_address = int.from_bytes(data, byteorder='big') << 16

            elif record_type == self.DATA:
                address += extended_address
                all_data.append((address, data))

            else:
                # Unknown record type.
                raise ValueError('Invalid hex file.')

        if not end_of_file:
            # No end of file marker
            raise ValueError('Invalid hex file.')

        self.parse_data(all_data)

    def bytes_from_line(self, line):
        """Convert a line from the hex file into bytes."""

        if line[0] != ':':
            raise ValueError('Invalid hex file.')

        # Remove : from beginning of line
        line = line[1:]
        byte_vals = [int(line[i:i+2], 16) for i in range(0, len(line), 2)]
        return bytes(byte_vals)

    def verify(self, line_bytes):
        """Verify the checksum and length for the bytes from a hex file line."""

        # Check the length
        if len(line_bytes) != 5 + line_bytes[0]:
            raise ValueError('Invalid hex file.')

        # Check the checksum
        line_sum = sum(line_bytes[:-1])
        checksum = (-(line_sum & 0xFF) & 0xFF)

        if checksum != line_bytes[-1]:
            raise ValueError('Invalid hex file.')

    def split_bytes(self, line_bytes):
        """Separate line bytes into address, type, and data."""

        address = int.from_bytes(line_bytes[1:3], byteorder='big')
        record_type = line_bytes[3]
        data = line_bytes[4:-1]
        return record_type, address, data

    def parse_data(self, all_data):
        BYTES_PER_ROW = 512
        # Construct a dict of the form {address: byte, ...}
        all_data_bytes = {}
        for address, data in all_data:
            for data_byte in data:
                if address in all_data_bytes:
                    # Same byte address occurred twice
                    raise ValueError('Invalid hex file.')
                all_data_bytes[address] = data_byte
                address += 1

        # Convert to single contiguous bytearray, filling in gaps with 0xFF
        code_bytes = bytearray()
        max_address = max(all_data_bytes.keys())
        for address in range(max_address):
            try:
                current_byte = all_data_bytes[address]
            except KeyError:
                current_byte = 0xFF
            code_bytes.append(current_byte)

        # Split into separate bytearrays for each flash row
        rows = [
            code_bytes[addr:addr+BYTES_PER_ROW]
            for addr in range(0, max_address, BYTES_PER_ROW)]

        # Make sure last row is full
        last_row_len = len(rows[-1])
        if last_row_len < BYTES_PER_ROW:
            rows[-1] += b'\xFF' * (BYTES_PER_ROW - last_row_len)

        # Populate self.rows list with non-empty rows
        for index, row in enumerate(rows):
            address = int((index * BYTES_PER_ROW) / 2)
            if row != b'\xFF' * BYTES_PER_ROW: # Ignore empty rows
                self.rows.append(FlashRowData(address, row))

        # Populate self.pages with list of non-empty flash pages
        pages = set()
        for row in self.rows:
            address = row.address
            page = int(address / 0x800) * 0x800
            pages.add(page)
        self.pages = sorted(pages)

class CRCFailError(Exception):
    """Indicates that a CRC check failed."""
    pass

class COBSDecodeError(Exception):
    """Incicates that COBS decoding was unsuccessful."""
    pass

def cobs_encode(data):
    """Encode a data packet using Consistent Overhead Byte Stuffing."""
    last_zero_index = 0
    encoded_data = bytearray(b'\x00')

    for byte in data:

        if len(encoded_data) - last_zero_index == 255:
            # Maximum control byte value reached; need to insert a new overhead
            # byte to avoid overflowing the counter.
            encoded_data[last_zero_index] = 255
            last_zero_index = len(encoded_data)
            encoded_data.append(0)

        if byte == 0:
            # Found a new zero byte.  Update the last zero or control byte with
            # the distance to this value.
            encoded_data[last_zero_index] = len(encoded_data) - last_zero_index
            last_zero_index = len(encoded_data)

        # Add the current byte to the output array.  If it's a zero, it will be
        # overwritten later.
        encoded_data.append(byte)

    # Update the last zero or control byte to point to the end of the message.
    encoded_data[last_zero_index] = len(encoded_data) - last_zero_index

    return bytes(encoded_data)

def cobs_decode(data):
    """Decode a packet that was encoded using Consistent Overhead Byte Stuffing
    """
    next_zero_index = 0
    output_index = 0
    drop_next_zero = True
    decoded_data = bytearray()

    for index, byte in enumerate(data):
        if index == next_zero_index:
            # Reached a control byte.  Its value determines the position of the
            # next one.
            next_zero_index = index + byte

            if not drop_next_zero:
                # This byte replaced a zero in the original data
                decoded_data.append(0)

            # If the current byte is 255, the next control byte does not replace
            # a zero from the original data; it was inserted as overhead.
            drop_next_zero = byte == 255

        else:
            # Just a normal data byte.
            decoded_data.append(byte)

    if len(data) != next_zero_index:
        raise COBSDecodeError('Invalid COBS encoding.')

    return bytes(decoded_data)

class VersionNumber():
    def __init__(self, raw):
        self.raw = raw
        self.major = int.from_bytes(self.raw[:1], byteorder='big')
        self.minor = int.from_bytes(self.raw[1:2], byteorder='big')
        self.patch = int.from_bytes(self.raw[2:], byteorder='big')

    def __repr__(self):
        return 'VersionNumber({})'.format(repr(self.raw))

    def __str__(self):
        return '{}.{}.{}'.format(self.major, self.minor, self.patch)

class SerialNumber():
    def __init__(self, raw):
        self.raw = raw

    def __repr__(self):
        return 'SerialNumber({})'.format(repr(self.raw))

    def __str__(self):
        n = 5
        l = 3
        chunks = [self.raw[i:i+l].hex() for i in range(0, l*n, l)]
        return '-'.join(chunks)

class DeviceInfo():
    def __init__(self, raw):
        self.raw = raw
        self.serial_number = SerialNumber(self.raw[:15])
        self.bl_version = VersionNumber(self.raw[15:19])
        self.app_version = VersionNumber(self.raw[19:])

