#include "synth_ctrl.h"

#include "cmd_parser.h"
#include "voice.h"
#include "audio.h"
#include "common_cmd.h"

#include <string.h>
#include <stdbool.h>

/******************************************************************************
 * Macros
 ******************************************************************************/

// Increment the head or tail pointer for the command ring buffer, wrapping
// around when the end is reached.
#define INCREMENT_BUFFER_INDEX(index) { \
        if (++(index) >= SYNTH_CTRL_BUFFER_SIZE) (index) = 0; }

#define MODULATION_TO_SHIFT (2)
#define MODULATION_FROM_MASK (0x03)

#define OUTPUT_MOD_ROW (VOICE_OPERATORS_PER_VOICE)
#define NUM_MOD_CHANNELS (VOICE_OPERATORS_PER_VOICE * VOICE_OPERATORS_PER_VOICE)

/******************************************************************************
 * Types
 ******************************************************************************/

typedef struct command_values_s {
    uint16_t type;
    uint16_t channel;
    uint16_t command;
    uint32_t data;
} command_values_t;

/******************************************************************************
 * Static Function Declarations
 ******************************************************************************/

static void cmd_received_callback(
        uint16_t type, uint16_t channel, uint16_t command, uint32_t data);
static inline void process_cmd(command_values_t *cmd);
static inline void process_voice_cmd(command_values_t *cmd);
static inline void process_modulation_cmd(command_values_t *cmd);
static inline void process_operator_cmd(command_values_t *cmd);

/******************************************************************************
 * Module-Scope Data
 ******************************************************************************/

// Voice instances.
static voice_t m_voices[SYNTH_CTRL_NUM_VOICES];

// Envelope configurations for each operator.
static envelope_config_t m_env_configs[VOICE_OPERATORS_PER_VOICE];

// Oscillator configurations for each operator.
static oscillator_config_t m_osc_configs[VOICE_OPERATORS_PER_VOICE];

// Modulation matrix to control modulation froms to each operator.
static int16_t m_mod_matrix[VOICE_OPERATORS_PER_VOICE + 1][VOICE_OPERATORS_PER_VOICE];

command_values_t m_cmd_buffer[SYNTH_CTRL_BUFFER_SIZE];
uint8_t m_cmd_buffer_head;
uint8_t m_cmd_buffer_tail;
bool m_cmd_buffer_empty;

/******************************************************************************
 * Interface Functions
 ******************************************************************************/

void synth_ctrl_init(void)
{
    memset(m_cmd_buffer, 0, sizeof(m_cmd_buffer));
    memset(m_mod_matrix, 0, sizeof(m_mod_matrix));
    m_cmd_buffer_head = 0;
    m_cmd_buffer_tail = 0;
    m_cmd_buffer_empty = true;

    cmd_parser_init();
    for (uint16_t i = 0; i < VOICE_OPERATORS_PER_VOICE; i ++)
    {
        envelope_config_init(&m_env_configs[i]);
        oscillator_config_init(&m_osc_configs[i]);
    }

    for (uint16_t i = 0; i < SYNTH_CTRL_NUM_VOICES; i ++)
    {
        voice_init(&m_voices[i], m_env_configs, m_osc_configs, m_mod_matrix);
    }
    audio_init(m_voices, SYNTH_CTRL_NUM_VOICES);
    cmd_parser_set_callback(cmd_received_callback);
}

void synth_ctrl_update(void)
{
    cmd_parser_update();
    audio_update();
}

/******************************************************************************
 * Static Functions
 ******************************************************************************/

__attribute__((always_inline))
static inline void process_voice_cmd(command_values_t *cmd)
{
    if (cmd->channel >= SYNTH_CTRL_NUM_VOICES)
    {
        // Invalid voice; ignore command.
        return;
    }

    voice_t *voice = &m_voices[cmd->channel];
    switch (cmd->command)
    {
        case COMMON_CMD_VOICE_START_NOTE:
            voice_start(voice, cmd->data);
            break;
        case COMMON_CMD_VOICE_RELEASE:
            voice_release(voice);
            break;
        case COMMON_CMD_VOICE_STOP:
            voice_stop(voice);
            break;
        default:
            // Invalid command; ignore.
            break;
    }
}

