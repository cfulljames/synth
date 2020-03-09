/*
 * Command structure definition.
 *
 * This is the format used to send data from the master to the slave using the
 * FIFO.
 *
 * Each command contains a type, a channel number, a command code, and data.
 *
 * Type (3 bits):
 *      The type of the command.  This is used to determine the meaning of the
 *      channel number and command code.
 *
 * Channel Number (4 bits):
 *      The channel that the command applies to.  This may be, for instance, the
 *      number of a voice for voice-type commands, or the number of an operator
 *      for operator-type commands.
 *
 * Command Code (6 bits):
 *      Value specifying the operation to be performed.  See the "Commands"
 *      section of enumerations below.  The Command Code is used to determine
 *      the meaning of the data value.
 *
 * Data (32 bits):
 *      Command-specific data.  Most often, this is used to set the value of a
 *      parameter in the synthesis engine.
 *
 * In total, a command comprises three 16-bit words, formatted as follows:
 *
 * Word 0: WTTTNNNNCCCCCCDD
 *      W is the word type (always 0)
 *      T is the command type
 *      N is the channel number
 *      C is the command code
 *      D is the 2 most-significant bits of the command data
 *
 * Word 1: WDDDDDDDDDDDDDDD
 *      W is the word type (always 1)
 *      D is bits 29:15 of the command data
 *
 * Word 2: WDDDDDDDDDDDDDDD
 *      W is the word type (always 1)
 *      D is bits 15:0 of the command data
 *
 */
#ifndef _COMMON_CMD_H
#define _COMMON_CMD_H

// Number of words used for each command.
#define COMMON_CMD_NUM_WORDS (3U)

/******************************************************************************
 * Word Types
 ******************************************************************************/

typedef enum common_cmd_word_type_e {
    COMMON_CMD_WORD_TYPE_CMD            = 0,
    COMMON_CMD_WORD_TYPE_DATA           = 1,
} common_cmd_word_type_t;

/******************************************************************************
 * Command Types
 ******************************************************************************/

typedef enum common_cmd_type_e {
    COMMON_CMD_TYPE_GLOBAL              = 0,
    COMMON_CMD_TYPE_OPERATOR            = 1,
    COMMON_CMD_TYPE_VOICE               = 2,
    COMMON_CMD_TYPE_MODULATION          = 3,
} common_cmd_type_t;

/******************************************************************************
 * Commands
 ******************************************************************************/

/*
 * Global Commands:
 * - Stop All:
 *      Stop all voices from playing.  Channel and data are unused.
 */
typedef enum common_cmd_global_e {
    COMMON_CMD_GLOBAL_STOP_ALL          = 0,
} common_cmd_global_t;

/*
 * Operator Commands:
 *
 * For all operator commands, the Channel value selects the operator.
 *
 * - Set Harmonic:
 *      Set the harmonic of the operator.  Data contains the harmonic.
 *
 * - Set Output:
 *      Set the output gain of the operator.  Data contains the gain.
 *
 * - Set Attack:
 *      Set the attack rate of the operator.  Data contains the attack.
 *
 * - Set Decay:
 *      Set the decay rate of the operator.  Data contains the decay.
 *
 * - Set Sustain:
 *      Set the sustain level of the operator.  Data contains the sustain.
 *
 * - Set Release:
 *      Set the release rate of the operator.  Data contains the release.
 *
 * - Set Bitmask:
 *      Set the bitmask of the operator (pre gain).  Data contains the bitmask.
 */
typedef enum common_cmd_operator_e {
    COMMON_CMD_OPERATOR_SET_HARMONIC    = 0,
    COMMON_CMD_OPERATOR_SET_OUTPUT      = 1,
    COMMON_CMD_OPERATOR_SET_ATTACK      = 2,
    COMMON_CMD_OPERATOR_SET_DECAY       = 3,
    COMMON_CMD_OPERATOR_SET_SUSTAIN     = 4,
    COMMON_CMD_OPERATOR_SET_RELEASE     = 5,
    COMMON_CMD_OPERATOR_SET_BITMASK     = 6,
} common_cmd_operator_t;

/*
 * Voice Commands:
 *
 * For all voice commands, the Channel Number selects the voice.
 *
 * - Start Note:
 *      Start playing a note.  Data contains the note frequency.
 *
 * - Release:
 *      Release the current note, if one is playing.  This will transition the
 *      envelope to the release state, and should normally be used when a key is
 *      released or a MIDI note off event occurs. Data is unused.
 *
 * - Stop:
 *      Stop the current note, if one is playing.  Data is unused.
 */
typedef enum common_cmd_voice_e {
    COMMON_CMD_VOICE_START_NOTE         = 0,
    COMMON_CMD_VOICE_RELEASE            = 1,
    COMMON_CMD_VOICE_STOP               = 2,
} common_cmd_voice_t;

/*
 * Modulation Matrix Commands:
 *
 * For all operator commands, the Channel Number selects the position in the
 * modulation matrix.  It can be calculated as follows:
 *
 *  Channel Number = (To Operator) * 4 + (From Operator)
 *
 * - Set:
 *      Set the amount of modulation.  Data contains the modulation level.
 */
typedef enum common_cmd_modulation_e {
    COMMON_CMD_MODULATION_SET           = 0,
} common_cmd_modulation_t;

/******************************************************************************
 * Data Structure
 ******************************************************************************/

typedef union {

    // Bits in a command word.
    struct {
        // The 2 most significant bits of the data for this command.
        uint16_t data_msb           :2;

        // The command number.
        uint16_t command            :6;

        // The channel that this command is for.
        uint16_t channel            :4;

        // The type of the command.
        uint16_t type               :3;

        // Type of the word.
        uint16_t _word_type         :1;
    };

    // Bits in a data word.
    struct {

        // Data portion of a data word.
        uint16_t data               :15;

        // Type of the word.
        uint16_t word_type          :1;
    };

    // Integer value of the structure.
    uint16_t value;

} common_cmd_t;

#endif
