#include "unity.h"

#include "serial.h"
#include "cobs.h"
#include "mock_crc.h"
#include "mock_uart.h"
#include "mock_timer.h"

#include <string.h>

static void msg_callback(const uint8_t *data, uint16_t length);
static void error_callback(serial_status_t error);
static void inject_uart_byte(uint8_t *byte);
static void inject_uart_data(uint8_t *data, int length);
static void assert_error_cb_called_once_with_error(serial_status_t error);

serial_status_t m_error;
int m_error_callback_called;
uint8_t m_msg_data_buffer[SERIAL_MAX_RX_LENGTH];
uint16_t m_msg_data_length;

void setUp(void)
{
    serial_init();
    serial_set_error_callback(error_callback);
    serial_set_msg_received_callback(msg_callback);
    m_error_callback_called = 0;
    m_error = SERIAL_OK;
}

void tearDown(void)
{
}

/******************************************************************************
 * Send
 ******************************************************************************/

void test_send_too_long(void)
{
    uint8_t data;
    serial_status_t ret;
    ret = serial_send(&data, SERIAL_MAX_TX_LENGTH + 1);
    TEST_ASSERT_EQUAL_INT(SERIAL_DATA_TOO_LONG, ret);
}

void test_send_null(void)
{
    serial_status_t ret;
    ret = serial_send(NULL, 1);
    TEST_ASSERT_EQUAL_INT(SERIAL_DATA_NULL, ret);
}

void test_send_max_length(void)
{
    // 32 Bytes of Valid Data + 4 byte CRC
    uint8_t data[] = {
        // Data
        0xCA, 0xFE, 0xAB, 0xCD, 0x12, 0x34, 0x56, 0x78,
        0xCA, 0xFE, 0xAB, 0xCD, 0x12, 0x34, 0x56, 0x78,
        0xCA, 0xFE, 0xAB, 0xCD, 0x12, 0x34, 0x56, 0x78,
        0xCA, 0xFE, 0xAB, 0xCD, 0x12, 0x34, 0x56, 0x78,

        // CRC (Not passed to serial_send)
        0xBA, 0xDD, 0xF0, 0x0D,
    };
    uint8_t cobs_data[37];
    uint16_t cobs_length;

    // Data is encoded with the CRC.
    cobs_length = cobs_encode(data, sizeof(data), cobs_data);

    // Should calculate CRC.
    crc_seed_Expect();
    crc_calculate_Expect(NULL, 32);
    crc_calculate_IgnoreArg_data();
    crc_get_result_ExpectAndReturn(0xBADDF00D);

    // Should send each byte of data.
    for (int i = 0; i < cobs_length; i ++)
    {
        uart_write_Expect(cobs_data[i]);
    }
    // Should send null packet separator at end.
    uart_write_Expect(0);

    serial_status_t ret;
    // Only send first 32 bytes without CRC (CRC is calculated by the send
    // function)
    ret = serial_send(data, 32);
    TEST_ASSERT_EQUAL_INT(SERIAL_OK, ret);
}

/******************************************************************************
 * Update
 ******************************************************************************/

void test_timer_started_after_non_zero_byte_received(void)
{
    uint8_t byte = 0x01;
    inject_uart_byte(&byte);

    // Expect timer to be restarted, since a non-zero byte was received,
    // indicating that a packet is still in progress.
    timer_reset_Expect();
    timer_start_Expect();

    serial_update();
}

void test_timer_not_restarted_when_no_data_received(void)
{
    uart_read_ExpectAnyArgsAndReturn(0);

    // Note that timer reset/start functions NOT expected.

    serial_update();
}

void test_error_cb_invoked_for_empty_packet(void)
{
    // Send null packet separator with no previous data.
    uint8_t byte = 0x00;
    inject_uart_byte(&byte);

    serial_update();

    assert_error_cb_called_once_with_error(SERIAL_DATA_TOO_SHORT);
}

void test_error_cb_not_invoked_for_no_error(void)
{
    uart_read_ExpectAnyArgsAndReturn(0);
    serial_update();
    TEST_ASSERT_EQUAL_INT(0, m_error_callback_called);
}

void test_error_cb_null(void)
{
    // Send null packet separator with no previous data.
    uint8_t byte = 0x00;
    inject_uart_byte(&byte);

    // Clear error callback
    serial_set_error_callback(NULL);

    // If NULL callback called, will segfault.
    serial_update();
}