__attribute__((always_inline))
static inline void process_modulation_cmd(command_values_t *cmd)
{
    if (cmd->channel >= NUM_MOD_CHANNELS)
    {
        // Invalid modulation channel; ignore command.
        return;
    }

    uint8_t from_operator = cmd->channel & MODULATION_FROM_MASK;
    uint8_t to_operator = cmd->channel >> MODULATION_TO_SHIFT;

    m_mod_matrix[to_operator][from_operator] = cmd->data;
}

__attribute__((always_inline))
static inline void process_operator_cmd(command_values_t *cmd)
{
    if (cmd->channel >= VOICE_OPERATORS_PER_VOICE)
    {
        // Invalid operator; ignore command.
        return;
    }

    oscillator_config_t *osc_cfg = &m_osc_configs[cmd->channel];
    envelope_config_t *env_cfg = &m_env_configs[cmd->channel];
    switch(cmd->command)
    {
        case COMMON_CMD_OPERATOR_SET_HARMONIC:
            oscillator_config_set_harmonic(osc_cfg, cmd->data);
            break;
        case COMMON_CMD_OPERATOR_SET_BITMASK:
            oscillator_config_set_bitmask(osc_cfg, cmd->data);
            break;
        case COMMON_CMD_OPERATOR_SET_OUTPUT:
            m_mod_matrix[OUTPUT_MOD_ROW][cmd->channel] = cmd->data;
            break;
        case COMMON_CMD_OPERATOR_SET_ATTACK:
            envelope_set_attack(env_cfg, cmd->data);
            break;
        case COMMON_CMD_OPERATOR_SET_DECAY:
            envelope_set_decay(env_cfg, cmd->data);
            break;
        case COMMON_CMD_OPERATOR_SET_SUSTAIN:
            envelope_set_sustain(env_cfg, cmd->data);
            break;
        case COMMON_CMD_OPERATOR_SET_RELEASE:
            envelope_set_release(env_cfg, cmd->data);
            break;
        default:
            break;
    }
}

__attribute__((always_inline))
static inline void process_cmd(command_values_t *cmd)
{
    switch (cmd->type)
    {
        case COMMON_CMD_TYPE_GLOBAL:
            // Only command is Stop All
            for (uint8_t i = 0; i < SYNTH_CTRL_NUM_VOICES; i ++)
            {
                voice_stop(&m_voices[i]);
            }
            break;
        case COMMON_CMD_TYPE_VOICE:
            process_voice_cmd(cmd);
            break;
        case COMMON_CMD_TYPE_OPERATOR:
            process_operator_cmd(cmd);
            break;
        case COMMON_CMD_TYPE_MODULATION:
            process_modulation_cmd(cmd);
            break;
        default:
            // Invalid command type; ignore.
            break;
    }
}

/*
 * Process received commands.
 */
static void cmd_received_callback(
        uint16_t type, uint16_t channel, uint16_t command, uint32_t data)
{
    if (COMMON_CMD_TYPE_GLOBAL == type && COMMON_CMD_GLOBAL_COMPLETE == command)
    {
        // Command sequence complete; process received commands.
        while (!m_cmd_buffer_empty)
        {
            command_values_t *cmd = &m_cmd_buffer[m_cmd_buffer_tail];
            process_cmd(cmd);

            INCREMENT_BUFFER_INDEX(m_cmd_buffer_tail);

            if (m_cmd_buffer_tail == m_cmd_buffer_head)
            {
                m_cmd_buffer_empty = true;
            }
        }
    }
    else
    {
        command_values_t *cmd = &m_cmd_buffer[m_cmd_buffer_head];
        if (!m_cmd_buffer_empty && m_cmd_buffer_head == m_cmd_buffer_tail)
        {
            // Buffer overflow; move tail pointer to remove oldest command.
            INCREMENT_BUFFER_INDEX(m_cmd_buffer_tail);
        }

        INCREMENT_BUFFER_INDEX(m_cmd_buffer_head);

        m_cmd_buffer_empty = false;

        cmd->type = type;
        cmd->channel = channel;
        cmd->command = command;
        cmd->data = data;
    }
}

