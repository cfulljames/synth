#include "app_status.h"
#include "crc.h"
#include "flash.h"
#include "messages.h"
#include "serial.h"
#include "version.h"
#include <stddef.h>
#include <string.h>

// Number of bits to shift the upper nibble of a byte into the lower nibble.
#define UPPER_NIBBLE_SHIFT (4U)

// Bitmask for isolating the lower nibble of a byte.
#define LOWER_NIBBLE_MASK (0x0FU)

// Number of bits to shift the middle byte of a flash word into the lower byte.
#define MIDDLE_BYTE_SHIFT (8U)

// Number of bits to shift the upper byte of a flash word into the lower byte.
#define UPPER_BYTE_SHIFT (16U)

// Bitmask for isolating the lower byte of a word.
#define LOWER_BYTE_MASK (0x00FFU)

// Number of 16-bit words per instruction space in flash.
#define WORDS_PER_INSTRUCTION (2U)

// Number of instructions used to store the UDID.
#define UDID_LENGTH (5U)

// The address offset of the least-significant double-word of the UDID.
#define UDID_FIRST_ADDRESS (0x801200UL)

// The address of the most-significant double-word of the UDID.
#define UDID_LAST_ADDRESS \
    (UDID_FIRST_ADDRESS + (WORDS_PER_INSTRUCTION * (UDID_LENGTH - 1U)))

// Number of instructions used to store the application version.
#define APP_VERSION_LENGTH (2U)

// The address of the most significant double-word of the application version.
#define APP_VERSION_LAST_ADDRESS \
    (APP_VERSION_FIRST_ADDRESS + \
    (WORDS_PER_INSTRUCTION * (APP_VERSION_LENGTH - 1U)))

// Index of the message type in all messages.
#define MSG_TYPE_INDEX (0U)

// Index of the response code in the response message.
#define MSG_RESP_INDEX (1U)

// Length of the data info message.
#define DATA_INFO_MSG_LENGTH (24U)

// Length of the erase command message.
#define ERASE_MESSAGE_LENGTH (9U)

// Byte index of the start address in the erase command message.
#define ERASE_START_ADDR_INDEX (1U)

// Byte index of the end address in the erase command message.
#define ERASE_END_ADDR_INDEX (5U)

// Length of the verify command message.
#define VERIFY_MSG_LENGTH (13U)

// Byte index of the start address in the verify command message.
#define VERIFY_START_ADDR_INDEX (1U)

// Byte index of the end address in the verify command message.
#define VERIFY_END_ADDR_INDEX (5U)

// Index of the CRC in the verify command message.
#define VERIFY_CRC_INDEX (9U)

// Length of the double word write command message.
#define WRITE_DWORD_MSG_LENGTH (13U)

// Index of the start address in the double word write command message.
#define WRITE_DWORD_START_ADDR_INDEX (1U)

// Index of the low data word in the double word write command message.
#define WRITE_DWORD_DATA_L_INDEX (5U)

// Index of the high data word in the double word write command message.
#define WRITE_DWORD_DATA_H_INDEX (9U)

// Length of the row write command message.
#define WRITE_ROW_MSG_LENGTH (517U)

// Index of the start address in the row write command message.
#define WRITE_ROW_START_ADDR_INDEX (1U)

// Index of the first data byte in the row write command message.
#define WRITE_ROW_DATA_START_INDEX (5U)

static void on_serial_error(serial_status_t error);
static void on_serial_msg_received(const uint8_t *data, uint16_t length);
static void send_response(msg_response_t response);
static void handle_device_info_request(const uint8_t *data, uint16_t length);
static void handle_erase(const uint8_t *data, uint16_t length);
static void handle_verify(const uint8_t *data, uint16_t length);
static void handle_write_dword(const uint8_t *data, uint16_t length);
static void handle_write_row(const uint8_t *data, uint16_t length);
static void handle_run(const uint8_t *data, uint16_t length);
static flash_status_t read_serial_number(uint8_t *msg, uint8_t *index);
static flash_status_t read_application_version(uint8_t *msg, uint8_t *index);
static uint32_t unpack_long(const uint8_t *data);
static flash_status_t calculate_flash_crc(
        uint32_t start_address, uint32_t end_address, uint32_t *crc);

