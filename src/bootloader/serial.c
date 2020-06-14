#include "serial.h"
#include "cobs.h"
#include "uart.h"
#include "crc.h"
#include "timer.h"
#include <stddef.h>
#include <string.h>

#define CRC_LENGTH (sizeof(uint32_t))

#define MAX_DECODED_TX_PACKET_LENGTH (SERIAL_MAX_TX_LENGTH + CRC_LENGTH)
#define MAX_DECODED_RX_PACKET_LENGTH (SERIAL_MAX_RX_LENGTH + CRC_LENGTH)

#define MAX_ENCODED_TX_PACKET_LENGTH \
    (COBS_MAX_DATA_LENGTH_AFTER_ENCODE(MAX_DECODED_TX_PACKET_LENGTH))

#define MAX_ENCODED_RX_PACKET_LENGTH \
    (COBS_MAX_DATA_LENGTH_AFTER_ENCODE(MAX_DECODED_RX_PACKET_LENGTH))

static serial_status_t handle_data_byte(uint8_t rx_byte);
static serial_status_t handle_end_of_packet(void);
static serial_status_t check_crc_for_data(
        uint8_t *decoded_data, uint16_t decoded_length);

static serial_error_cb_t m_error_cb = NULL;
static serial_msg_cb_t m_msg_received_cb = NULL;
static uint8_t m_rx_data[MAX_ENCODED_RX_PACKET_LENGTH];
static uint16_t m_rx_data_index = 0;

void serial_init(void)
{
    m_rx_data_index = 0;
}

void serial_set_error_callback(serial_error_cb_t cb)
{
    m_error_cb = cb;
}

void serial_set_msg_received_callback(serial_msg_cb_t cb)
{
    m_msg_received_cb = cb;
}

serial_status_t serial_send(const uint8_t *data, uint16_t length)
{
    uint8_t data_with_crc[MAX_DECODED_TX_PACKET_LENGTH];
    uint8_t encoded_data[MAX_ENCODED_TX_PACKET_LENGTH];
    uint16_t encoded_length;
    uint32_t crc;
    uint16_t data_index;
    serial_status_t status = SERIAL_OK;

    if (data == NULL)
    {
        status = SERIAL_DATA_NULL;
    }

    if (length > SERIAL_MAX_TX_LENGTH)
    {
        status = SERIAL_DATA_TOO_LONG;
    }

    if (status == SERIAL_OK)
    {
        crc = crc_calculate(data, length);
        memcpy(data_with_crc, data, length);
        data_with_crc[length++] = (uint8_t)((crc & 0xFF000000) >> 24);
        data_with_crc[length++] = (uint8_t)((crc & 0x00FF0000) >> 16);
        data_with_crc[length++] = (uint8_t)((crc & 0x0000FF00) >> 8);
        data_with_crc[length++] = (uint8_t)((crc & 0x000000FF) >> 0);

        encoded_length = cobs_encode(data_with_crc, length, encoded_data);
        for (data_index = 0; data_index < encoded_length; data_index++)
        {
            uart_write(encoded_data[data_index]);
        }

        // Send null packet separator.
        uart_write(0);
    }

    return status;
}

void serial_update(void)
{
    uint8_t rx_byte;
    bool byte_received;
    byte_received = uart_read(&rx_byte);
    serial_status_t status = SERIAL_OK;

    if (byte_received)
    {
        if (0 == rx_byte)
        {
            // The zero byte is the packet separator; this is the end of the
            // packet.
            status = handle_end_of_packet();
        }
        else
        {
            // A non-zero byte was received.  This is just another data byte.
            status = handle_data_byte(rx_byte);
        }
    }
    else if (m_rx_data_index && timer_check_expired())
    {
        // No new byte received, and it's been too long since the last one.
        // We'll assume that a communication problem happened and set the error.
        status = SERIAL_TIMEOUT;
        m_rx_data_index = 0;
    }

    if (SERIAL_OK != status && m_error_cb)
    {
        m_error_cb(status);
    }
}

static serial_status_t handle_data_byte(uint8_t rx_byte)
{
    serial_status_t status = SERIAL_OK;
    if (m_rx_data_index == MAX_ENCODED_RX_PACKET_LENGTH)
    {
        // Ran out of buffer space.
        status = SERIAL_DATA_TOO_LONG;
        m_rx_data_index = 0;
    }
    else
    {
        // Still room in the buffer; just keep adding bytes.
        m_rx_data[m_rx_data_index++] = rx_byte;
    }

    // We got a byte, so reset the timeout timer.
    timer_reset();
    timer_start();

    return status;
}

static serial_status_t handle_end_of_packet(void)
{
    int16_t data_length;
    int16_t decoded_length;
    uint8_t decoded_data[MAX_DECODED_RX_PACKET_LENGTH];
    serial_status_t status = SERIAL_OK;

    // End of packet.
    if (0 == m_rx_data_index)
    {
        // No data in buffer; empty packet.
        status = SERIAL_DATA_TOO_SHORT;
    }
    else
    {
        // At least one byte of data was received.

        decoded_length = cobs_decode(m_rx_data, m_rx_data_index, decoded_data);

        if (decoded_length == COBS_DECODE_FAIL)
        {
            // Invalid COBS encoding.
            status = SERIAL_COBS_DECODE_FAIL;
        }
        else if (decoded_length < CRC_LENGTH + 1)
        {
            // Not enough data to calculate a CRC.
            status = SERIAL_DATA_TOO_SHORT;
        }
        else
        {
            // Packet long enough for a CRC check.
            status = check_crc_for_data(decoded_data, decoded_length);
            data_length = decoded_length - CRC_LENGTH;
        }
    }

    if (SERIAL_OK == status && m_msg_received_cb)
    {
        // Valid packet was received, invoke the callback.
        m_msg_received_cb(decoded_data, data_length);
    }

    return status;
}

static serial_status_t check_crc_for_data(
        uint8_t *decoded_data, uint16_t decoded_length)
{
    uint32_t actual_crc = 0;
    uint32_t expected_crc = 0;
    uint16_t data_length = decoded_length - CRC_LENGTH;
    serial_status_t status = SERIAL_OK;

    actual_crc = crc_calculate(decoded_data, data_length);

    // Read CRC from last four bytes of data.
    uint16_t data_crc_index = data_length;
    expected_crc |= (uint32_t)decoded_data[data_crc_index++] << 24;
    expected_crc |= (uint32_t)decoded_data[data_crc_index++] << 16;
    expected_crc |= (uint32_t)decoded_data[data_crc_index++] <<  8;
    expected_crc |= (uint32_t)decoded_data[data_crc_index++] <<  0;

    if (expected_crc != actual_crc)
    {
        // Last four bytes of data do not match calculated CRC for the packet.
        status = SERIAL_CRC_FAIL;
    }

    return status;
}
