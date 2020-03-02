/*
 * Master-Slave Interface Command Parser
 *
 * The command parser is responsible for interpreting the data received from the
 * master core via the FIFO.  A callback is used to notify another module
 * whenever a valid command is received.
 *
 * Note that this module does not interpret the command or validate its
 * contents; it only verifies that the format is correct.
 *
 * See common_cmd.h for command format information.
 */
#ifndef _CMD_PARSER_H
#define _CMD_PARSER_H

#include <stdint.h>

/*
 * Callback function type.
 *
 * This function will be called with the following parameters:
 *
 * type: The type of the command.
 * channel: The channel number of the command.
 * command: The command code of the command.
 * data: The data of the command.
 *
 * See common_cmd.h for more information about these values and their meanings.
 */
typedef void (*cmd_parser_cb_t)(
        uint16_t type, uint16_t channel, uint16_t command, uint32_t data);

/*
 * Initialize the command parser.
 *
 * The command parser depends on the master-slave interface, so msi_init must be
 * called prior to calling this function.  This function must be called before
 * using any of the other functions in this module.
 */
void cmd_parser_init(void);

/*
 * Set the callback to be invoked when a new command is received.
 */
void cmd_parser_set_callback(cmd_parser_cb_t cb);

/*
 * Update the command parser.
 *
 * This will check the FIFO for any new data, parse commands, and invoke the
 * callback if a valid command has been received since the previous update.
 */
void cmd_parser_update(void);

#endif // _CMD_PARSER_H