struct serial_error_response_s {
    serial_status_t serial_status;
    msg_response_t msg_response;
};

struct serial_error_response_s error_responses[] = {
    {SERIAL_TIMEOUT,            MESSAGE_RESP_SERIAL_TIMEOUT},
    {SERIAL_CRC_FAIL,           MESSAGE_RESP_SERIAL_CRC_FAIL},
    {SERIAL_COBS_DECODE_FAIL,   MESSAGE_RESP_SERIAL_COBS_DECODE_FAIL},
    {SERIAL_DATA_TOO_LONG,      MESSAGE_RESP_SERIAL_PACKET_TOO_LONG},
    {SERIAL_DATA_TOO_SHORT,     MESSAGE_RESP_SERIAL_PACKET_TOO_SHORT},
};

#define ERROR_RESPONSES_LENGTH ( \
        sizeof(error_responses) / sizeof(struct serial_error_response_s))

/******************************************************************************
 * Interface Functions
 ******************************************************************************/

void messages_init(void)
{
    serial_set_error_callback(on_serial_error);
    serial_set_msg_received_callback(on_serial_msg_received);
}

/******************************************************************************
 * Serial Callbacks
 ******************************************************************************/

/*
 * Callback invoked when a serial error occurs.
 *
 * Sends a new response message containing the corresponding error code.
 */
static void on_serial_error(serial_status_t error)
{
    msg_response_t response = MESSAGE_RESP_OK;

    for (uint8_t i = 0; i < ERROR_RESPONSES_LENGTH; i ++)
    {
        struct serial_error_response_s *map = &error_responses[i];
        if (map->serial_status == error)
        {
            response = map->msg_response;
            break;
        }
    }

    if (MESSAGE_RESP_OK != response)
    {
        send_response(response);
    }
}

/*
 * Callback invoked when a complete message packet is received via serial.
 *
 * This will parse the message, perform any requested actions, and send the
 * appropriate response.  This is where most of the application logic happens.
 */
static void on_serial_msg_received(const uint8_t *data, uint16_t length)
{
    if (length == 0)
    {
        send_response(MESSAGE_RESP_MESSAGE_TOO_SHORT);
        return;
    }

    switch (data[MSG_TYPE_INDEX])
    {
        case MESSAGE_TYPE_DEVICE_INFO_REQUEST:
            handle_device_info_request(data, length);
            break;
        case MESSAGE_TYPE_ERASE:
            handle_erase(data, length);
            break;
        case MESSAGE_TYPE_VERIFY:
            handle_verify(data, length);
            break;
        case MESSAGE_TYPE_WRITE_DWORD:
            handle_write_dword(data, length);
            break;
        case MESSAGE_TYPE_WRITE_ROW:
            handle_write_row(data, length);
            break;
        case MESSAGE_TYPE_RUN:
            handle_run(data, length);
            break;
        default:
            send_response(MESSAGE_RESP_INVALID_TYPE);
            break;
    }
}

/******************************************************************************
 * Message Handlers
 ******************************************************************************/

/*
 * Handle the device info request message.
 *
 * This will read the device info (serial number, bootloader version, app
 * version) and send them in response.
 */
