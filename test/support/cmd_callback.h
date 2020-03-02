/*
 * These functions don't actually exist; they are just prototypes for CMock to
 * generate mocks for.  To use, import mock_cmd_callback.h
 */
#ifndef _CMD_CALLBACK_H
#define _CMD_CALLBACK_H

/*
 * This is a fake function that will be mocked using CMock when the tests are
 * run.
 */
void fake_cmd_callback(
        uint16_t type, uint16_t channel, uint16_t command, uint32_t data);

#endif
