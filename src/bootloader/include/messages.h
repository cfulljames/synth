/*
 * Bootloader message protocol.
 *
 * This module processes the command messages and executes the appropriate
 * action and sends a response message.  The message protocol definition can be
 * found in docs/bootloader_protocol.md.
 */
#ifndef MESSAGES_H
#define MESSAGES_H

typedef enum msg_response_e {
    MESSAGE_RESP_OK = 0,
    MESSAGE_RESP_SERIAL_TIMEOUT = 1,
    MESSAGE_RESP_SERIAL_CRC_FAIL = 2,
    MESSAGE_RESP_SERIAL_COBS_DECODE_FAIL = 3,
    MESSAGE_RESP_SERIAL_PACKET_TOO_LONG = 4,
    MESSAGE_RESP_SERIAL_PACKET_TOO_SHORT = 5,
    MESSAGE_RESP_INVALID_TYPE = 6,
    MESSAGE_RESP_MESSAGE_TOO_LONG = 7,
} msg_response_t;

typedef enum msg_type_e {
    MESSAGE_TYPE_CMD_RESULT = 0,
    MESSAGE_TYPE_DEVICE_INFO_REQUEST = 5,
    MESSAGE_TYPE_DEVICE_INFO = 6,
} msg_type_t;

void messages_init(void);

#endif // MESSAGES_H
