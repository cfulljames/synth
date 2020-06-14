#include "unity.h"

#include "messages.h"
#include "mock_serial.h"
#include "mock_flash.h"
#include "mock_crc.h"
#include "version.h"

#include <string.h>

static void fake_serial_set_error_callback(serial_error_cb_t cb, int num_calls);
static void fake_serial_set_msg_received_callback(
        serial_msg_cb_t cb, int num_calls);
static serial_status_t fake_serial_send(
        const uint8_t *data, uint16_t length, int numCalls);

serial_error_cb_t m_error_cb;
serial_msg_cb_t m_msg_cb;
uint8_t m_send_data[1024];
uint16_t m_send_length;

// The actual definition for this valus is in the linker script, but we can't
// use that for testing when building for the host platform.
uint32_t APP_VERSION_FIRST_ADDRESS = 0x015EFC;

#define RECEIVE_MESSAGE(m) m_msg_cb((m), sizeof(m))

#define TEST_ASSERT_SENT(d) { \
        TEST_ASSERT_EQUAL_INT_MESSAGE( \
                sizeof(d), m_send_length, "Incorrect response length."); \
        TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE( \
                (d), m_send_data, m_send_length, \
                "Incorrect response contents."); \
        }

void setUp(void)
{
    m_error_cb = NULL;
    m_msg_cb = NULL;
    memset(m_send_data, 0, sizeof(m_send_data));
    m_send_length = 0;

    // Replace serial functions with fake versions.  This allows us to get the
    // data passed to these functions and verify it using custom logic.
    serial_set_error_callback_StubWithCallback(fake_serial_set_error_callback);
    serial_set_msg_received_callback_StubWithCallback(
            fake_serial_set_msg_received_callback);
    serial_send_StubWithCallback(fake_serial_send);

    messages_init();
}

void tearDown(void)
{
}

/******************************************************************************
 * Initialization
 ******************************************************************************/

void test_init_sets_callbacks(void)
{
    TEST_ASSERT_NOT_NULL(m_error_cb);
    TEST_ASSERT_NOT_NULL(m_msg_cb);
}

/******************************************************************************
 * Serial Protocol Errors
 ******************************************************************************/

void test_serial_timeout(void)
{
    uint8_t expected[] = {
        MESSAGE_TYPE_CMD_RESULT,
        MESSAGE_RESP_SERIAL_TIMEOUT,
    };
    m_error_cb(SERIAL_TIMEOUT);
    TEST_ASSERT_SENT(expected);
}

void test_serial_crc_fail(void)
{
    uint8_t expected[] = {
        MESSAGE_TYPE_CMD_RESULT,
        MESSAGE_RESP_SERIAL_CRC_FAIL,
    };
    m_error_cb(SERIAL_CRC_FAIL);
    TEST_ASSERT_SENT(expected);
}

void test_serial_cobs_decode_fail(void)
{
    uint8_t expected[] = {
        MESSAGE_TYPE_CMD_RESULT,
        MESSAGE_RESP_SERIAL_COBS_DECODE_FAIL,
    };
    m_error_cb(SERIAL_COBS_DECODE_FAIL);
    TEST_ASSERT_SENT(expected);
}

void test_serial_packet_too_long(void)
{
    uint8_t expected[] = {
        MESSAGE_TYPE_CMD_RESULT,
        MESSAGE_RESP_SERIAL_PACKET_TOO_LONG,
    };
    m_error_cb(SERIAL_DATA_TOO_LONG);
    TEST_ASSERT_SENT(expected);
}

void test_serial_packet_too_short(void)
{
    uint8_t expected[] = {
        MESSAGE_TYPE_CMD_RESULT,
        MESSAGE_RESP_SERIAL_PACKET_TOO_SHORT,
    };
    m_error_cb(SERIAL_DATA_TOO_SHORT);
    TEST_ASSERT_SENT(expected);
}

void test_serial_unused_error_statuses(void)
{
    m_error_cb(SERIAL_OK);
    TEST_ASSERT_EQUAL_INT(0, m_send_length);

    m_error_cb(SERIAL_DATA_NULL);
    TEST_ASSERT_EQUAL_INT(0, m_send_length);
}

/******************************************************************************
 * General Message Errors
 ******************************************************************************/