static void handle_device_info_request(const uint8_t *data, uint16_t length)
{
    if (length > 1)
    {
        send_response(MESSAGE_RESP_MESSAGE_TOO_LONG);
    }
    else
    {
        uint8_t msg[DATA_INFO_MSG_LENGTH];
        uint8_t index = 0;

        msg[index++] = MESSAGE_TYPE_DEVICE_INFO;

        read_serial_number(msg, &index);

        // Bootloader version.
        msg[index++] = bootloader_version.major;
        msg[index++] = bootloader_version.minor;
        msg[index++] = (bootloader_version.patch >> MIDDLE_BYTE_SHIFT) & LOWER_BYTE_MASK;
        msg[index++] = bootloader_version.patch & LOWER_BYTE_MASK;

        // Application version.
        read_application_version(msg, &index);

        serial_send(msg, sizeof(msg));
    }
}

/*
 * Handle the erase message.
 *
 * This will check the start/end addresses in the command message, and erase the
 * corresponding region of flash.
 */
static void handle_erase(const uint8_t *data, uint16_t length)
{
    if (length > ERASE_MESSAGE_LENGTH)
    {
        // Extra trailing bytes in the message.
        send_response(MESSAGE_RESP_MESSAGE_TOO_LONG);
        return;
    }

    if (length < ERASE_MESSAGE_LENGTH)
    {
        // Not enough bytes to decode addresses.
        send_response(MESSAGE_RESP_MESSAGE_TOO_SHORT);
        return;
    }

    // Got correct number of bytes for the message.
    uint32_t start_address = unpack_long(&data[ERASE_START_ADDR_INDEX]);
    uint32_t end_address = unpack_long(&data[ERASE_END_ADDR_INDEX]);

    if ((start_address % PAGE_SIZE != 0) || (end_address % PAGE_SIZE != 0))
    {
        // One of the addresses does not align with a page boundary.
        send_response(MESSAGE_RESP_ADDRESS_BAD_ALIGNMENT);
        return;
    }
    else if (start_address < APP_PARTITION_FIRST_ADDRESS
            || end_address > CONFIGURATION_PAGE_FIRST_ADDRESS
            || start_address >= end_address)
    {
        // At least one of the addresses is outside the range of erasable flash.
        send_response(MESSAGE_RESP_ADDRESS_OUT_OF_RANGE);
        return;
    }

    // Addresses are valid; erase the flash.
    uint32_t page_start;
    flash_status_t status;
    for (page_start = start_address;
            page_start < end_address;
            page_start += PAGE_SIZE)
    {
        status = flash_erase_page(page_start);
        if (status != FLASH_OK)
        {
            send_response(MESSAGE_RESP_INTERNAL_ERROR);
            return;
        }
    }

    send_response(MESSAGE_RESP_OK);
}

/*
 * Handle a verify command message.
 *
 * Read the requested section of flash, calculate its CRC, and compare it to the
 * given value.
 */
static void handle_verify(const uint8_t *data, uint16_t length)
{
    if (length > VERIFY_MSG_LENGTH)
    {
        send_response(MESSAGE_RESP_MESSAGE_TOO_LONG);
        return;
    }
    else if (length < VERIFY_MSG_LENGTH)
    {
        send_response(MESSAGE_RESP_MESSAGE_TOO_SHORT);
        return;
    }

    // Got correct number of bytes for the message.
    uint32_t start_address = unpack_long(&data[ERASE_START_ADDR_INDEX]);
    uint32_t end_address = unpack_long(&data[ERASE_END_ADDR_INDEX]);

    if ((start_address % INSTR_WORD_SIZE != 0)
            || (end_address % INSTR_WORD_SIZE != 0))
    {
        // At least one address is not word-aligned.
        send_response(MESSAGE_RESP_ADDRESS_BAD_ALIGNMENT);
        return;
    }
    else if (end_address > (MAX_READ_ADDRESS + INSTR_WORD_SIZE)
            || start_address >= end_address)
    {
        // At least one address is out of range.
        send_response(MESSAGE_RESP_ADDRESS_OUT_OF_RANGE);
        return;
    }

    // Addresses are valid; read the flash and calculate the CRC.
    uint32_t expected_crc = unpack_long(&data[VERIFY_CRC_INDEX]);
    flash_status_t status;
    uint32_t actual_crc;
    status = calculate_flash_crc(start_address, end_address, &actual_crc);

    if (status != FLASH_OK)
    {
        send_response(MESSAGE_RESP_INTERNAL_ERROR);
    }
    else if (actual_crc == expected_crc)
    {
        send_response(MESSAGE_RESP_OK);
    }
    else
    {
        send_response(MESSAGE_RESP_VERIFICATION_FAIL);
    }
}