void test_cobs_decode_fail(void)
{
    // Invalid COBS encoding
    uint8_t data[] = {0x08, 0x00};
    inject_uart_data(data, sizeof(data));
    assert_error_cb_called_once_with_error(SERIAL_COBS_DECODE_FAIL);
}

void test_data_too_short_for_crc(void)
{
    // First byte is COBS control byte, then four bytes of data, then packet
    // separator.  Data must be at least 4 bytes for CRC plus at least one more
    // byte of data, so this 4-byte data is too short.
    uint8_t data[] = {0x04, 0x01, 0x01, 0x01, 0x01, 0x00};
    inject_uart_data(data, sizeof(data));
    assert_error_cb_called_once_with_error(SERIAL_DATA_TOO_SHORT);
}

void test_packet_crc_check_fail(void)
{
    // Message is just one byte of COBS overhead, followed by one byte of data
    // and four byte CRC.
    uint8_t data[] = {0x05, 0xAA, 0x01, 0x01, 0x01, 0x01, 0x00};

    // Skip the packet separator, sent below.
    inject_uart_data(data, sizeof(data) - 1);

    // Send packet separator to trigger end of packet handling.
    inject_uart_byte(&data[sizeof(data) - 1]);

    // CRC returns 00 00 00 00, data contains 01 01 01 01, check should fail.
    crc_seed_Expect();
    crc_calculate_ExpectAnyArgs();
    crc_get_result_ExpectAndReturn(0x00000000);

    serial_update();

    assert_error_cb_called_once_with_error(SERIAL_CRC_FAIL);
}

void test_packet_timeout(void)
{
    // Just a single byte of non-zero data.  No packet separator is sent.
    uint8_t data[] = {0x01};
    inject_uart_data(data, sizeof(data));

    // No new data available
    uart_read_ExpectAnyArgsAndReturn(0);

    // Inject timeout status
    timer_check_expired_ExpectAndReturn(true);

    serial_update();

    assert_error_cb_called_once_with_error(SERIAL_TIMEOUT);

    // Check that data is discarded and does not get added to next packet.
    uint8_t new_data[] = {0x04, 0x01, 0x01, 0x01, 0x01, 0x00};
    inject_uart_data(new_data, sizeof(new_data));
    assert_error_cb_called_once_with_error(SERIAL_DATA_TOO_SHORT);
}

void test_no_timeout_when_buffer_empty(void)
{
    // No new data available
    uart_read_ExpectAnyArgsAndReturn(0);

    // Inject timeout status
    timer_check_expired_IgnoreAndReturn(true);

    serial_update();

    // Error callback should not be invoked
    TEST_ASSERT_EQUAL_INT(0, m_error_callback_called);
}

void test_packet_too_long(void)
{
    uint16_t max_length =
        COBS_MAX_DATA_LENGTH_AFTER_ENCODE(SERIAL_MAX_RX_LENGTH + 4);

    uint16_t data_length = max_length + 1;
    uint8_t data[data_length];
    for (int i = 0; i < data_length; i ++)
    {
        data[i] = 0x01;
    }
    inject_uart_data(data, data_length);

    assert_error_cb_called_once_with_error(SERIAL_DATA_TOO_LONG);

    // Check that data is discarded and does not get added to next packet.
    // Obviously, if only a few more bytes are added after this, those will be
    // considered a part of the next packet; we just want to make sure the
    // buffer doesn't overflow.
    uint8_t new_data[] = {0x04, 0x01, 0x01, 0x01, 0x01, 0x00};
    inject_uart_data(new_data, sizeof(new_data));
    assert_error_cb_called_once_with_error(SERIAL_DATA_TOO_SHORT);
}

void test_valid_packet(void)
{
    // Message is just one byte of COBS overhead, followed by one byte of data
    // and four byte CRC.
    uint8_t data[] = {0x06, 0xAA, 0x01, 0x02, 0x03, 0x04, 0x00};

    // Skip the packet separator, sent below.
    inject_uart_data(data, sizeof(data) - 1);

    // Send packet separator to trigger end of packet handling.
    inject_uart_byte(&data[sizeof(data) - 1]);

    // CRC returns 01 02 03 04, data contains 01 02 03 04, check should pass.
    crc_seed_Expect();
    crc_calculate_ExpectAnyArgs();
    crc_get_result_ExpectAndReturn(0x01020304);

    serial_update();

    // Check that message callback was called with correct data.
    uint8_t expected_data[] = {0xAA};

    TEST_ASSERT_EQUAL_INT(1, m_msg_data_length);
    TEST_ASSERT_EQUAL_INT(0xAA, m_msg_data_buffer[0]);
}

