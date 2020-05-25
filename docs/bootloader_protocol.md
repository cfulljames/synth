Bootloader Communication Protocol
================================================================================

Messages
--------

### Command Result
Type Code = 0x00
Response: None
Direction: Device -> Host

+---------------+---------------+---------------+
|    8 bits     |    8 bits     |    32 bits    |
+---------------+---------------+---------------+
|   Type Code   |  Result Code  |      CRC      |
+---------------+---------------+---------------+

Result Codes:
- 0x00: OK
- 0x01: Serial Protocol Error: Timeout
- 0x02: Serial Protocol Error: Packet CRC Fail
- 0x03: Serial Protocol Error: COBS Decode Fail
- 0x04: Serial Protocol Error: Packet Too Long
- 0x05: Serial Protocol Error: Packet Too Short
- 0x10: Message Error: Invalid Message Type
- 0x11: Message Error: Message Data Too Long for Type

### Erase
Type Code = 0x01
Response: After Processing
Direction: Host -> Device

+---------------+---------------+---------------+---------------+
|    8 bits     |    32 bits    |    16 bits    |    32 bits    |
+---------------+---------------+---------------+---------------+
|   Type Code   | Start Address |    Length     |      CRC      |
+---------------+---------------+---------------+---------------+


### Write
Type Code = 0x02
Response: After Processing
Direction: Host -> Device

+---------------+---------------+---------------+---------------+---------------+
|    8 bits     |    32 bits    |    16 bits    |    n bytes    |    32 bits    |
+---------------+---------------+---------------+---------------+---------------+
|   Type Code   | Start Address |  Length  (n)  |     Data      |      CRC      |
+---------------+---------------+---------------+---------------+---------------+


### Verify
Type Code = 0x03
Response: After Processing
Direction: Host -> Device

+---------------+---------------+---------------+---------------+---------------+
|    8 bits     |    32 bits    |    32 bits    |    32 bits    |    32 bits    |
+---------------+---------------+---------------+---------------+---------------+
|   Type Code   | Start Address |  End Address  | Expected CRC  |      CRC      |
+---------------+---------------+---------------+---------------+---------------+


### Run
Type Code = 0x04
Response: Immediate
Direction: Host -> Device

+---------------+---------------+
|    8 bits     |    32 bits    |
+---------------+---------------+
|   Type Code   |      CRC      |
+---------------+---------------+


### Request Device Info
Type Code = 0x05
Response: Immediate
Direction: Host -> Device

+---------------+---------------+
|    8 bits     |    32 bits    |
+---------------+---------------+
|   Type Code   |      CRC      |
+---------------+---------------+


### Device Info
Type Code = 0x06
Response: None
Direction: Device -> Host

+---------------+---------------+---------------+---------------+---------------+
|    8 bits     |   120 bits    |    32 bits    |    32 bits    |    32 bits    |
+---------------+---------------+---------------+---------------+---------------+
|   Type Code   | Serial Number |  BL Version   |  App Version  |      CRC      |
+---------------+---------------+---------------+---------------+---------------+

BL/App version number format:
+---------------+---------------+---------------+
|    8 bits     |    8 bits     |    16 bits    |
+---------------+---------------+---------------+
|     Major     |     Minor     |     Patch     |
+---------------+---------------+---------------+
