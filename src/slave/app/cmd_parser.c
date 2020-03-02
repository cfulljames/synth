#include "cmd_parser.h"
#include "msi.h"
#include "common_cmd.h"

#include <stdlib.h>

#define CMD_WORD (0U)
#define DATA_WORD0 (1U)
#define DATA_WORD1 (2U)

#define DATA_MSB_SHIFT (30U)
#define DATA_WORD0_SHIFT (15U)
#define DATA_WORD1_SHIFT (0U)

cmd_parser_cb_t m_cmd_received_callback;
common_cmd_t m_buffer[COMMON_CMD_NUM_WORDS];
uint8_t m_index = 0;


void cmd_parser_init(void)
{
    m_index = 0;
    m_cmd_received_callback = NULL;
}

void cmd_parser_set_callback(cmd_parser_cb_t cb)
{
    m_cmd_received_callback = cb;
}

void cmd_parser_update(void)
{
    uint16_t fifo_data;

    while (msi_get_fifo_data(&fifo_data))
    {
        common_cmd_t cmd;
        cmd.value = fifo_data;

        if (COMMON_CMD_WORD_TYPE_CMD == cmd.word_type)
        {
            // Start of a new command.
            m_index = CMD_WORD;
        }

        m_buffer[m_index] = cmd;
        m_index ++;

        if (COMMON_CMD_NUM_WORDS == m_index && m_cmd_received_callback)
        {
            // A full command has been received and a callback is set.

            // Extract and concatenate data from all words.
            uint32_t cmd_data =
                (uint32_t)m_buffer[CMD_WORD].data_msb << DATA_MSB_SHIFT
                | (uint32_t)m_buffer[DATA_WORD0].data << DATA_WORD0_SHIFT
                | (uint32_t)m_buffer[DATA_WORD1].data << DATA_WORD1_SHIFT;

            m_cmd_received_callback(
                    m_buffer[CMD_WORD].type,
                    m_buffer[CMD_WORD].channel,
                    m_buffer[CMD_WORD].command,
                    cmd_data);
        }
    }

}
