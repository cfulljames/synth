#include "messages.h"
#include "serial.h"
#include "flash.h"
#include "version.h"
#include <stddef.h>

#define MSG_TYPE_INDEX (0)
#define MSG_RESP_INDEX (1)
#define DATA_INFO_MSG_LENGTH (24)

// Number of bits to shift the upper nibble of a byte into the lower nibble.
#define UPPER_NIBBLE_SHIFT 4U

// Bitmask for isolating the lower nibble of a byte.
#define LOWER_NIBBLE_MASK 0x0F

// Number of bits to shift the middle byte of a flash word into the lower byte.
#define MIDDLE_BYTE_SHIFT 8U

// Number of bits to shift the upper byte of a flash word into the lower byte.
#define UPPER_BYTE_SHIFT 16U

// Bitmask for isolating the lower byte of a word.
#define LOWER_BYTE_MASK 0x00FF

// Number of 16-bit words per instruction space in flash.
#define WORDS_PER_INSTRUCTION 2U

// Number of instructions used to store the UDID.
#define UDID_LENGTH 5U

// The address offset of the least-significant double-word of the UDID.
#define UDID_FIRST_ADDRESS 0x801200

// The address of the most-significant double-word of the UDID.
#define UDID_LAST_ADDRESS \
    (UDID_FIRST_ADDRESS + (WORDS_PER_INSTRUCTION * (UDID_LENGTH - 1U)))

// Number of instructions used to store the application version.
#define APP_VERSION_LENGTH 2U

// The address of the most significant double-word of the application version.
#define APP_VERSION_LAST_ADDRESS \
    (APP_VERSION_FIRST_ADDRESS + \
    (WORDS_PER_INSTRUCTION * (APP_VERSION_LENGTH - 1U)))

static void on_serial_error(serial_status_t error);
static void on_serial_msg_received(const uint8_t *data, uint16_t length);
static void send_response(msg_response_t response);
static void handle_device_info_request(const uint8_t *data, uint16_t length);
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

#ifdef TEST
// Address defined in test code.
extern const uint32_t APP_VERSION_FIRST_ADDRESS;
#else
// Address defined in linker script.
__prog__ __attribute__((space(prog)))
extern uint8_t _APP_VERSION;
const uint32_t APP_VERSION_FIRST_ADDRESS = ((uint32_t)&_APP_VERSION);
#endif

void messages_init(void)
{
    serial_set_error_callback(on_serial_error);
    serial_set_msg_received_callback(on_serial_msg_received);
}

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

static void on_serial_msg_received(const uint8_t *data, uint16_t length)
{
    switch (data[MSG_TYPE_INDEX])
    {
        case MESSAGE_TYPE_DEVICE_INFO_REQUEST:
            handle_device_info_request(data, length);
            break;
        default:
            send_response(MESSAGE_RESP_INVALID_TYPE);
            break;
    }
}

static void send_response(msg_response_t response)
{
    uint8_t resp_msg[] = {MESSAGE_TYPE_CMD_RESULT, (uint8_t)response};
    serial_send(resp_msg, sizeof(resp_msg));
}

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
