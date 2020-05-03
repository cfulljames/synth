/*
 * Flash Storage
 *
 * Provides some interface functions for reading and writing flash memory.  This
 * is used to program and verify the application code.
 *
 * Note that this module doesn't have init/deinit functions.  Each function in
 * this module does its own setup and cleanup as needed.
 */
#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>

extern uint32_t _APPLICATION_IVT_BASE;

// Number of instruction words in a row of flash.
#define WORDS_PER_ROW (128U)

// Number of rows per page of flash.
#define ROWS_PER_PAGE (8U)

// Number of memory locations per instruction word.
#define INSTR_WORD_SIZE (2U)

// Number of memory locations per double instruction word.
#define DOUBLE_WORD_SIZE (4U)

// Number of memory locations per row of flash.
#define ROW_SIZE (WORDS_PER_ROW * DOUBLE_WORD_SIZE)

// Number of memory locations per page of flash.
#define PAGE_SIZE (ROW_SIZE * ROWS_PER_PAGE)

// Address at end of user flash (first invalid address).
#define USER_FLASH_ADDRESS_END (0x015F00UL)

// Maximum valid address for a word read.
#define MAX_WORD_ADDRESS (USER_FLASH_ADDRESS_END - INSTR_WORD_SIZE)

// Maximum valid address for a double word write.
#define MAX_DWORD_ADDRESS (USER_FLASH_ADDRESS_END - DOUBLE_WORD_SIZE)

// Maximum valid starting address for a row of flash.
#define MAX_ROW_ADDRESS (USER_FLASH_ADDRESS_END - ROW_SIZE)

// Maximum valid starting address of a page in flash.
#define MAX_PAGE_ADDRESS (USER_FLASH_ADDRESS_END - PAGE_SIZE)

// Maximum valid address for reading from flash.
#define MAX_READ_ADDRESS (0x00FFFFFFUL)

// Minimum valid address for application space.
#define MIN_ADDRESS (_APPLICATION_IVT_BASE)

/*
 * Status values returned from the flash functions.
 */
typedef enum flash_status_e {
    FLASH_OK = 0,
    FLASH_BAD_ALIGNMENT = -1,
    FLASH_OUT_OF_RANGE = -2,
    FLASH_WRITE_ERROR = -3,
} flash_status_t;

/*
 * Erase a page of flash memory.
 */
flash_status_t flash_erase_page(uint32_t address);

/*
 * Write a double word (two 24-bit instructions) to flash.
 *
 * Note that while 32 bit values are used, the most significant byte will always
 * be written (and read) as 0.  Only the lower 24 bits are written.
 */
flash_status_t flash_write_dword(
        uint32_t address, uint32_t data_l, uint32_t data_h);

/*
 * Write a row (128 24-bit instructions) to flash.
 *
 * Note that while 32 bit values are used, the most significant byte will always
 * be written (and read) as 0.  Only the lower 24 bits are written.
 *
 * data must point to an array with a length of 128.
 */
flash_status_t flash_write_row(uint32_t address, const uint32_t *data);

/*
 * Read one 24-bit instruction word from flash.
 */
flash_status_t flash_read_word(uint32_t address, uint32_t *data);

#endif
