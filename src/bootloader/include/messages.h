/*
 * Bootloader message protocol.
 *
 * This module processes the command messages and executes the appropriate
 * action and sends a response message.  The message protocol definition can be
 * found in docs/bootloader_protocol.md.
 */
#ifndef MESSAGES_H
#define MESSAGES_H

// Message response codes; used to indicate the status of a command request.
typedef enum msg_response_e {
    MESSAGE_RESP_OK = 0x00,
    MESSAGE_RESP_SERIAL_TIMEOUT = 0x01,
    MESSAGE_RESP_SERIAL_CRC_FAIL = 0x02,
    MESSAGE_RESP_SERIAL_COBS_DECODE_FAIL = 0x03,
    MESSAGE_RESP_SERIAL_PACKET_TOO_LONG = 0x04,
    MESSAGE_RESP_SERIAL_PACKET_TOO_SHORT = 0x05,

    MESSAGE_RESP_INVALID_TYPE = 0x10,
    MESSAGE_RESP_MESSAGE_TOO_LONG = 0x11,
    MESSAGE_RESP_MESSAGE_TOO_SHORT = 0x12,
    MESSAGE_RESP_ADDRESS_BAD_ALIGNMENT = 0x13,
    MESSAGE_RESP_ADDRESS_OUT_OF_RANGE = 0x14,

    MESSAGE_RESP_VERIFICATION_FAIL = 0x20,

    MESSAGE_RESP_INTERNAL_ERROR = 0xFF,
} msg_response_t;

// Message type codes; used to identify the packet type.
typedef enum msg_type_e {
    MESSAGE_TYPE_CMD_RESULT = 0,
    MESSAGE_TYPE_ERASE = 1,
    MESSAGE_TYPE_VERIFY = 3,
    MESSAGE_TYPE_DEVICE_INFO_REQUEST = 5,
    MESSAGE_TYPE_DEVICE_INFO = 6,
    MESSAGE_TYPE_WRITE_DWORD = 7,
} msg_type_t;

/*
 * Initialize the message handler.
 *
 * This will register callbacks in the serial module for handling received
 * messages.
 */
void messages_init(void);

#endif // MESSAGES_H
