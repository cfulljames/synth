import binascii
import serial

BOOTLOADER_BAUD = 115200

CRC_LEN = 4

class BootloaderSerial():
    """Bootloader serial connection.

    This can be used to interact with the bootloader via a serial port.
    """

    def __init__(self, port):
        self.ser = serial.Serial(port, BOOTLOADER_BAUD, timeout=0)

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

