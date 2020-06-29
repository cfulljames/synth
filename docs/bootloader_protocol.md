Bootloader Communication Protocol
================================================================================

Physical
--------

The bootloader communicates via UART with a baudrate of 115200, 8 bits per byte,
one stop bit, and no parity bit.

Packet Framing
--------------

To construct a packet, the CRC32 is calculated for the message data, and
concatenated onto the end.  This new data is then encoded using
Consistent-Overhead Byte Stuffing (COBS), and a zero byte is added to the end as
a packet separator.

+----------------------+------+
|     COBS Encoded     |      |
| +----------+-------+ | Zero |
| | Msg Data | CRC32 | | Byte |
| +----------+-------+ |      |
+----------------------+------+

Endianness
----------

All data is big-endian; that is, the most significant byte is sent first.

Messages
--------

### Command Result
- Type Code = 0x00
- Response: None
- Direction: Device -> Host

+---------------+---------------+
|    8 bits     |    8 bits     |
+---------------+---------------+
|   Type Code   |  Result Code  |
+---------------+---------------+

#### Result Codes:
- 0x00: OK
- 0x01: Serial Protocol Error: Timeout
- 0x02: Serial Protocol Error: Packet CRC Fail
- 0x03: Serial Protocol Error: COBS Decode Fail
- 0x04: Serial Protocol Error: Packet Too Long
- 0x05: Serial Protocol Error: Packet Too Short
- 0x10: Message Error: Invalid Message Type
- 0x11: Message Error: Message Data Too Long for Type
- 0x12: Message Error: Message Data Too Short for Type
- 0x13: Message Error: One or more addresses have incorrect alignment
- 0x14: Message Error: One or more addresses are out of range
- 0x20: Verification Failure: Calculated CRC does not match
- 0xFF: Internal Error: An unexpected error occurred


### Erase Page
- Type Code = 0x01
- Response: After Processing
- Direction: Host -> Device

+---------------+---------------+---------------+
|    8 bits     |    32 bits    |    32 bits    |
+---------------+---------------+---------------+
|   Type Code   | Start Address |  End Address  |
+---------------+---------------+---------------+


### Write Row
- Type Code = 0x02
- Response: After Processing
- Direction: Host -> Device

+---------------+---------------+---------------+
|    8 bits     |    32 bits    |   512 bytes   |
+---------------+---------------+---------------+
|   Type Code   | Start Address |     Data      |
+---------------+---------------+---------------+


### Verify
- Type Code = 0x03
- Response: After Processing
- Direction: Host -> Device

+---------------+---------------+---------------+---------------+
|    8 bits     |    32 bits    |    32 bits    |    32 bits    |
+---------------+---------------+---------------+---------------+
|   Type Code   | Start Address |  End Address  | Expected CRC  |
+---------------+---------------+---------------+---------------+


### Run
- Type Code = 0x04
- Response: Immediate
- Direction: Host -> Device

+---------------+
|    8 bits     |
+---------------+
|   Type Code   |
+---------------+


### Request Device Info
- Type Code = 0x05
- Response: Immediate
- Direction: Host -> Device

+---------------+
|    8 bits     |
+---------------+
|   Type Code   |
+---------------+


### Device Info
- Type Code = 0x06
- Response: None
- Direction: Device -> Host

+---------------+---------------+---------------+---------------+
|    8 bits     |   120 bits    |    32 bits    |    32 bits    |
+---------------+---------------+---------------+---------------+
|   Type Code   | Serial Number |  BL Version   |  App Version  |
+---------------+---------------+---------------+---------------+

BL/App version number format:

+---------------+---------------+---------------+
|    8 bits     |    8 bits     |    16 bits    |
+---------------+---------------+---------------+
|     Major     |     Minor     |     Patch     |
+---------------+---------------+---------------+


### Write Double Word
- Type Code = 0x07
- Response: After Processing
- Direction: Host -> Device

+---------------+---------------+---------------+
|    8 bits     |    32 bits    |    8 bytes    |
+---------------+---------------+---------------+
|   Type Code   | Start Address |     Data      |
+---------------+---------------+---------------+
