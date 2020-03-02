#include "unity.h"
#include "cmd_parser.h"
#include "mock_msi.h"
#include "mock_cmd_callback.h"
#include "common_cmd.h"

static void send_command(
        uint16_t type,
        uint16_t channel,
        uint16_t command,
        uint32_t data);
static void format_command(
        uint16_t type,
        uint16_t channel,
        uint16_t command,
        uint32_t data,
        common_cmd_t *buffer);
static void send_word(uint16_t *word);

void setUp(void)
{
    cmd_parser_init();
}

void tearDown(void)
{
}

void test_simple_data(void)
{
    cmd_parser_set_callback(fake_cmd_callback);
    send_command(0, 0, 1, 0);
}

void test_max_values(void)
{
    cmd_parser_set_callback(fake_cmd_callback);
    send_command(0x7, 0xF, 0x3F, 0xFFFFFFFF);
}

void test_command_without_data_ignored(void)
{
    cmd_parser_set_callback(fake_cmd_callback);
    // Send a command word without data words.
    uint16_t cmd_word = 0x0F;
    send_word(&cmd_word);

    // Send a full command.  The previous word should be ignored, and this
    // command should be processed as normal.
    send_command(2, 3, 5, 300);
}

void test_command_missing_one_data_word_ignored(void)
{
    cmd_parser_set_callback(fake_cmd_callback);
    // Send a command word with one data word.
    uint16_t cmd_word = 0x0F;
    send_word(&cmd_word);

    uint16_t data_word = 0x80;
    send_word(&data_word);

    // Send a full command.  The previous word should be ignored, and this
    // command should be processed as normal.
    send_command(0, 0, 1, 0xBEADCAFE);
}

void test_data_before_command(void)
{
    cmd_parser_set_callback(fake_cmd_callback);
    // Send only data with no command at start.
    uint16_t data_word = 0xFF;
    send_word(&data_word);

    // Send a full command.  The previous word should be ignored, and this
    // command should be processed as normal.
    send_command(2, 4, 6, 0xBEADCAFE);
}

void test_send_over_multiple_updates(void)
{
    cmd_parser_set_callback(fake_cmd_callback);
    common_cmd_t cmd[COMMON_CMD_NUM_WORDS];
    format_command(3, 6, 54, 0xAAC0FFEE, cmd);

    send_word(&cmd[0].value);
    msi_get_fifo_data_ExpectAnyArgsAndReturn(false);
    cmd_parser_update();

    send_word(&cmd[1].value);
    msi_get_fifo_data_ExpectAnyArgsAndReturn(false);
    cmd_parser_update();

    send_word(&cmd[2].value);

    // Parser should invoke the callback after parsing the command.
    fake_cmd_callback_Expect(3, 6, 54, 0xAAC0FFEE);

    msi_get_fifo_data_ExpectAnyArgsAndReturn(false);

    cmd_parser_update();
}

void test_update_with_no_callback(void)
{
    common_cmd_t cmd[COMMON_CMD_NUM_WORDS];
    format_command(3, 6, 54, 0xAAC0FFEE, cmd);

    for (int i = 0; i < COMMON_CMD_NUM_WORDS; i ++)
    {
        send_word(&cmd[i].value);
    }
    msi_get_fifo_data_ExpectAnyArgsAndReturn(false);
    // No callback to invoke; should not try to dereference null pointer.
    cmd_parser_update();
}

void test_multiple_commands_in_one_update(void)
{
    cmd_parser_set_callback(fake_cmd_callback);
    common_cmd_t cmd[COMMON_CMD_NUM_WORDS];
    format_command(3, 6, 54, 0xAAC0FFEE, cmd);

    for (int i = 0; i < COMMON_CMD_NUM_WORDS; i ++)
    {
        send_word(&cmd[i].value);
    }
    fake_cmd_callback_Expect(3, 6, 54, 0xAAC0FFEE);

    for (int i = 0; i < COMMON_CMD_NUM_WORDS; i ++)
    {
        send_word(&cmd[i].value);
    }
    fake_cmd_callback_Expect(3, 6, 54, 0xAAC0FFEE);

    msi_get_fifo_data_ExpectAnyArgsAndReturn(false);

    cmd_parser_update();
}

/******************************************************************************
 * Static Helper Functions
 ******************************************************************************/

static void send_command(
        uint16_t type, uint16_t channel, uint16_t command, uint32_t data)
{
    common_cmd_t cmd[COMMON_CMD_NUM_WORDS];

    format_command(type, channel, command, data, cmd);

    for (int i = 0; i < COMMON_CMD_NUM_WORDS; i ++)
    {
        send_word(&cmd[i].value);
    }

    // Parser should invoke the callback after parsing the command.
    fake_cmd_callback_Expect(type, channel, command, data);

    msi_get_fifo_data_ExpectAnyArgsAndReturn(false);

    cmd_parser_update();
}

static void format_command(
        uint16_t type, uint16_t channel, uint16_t command, uint32_t data,
        common_cmd_t *buffer)
{
    // First word contains the actual command and first two bits of data.
    buffer[0].word_type = COMMON_CMD_WORD_TYPE_CMD;
    buffer[0].type = type;
    buffer[0].channel = channel;
    buffer[0].command = command;
    buffer[0].data_msb = (data >> 30);

    // Next two words each contain 15 bits of data.
    buffer[1].word_type = COMMON_CMD_WORD_TYPE_DATA;
    buffer[1].data = (data >> 15) & 0x7FFF;

    buffer[2].word_type = COMMON_CMD_WORD_TYPE_DATA;
    buffer[2].data = data & 0x7FFF;
}

static void send_word(uint16_t *word)
{
    msi_get_fifo_data_ExpectAnyArgsAndReturn(true);
    msi_get_fifo_data_ReturnThruPtr_data(word);
}
