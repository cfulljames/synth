#include "messages.h"
#include "serial.h"
#include "flash.h"
#include "version.h"
#include <stddef.h>

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


static void on_serial_error(serial_status_t error);
static void on_serial_msg_received(const uint8_t *data, uint16_t length);
static void send_response(msg_response_t response);
static void handle_device_info_request(const uint8_t *data, uint16_t length);
static void handle_erase(const uint8_t *data, uint16_t length);
static flash_status_t read_serial_number(uint8_t *msg, uint8_t *index);
static flash_status_t read_application_version(uint8_t *msg, uint8_t *index);

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
 * This will read the deivce info (serial number, bootloader version, app
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

    // Got correct number of bytes for the erase message.
    uint32_t start_address = 0;
    start_address |= (uint32_t)data[ERASE_START_ADDR_INDEX + 0] << 24;
    start_address |= (uint32_t)data[ERASE_START_ADDR_INDEX + 1] << 16;
    start_address |= (uint32_t)data[ERASE_START_ADDR_INDEX + 2] << 8;
    start_address |= (uint32_t)data[ERASE_START_ADDR_INDEX + 3] << 0;

    uint32_t end_address = 0;
    end_address |= (uint32_t)data[ERASE_END_ADDR_INDEX + 0] << 24;
    end_address |= (uint32_t)data[ERASE_END_ADDR_INDEX + 1] << 16;
    end_address |= (uint32_t)data[ERASE_END_ADDR_INDEX + 2] << 8;
    end_address |= (uint32_t)data[ERASE_END_ADDR_INDEX + 3] << 0;

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