/*
 * Handle a write double word message.
 *
 * Write the given double instruction word to flash.
 */
static void handle_write_dword(const uint8_t *data, uint16_t length)
{
    if (length > WRITE_DWORD_MSG_LENGTH)
    {
        send_response(MESSAGE_RESP_MESSAGE_TOO_LONG);
        return;
    }
    else if (length < WRITE_DWORD_MSG_LENGTH)
    {
        send_response(MESSAGE_RESP_MESSAGE_TOO_SHORT);
        return;
    }

    // Unpack data from the message.
    uint32_t start_address = unpack_long(&data[WRITE_DWORD_START_ADDR_INDEX]);
    uint32_t words[2];
    memcpy(words, &data[WRITE_DWORD_DATA_L_INDEX], sizeof(words));

    if (start_address % DOUBLE_WORD_SIZE != 0)
    {
        // Address not double-word aligned.
        send_response(MESSAGE_RESP_ADDRESS_BAD_ALIGNMENT);
        return;
    }
    else if (start_address >= USER_FLASH_ADDRESS_END
            || start_address < APP_PARTITION_FIRST_ADDRESS)
    {
        // Address is not within application flash range.
        send_response(MESSAGE_RESP_ADDRESS_OUT_OF_RANGE);
        return;
    }

    // Start address is valid.  Write words to flash.
    flash_status_t status = flash_write_dword(start_address, words[0], words[1]);

    if (status == FLASH_OK)
    {
        send_response(MESSAGE_RESP_OK);
    }
    else
    {
        // Flash error occurred while writing.
        send_response(MESSAGE_RESP_INTERNAL_ERROR);
    }
}

/*
 * Handle a write row message.
 *
 * Write the given row of data to flash.
 */
static void handle_write_row(const uint8_t *data, uint16_t length)
{
    if (length > WRITE_ROW_MSG_LENGTH)
    {
        send_response(MESSAGE_RESP_MESSAGE_TOO_LONG);
        return;
    }
    else if (length < WRITE_ROW_MSG_LENGTH)
    {
        send_response(MESSAGE_RESP_MESSAGE_TOO_SHORT);
        return;
    }

    // Unpack data from the message.
    uint32_t start_address = unpack_long(&data[WRITE_ROW_START_ADDR_INDEX]);

    if (start_address % ROW_SIZE != 0)
    {
        // Address not double-word aligned.
        send_response(MESSAGE_RESP_ADDRESS_BAD_ALIGNMENT);
        return;
    }
    else if (start_address >= USER_FLASH_ADDRESS_END
            || start_address < APP_PARTITION_FIRST_ADDRESS)
    {
        // Address is not within application flash range.
        send_response(MESSAGE_RESP_ADDRESS_OUT_OF_RANGE);
        return;
    }

    // Copy data into new 4-byte-aligned array.
    uint32_t data_row[WORDS_PER_ROW];
    memcpy(data_row, &data[WRITE_ROW_DATA_START_INDEX], sizeof(data_row));

    // Start address is valid.  Write words to flash.
    flash_status_t status = flash_write_row(start_address, data_row);

    if (status == FLASH_OK)
    {
        send_response(MESSAGE_RESP_OK);
    }
    else
    {
        // Flash error occurred while writing.
        send_response(MESSAGE_RESP_INTERNAL_ERROR);
    }
}

