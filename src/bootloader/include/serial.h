/*
 * Serial Communication
 *
 * Handles COBS encoding and decoding, as well as CRC checks for UART
 * communication.  This listens for incoming data and transmits outgoing data.
 *
 * The following message framing protocol is implemented:
 * - Each frame is COBs encoded, and is terminated with exactly one 0 byte.
 * - The last four bytes of decoded data are the CRC32 for all preceding bytes
 *   in the decoded frame.
 * - The maximum encoded frame size is 1050 bytes.
 * - The maximum time between received bytes in the same frame is 50 ms.
 *
 *
 * Frame Structure:
 *
 *  +----------------------+------+
 *  |     COBS Encoded     |      |
 *  | +----------+-------+ | Zero |
 *  | | Msg Data | CRC32 | | Byte |
 *  | +----------+-------+ |      |
 *  +----------------------+------+
 */
#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

// Maximum transmit message length
#define SERIAL_MAX_TX_LENGTH (32U)

// Maximum receive message length
#define SERIAL_MAX_RX_LENGTH (2048U)

typedef enum serial_status_e {
    SERIAL_OK                   =  0,
    SERIAL_DATA_TOO_LONG        = -1,
    SERIAL_DATA_TOO_SHORT       = -2,
    SERIAL_DATA_NULL            = -3,
    SERIAL_COBS_DECODE_FAIL     = -4,
    SERIAL_CRC_FAIL             = -5,
    SERIAL_TIMEOUT              = -6,
} serial_status_t;

typedef void (*serial_error_cb_t)(serial_status_t error);
typedef void (*serial_msg_cb_t)(const uint8_t *data, uint16_t length);

/*
 * Initialize the serial module.
 */
void serial_init(void);

/*
 * Set the error callback.
 *
 * This callback will be invoked whenever a serial decoding error occurs.
 */
void serial_set_error_callback(serial_error_cb_t cb);

/*
 * Set the message received callback.
 *
 * This callback will be invoked whenever a serial message frame is successfully
 * decoded and verified.
 */
void serial_set_msg_received_callback(serial_msg_cb_t cb);

/*
 * Send a message using the serial connection.
 *
 * This will format and encode the given message for serial transmission, and
 * send it over UART.
 */
serial_status_t serial_send(const uint8_t *data, uint16_t length);

/*
 * Update the serial listener.
 *
 * This will check the UART for new received bytes, and invoke the message
 * callback or error callback as appropriate.
 */
void serial_update(void);

#endif // SERIAL_H