void test_invalid_message_type(void)
{
    uint8_t message[] = {
        0xFF,
    };
    uint8_t expected[] = {
        MESSAGE_TYPE_CMD_RESULT,
        MESSAGE_RESP_INVALID_TYPE,
    };
    RECEIVE_MESSAGE(message);
    TEST_ASSERT_SENT(expected);
}

/******************************************************************************
 * Device Info
 ******************************************************************************/

void test_device_info_bad_msg_length(void)
{
    uint8_t message[] = {
        MESSAGE_TYPE_DEVICE_INFO_REQUEST,
        0xAB, // Extra byte
    };
    uint8_t expected[] = {
        MESSAGE_TYPE_CMD_RESULT,
        MESSAGE_RESP_MESSAGE_TOO_LONG,
    };
    RECEIVE_MESSAGE(message);
    TEST_ASSERT_SENT(expected);
}

void test_device_info_success(void)
{
    uint8_t message[] = {
        MESSAGE_TYPE_DEVICE_INFO_REQUEST,
    };

    uint32_t serial_number[] = {
        0x00DACAFE,
        0x00BEFACE,
        0x00CADCAD,
        0x00B0DEAC,
        0x00AAAAAA,
    };

    // Expect serial number is read from flash.
    const uint32_t SERIAL_NUM_LAST_ADDR = 0x801208;
    for (int i = 0; i < 5; i ++)
    {
        uint32_t address = SERIAL_NUM_LAST_ADDR - (2 * i);
        flash_read_word_ExpectAndReturn(address, NULL, FLASH_OK);
        flash_read_word_IgnoreArg_data();
        flash_read_word_ReturnThruPtr_data(&serial_number[i]);
    }

    // Expect application version is read from flash.
    uint32_t application_version[] = {
        0x0000ABCD,
        0x0000DEAF,
    };
    for (int i = 0; i < 2; i ++)
    {
        flash_read_word_ExpectAndReturn(
                APP_VERSION_FIRST_ADDRESS + (2 * i), NULL, FLASH_OK);
        flash_read_word_IgnoreArg_data();
        flash_read_word_ReturnThruPtr_data(&application_version[i]);
    }

    uint8_t expected[] = {
        MESSAGE_TYPE_DEVICE_INFO,

        // Serial number.
        0xDA, 0xCA, 0xFE,
        0xBE, 0xFA, 0xCE,
        0xCA, 0xDC, 0xAD,
        0xB0, 0xDE, 0xAC,
        0xAA, 0xAA, 0xAA,

        // Bootloader Version (global constant).
        bootloader_version.major,
        bootloader_version.minor,
        (bootloader_version.patch & 0xFF00) >> 8,
        (bootloader_version.patch & 0x00FF) >> 0,

        // Application version.
        0xAB, 0xCD, 0xDE, 0xAF,
    };
    RECEIVE_MESSAGE(message);
    TEST_ASSERT_SENT(expected);
}

/*
 * request device info serial flash error
 * request device info app version flash error
 */

/******************************************************************************
 * Fake Serial Functions
 ******************************************************************************/

// These functions are used to get pointers to the static callback functions in
// the messages module.

static void fake_serial_set_error_callback(serial_error_cb_t cb, int num_calls)
{
    m_error_cb = cb;
}

static void fake_serial_set_msg_received_callback(
        serial_msg_cb_t cb, int num_calls)
{
    m_msg_cb = cb;
}

static serial_status_t fake_serial_send(
        const uint8_t *data, uint16_t length, int numCalls)
{
    memcpy(m_send_data, data, length);
    m_send_length = length;
}



/*
 * FUNCTIONS
 * messages_init
 *
 * TESTS
 *
 * empty message (length = 0)
 *
 * erase bad message length
 * erase start address not aligned
 * erase end address not aligned
 * erase start address out of range
 * erase end address out of range
 * erase flash fail
 * erase success
 *
 * write bad message length
 * write bad data length
 * write start address not aligned
 * write end address not aligned
 * write start address out of range
 * write end address out of range
 * write flash fail
 * write success
 *
 * verify bad message length
 * verify start address not aligned
 * verify end address not aligned
 * verify start address out of range
 * verify end address out of range
 * verify fail
 * verify success
 *
 * run bad message length
 * run app not ready
 * run success
 *
 * request device info bad message length
 * request device info success
 */