void test_null_msg_callback(void)
{
    // Message is just one byte of COBS overhead, followed by one byte of data
    // and four byte CRC.
    uint8_t data[] = {0x06, 0xAA, 0x01, 0x02, 0x03, 0x04, 0x00};

    // Skip the packet separator, sent below.
    inject_uart_data(data, sizeof(data) - 1);

    // Send packet separator to trigger end of packet handling.
    inject_uart_byte(&data[sizeof(data) - 1]);

    // CRC returns 01 02 03 04, data contains 01 02 03 04, check should pass.
    crc_seed_Expect();
    crc_calculate_ExpectAnyArgs();
    crc_get_result_ExpectAndReturn(0x01020304);

    serial_set_msg_received_callback(NULL);

    // If NULL callback is invoked, this will segfault.
    serial_update();
}

void test_multiple_packets(void)
{
    // Message is just one byte of COBS overhead, followed by one byte of data
    // and four byte CRC.
    uint8_t data[] = {0x06, 0xAA, 0x01, 0x02, 0x03, 0x04, 0x00};

    // Skip the packet separator, sent below.
    inject_uart_data(data, sizeof(data) - 1);

    // Send packet separator to trigger end of packet handling.
    inject_uart_byte(&data[sizeof(data) - 1]);

    // CRC returns 01 02 03 04, data contains 01 02 03 04, check should pass.
    crc_seed_Expect();
    crc_calculate_ExpectAnyArgs();
    crc_get_result_ExpectAndReturn(0x01020304);

    serial_update();

    TEST_ASSERT_EQUAL_INT(1, m_msg_data_length);
    TEST_ASSERT_EQUAL_INT(0xAA, m_msg_data_buffer[0]);

    // Send the packet a second time.
    uint8_t new_data[] = {0x06, 0xAB, 0x02, 0x03, 0x04, 0x05, 0x00};

    // Skip the packet separator, sent below.
    inject_uart_data(new_data, sizeof(new_data) - 1);

    // Send packet separator to trigger end of packet handling.
    inject_uart_byte(&new_data[sizeof(new_data) - 1]);

    // CRC returns 02 03 04 05, data contains 02 03 04 05, check should pass.
    crc_seed_Expect();
    crc_calculate_ExpectAnyArgs();
    crc_get_result_ExpectAndReturn(0x02030405);

    serial_update();

    TEST_ASSERT_EQUAL_INT(1, m_msg_data_length);
    TEST_ASSERT_EQUAL_INT(0xAB, m_msg_data_buffer[0]);
}

/******************************************************************************
 * Callback Functions
 ******************************************************************************/

static void msg_callback(const uint8_t *data, uint16_t length)
{
    m_msg_data_length = length;
    memcpy(m_msg_data_buffer, data, length);
}

static void error_callback(serial_status_t error)
{
    m_error = error;
    m_error_callback_called ++;
}

/******************************************************************************
 * Helper Functions
 ******************************************************************************/

static void inject_uart_data(uint8_t *data, int length)
{
    for (int i = 0; i < length; i ++)
    {
        inject_uart_byte(&data[i]);
        if (0 != data[i])
        {
            // Expect timer to be reset for each non-zero byte.
            timer_reset_Expect();
            timer_start_Expect();
        }
        serial_update();
    }
}

static void inject_uart_byte(uint8_t *byte)
{
    // read function returns 1 when byte received.
    uart_read_ExpectAnyArgsAndReturn(1);

    // Return data byte to caller.
    uart_read_ReturnThruPtr_rx_data(byte);
}

static void assert_error_cb_called_once_with_error(serial_status_t error)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(
            1,
            m_error_callback_called,
            "Error callback called wrong number of times.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(
            error, m_error, "Incorrect error code.");
    m_error_callback_called = 0;
    m_error = SERIAL_OK;
}
