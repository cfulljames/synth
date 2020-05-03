#include "flash.h"
#include "xc.h"

// Disable interrupts and write to flash.
#define WRITE_TO_FLASH() \
        __builtin_disi(5); \
        __builtin_write_NVM(); \
        while (_WR); \

flash_status_t flash_erase_page(uint32_t address)
{
    flash_status_t status = FLASH_OK;

    // Check alignment.
    if (address % PAGE_SIZE)
    {
        status = FLASH_BAD_ALIGNMENT;
    }

    // Check address range.
    if (FLASH_OK == status &&
            (address > MAX_PAGE_ADDRESS || address < MIN_ADDRESS))
    {
        status = FLASH_OUT_OF_RANGE;
    }

    if (FLASH_OK == status)
    {
        // Set NVM peripheral to page erase mode, and enable writing.
        NVMCON = 0x4003;

        // Set target address registers.
        NVMADRL = address & 0x0000FFFF;
        NVMADRH = (address >> 16) & 0x000000FF;

        WRITE_TO_FLASH();
    }

    // Check for write error.
    if (FLASH_OK == status && NVMCONbits.WRERR)
    {
        status = FLASH_WRITE_ERROR;
    }

    return status;
}

flash_status_t flash_write_dword(
        uint32_t address, uint32_t data_l, uint32_t data_h)
{
    flash_status_t status = FLASH_OK;

    // Check alignment.
    if (address % DOUBLE_WORD_SIZE)
    {
        status = FLASH_BAD_ALIGNMENT;
    }

    // Check address range.
    if (FLASH_OK == status &&
            (address > MAX_DWORD_ADDRESS || address < MIN_ADDRESS))
    {
        status = FLASH_OUT_OF_RANGE;
    }

    if (FLASH_OK == status)
    {
        // Set NVM peripheral to double-word mode, and enable writing.
        NVMCON = 0x4001;

        // Set table page register to the page where the write latches are located.
        TBLPAG = 0xFA;

        // Set target address registers.
        NVMADRL = address & 0x0000FFFF;
        NVMADRH = (address >> 16) & 0x000000FF;

        // Write first word to latch.
        __builtin_tblwtl(0, data_l & 0x0000FFFF);
        __builtin_tblwth(0, data_l >> 16);

        // Write second word to latch.
        __builtin_tblwtl(2, data_h & 0x0000FFFF);
        __builtin_tblwth(2, data_h >> 16);

        WRITE_TO_FLASH();
    }

    // Check for write error.
    if (FLASH_OK == status && NVMCONbits.WRERR)
    {
        status = FLASH_WRITE_ERROR;
    }

    return status;
}

flash_status_t flash_write_row(uint32_t address, const uint32_t *data)
{
    flash_status_t status = FLASH_OK;

    // Check alignment.
    if (address % ROW_SIZE)
    {
        status = FLASH_BAD_ALIGNMENT;
    }

    // Check address range.
    if (FLASH_OK == status &&
            (address > MAX_ROW_ADDRESS || address < MIN_ADDRESS))
    {
        status = FLASH_OUT_OF_RANGE;
    }

    if (FLASH_OK == status)
    {
        // Set NVM peripheral to row mode, and enable writing in uncompressed
        // format.
        NVMCON = 0x4002;

        // Set table page register to the page where the write latches are located.
        TBLPAG = 0xFA;

        // Set the write data source address registers.
        NVMSRCADRL = (uint16_t)data;
        NVMSRCADRH = 0;

        // Set target address registers.
        NVMADRL = address & 0x0000FFFF;
        NVMADRH = (address >> 16) & 0x000000FF;

        WRITE_TO_FLASH();
    }

    // Check for write error.
    if (FLASH_OK == status && (NVMCONbits.WRERR || NVMCONbits.URERR))
    {
        status = FLASH_WRITE_ERROR;
    }

    return status;
}

flash_status_t flash_read_word(uint32_t address, uint32_t *data)
{
    flash_status_t status = FLASH_OK;

    // Check alignment.
    if (address % INSTR_WORD_SIZE)
    {
        status = FLASH_BAD_ALIGNMENT;
    }

    // Check address range.
    if (FLASH_OK == status &&
            (address > MAX_READ_ADDRESS || address < MIN_ADDRESS))
    {
        status = FLASH_OUT_OF_RANGE;
    }

    if (FLASH_OK == status)
    {
        TBLPAG = (address & 0x00FF0000) >> 16;
        uint16_t offset = address & 0x0000FFFE;

        // Read the data from flash into the output pointer.
        uint16_t *output_word = (uint16_t *)data;
        *output_word++ = __builtin_tblrdl(offset);
        *output_word++ = __builtin_tblrdh(offset);
    }

    return status;
}