static void handle_run(const uint8_t *data, uint16_t length)
{
    if (length > 1)
    {
        send_response(MESSAGE_RESP_MESSAGE_TOO_LONG);
        return;
    }

    // Set request to run application.  This will be checked in the main loop in
    // main.c
    app_status_t actual_status = app_status_request(APP_STATUS_RUN);

    if (actual_status == APP_STATUS_RUN)
    {
        send_response(MESSAGE_RESP_OK);
    }
    else
    {
        send_response(MESSAGE_RESP_INTERNAL_ERROR);
    }
}

/******************************************************************************
 * Helper Functions
 ******************************************************************************/

/*
 * Send a response message.
 *
 * This should be called exactly once for each message that is received, except
 * in the case of device info, which has a separate response.
 */
static void send_response(msg_response_t response)
{
    uint8_t resp_msg[] = {MESSAGE_TYPE_CMD_RESULT, (uint8_t)response};
    serial_send(resp_msg, sizeof(resp_msg));
}


/*
 * Read the device serial number from flash.
 *
 * The serial number will be read into the buffer provided, and the index will
 * be incremented accordingly.
 */
static flash_status_t read_serial_number(uint8_t *msg, uint8_t *index)
{
    uint32_t address;
    uint32_t data_word;
    flash_status_t ret = FLASH_OK;

    for (address = UDID_LAST_ADDRESS;
            address >= UDID_FIRST_ADDRESS;
            address -= WORDS_PER_INSTRUCTION)
    {
        ret = flash_read_word(address, &data_word);
        msg[(*index)++] = (data_word >> UPPER_BYTE_SHIFT) & LOWER_BYTE_MASK;
        msg[(*index)++] = (data_word >> MIDDLE_BYTE_SHIFT) & LOWER_BYTE_MASK;
        msg[(*index)++] = data_word & LOWER_BYTE_MASK;
    }

    return ret;
}

/*
 * Read the application version from flash.
 *
 * The app version is stored in a pre-defined location in flash to allow it to
 * be accessed from the bootloader.  It will be read into the buffer provided,
 * and the index will be incremented accordingly.
 */
static flash_status_t read_application_version(uint8_t *msg, uint8_t *index)
{
    uint32_t address;
    uint32_t data_word;
    flash_status_t ret = FLASH_OK;

    for (address = APP_VERSION_FIRST_ADDRESS;
            address <= APP_VERSION_LAST_ADDRESS;
            address += WORDS_PER_INSTRUCTION)
    {
        ret = flash_read_word(address, &data_word);
        msg[(*index)++] = (data_word >> MIDDLE_BYTE_SHIFT) & LOWER_BYTE_MASK;
        msg[(*index)++] = data_word & LOWER_BYTE_MASK;
    }

    return ret;
}

/*
 * Calculate the CRC32 for a region of flash memory.
 */
static flash_status_t calculate_flash_crc(
        uint32_t start_address, uint32_t end_address, uint32_t *crc)
{
    flash_status_t status;
    crc_seed();
    uint32_t addr;
    for (addr = start_address; addr < end_address; addr += INSTR_WORD_SIZE)
    {
        // Read the next word from flash.
        uint32_t flash_word;
        status = flash_read_word(addr, &flash_word);

        if (status != FLASH_OK)
        {
            // Flash failure occurred; stop reading.
            return status;
        }

        // Add this word to the CRC calculation.
        crc_calculate((uint8_t*)&flash_word, sizeof(uint32_t));
    }

    // Get the CRC result and compare it to the expected value from the
    // message.
    *crc = crc_get_result();

    return status;
}

/*
 * Unpack an unsigned long integer from a message data byte array.
 *
 * This can be used for addresses, CRCs, etc.  Most data in the bootloader
 * protocol is sent as 32-bit unsigned integers.
 */
static uint32_t unpack_long(const uint8_t *data)
{
    uint32_t result = 0;
    result |= (uint32_t)data[0] << 24;
    result |= (uint32_t)data[1] << 16;
    result |= (uint32_t)data[2] << 8;
    result |= (uint32_t)data[3] << 0;
    return result;
}
