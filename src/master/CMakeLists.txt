# This is the project for the master core of the DSP.  The master core must load
# the slave core's image, so it is compiled and linked as part of the build
# process.

project(synth_master C ASM)

# Project source files
set(SOURCES
    main.c
    notes.c
    envtable.c
)

# Include the slave project as part of the master build
add_subdirectory(./../slave slave)

# Set core-specific compiler and linker flags
set(MP_PROCESSOR_OPTION 33CH128MP508)
set(MP_LINKER_FILE_OPTION --script=p${MP_PROCESSOR_OPTION}.gld)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=${MP_PROCESSOR_OPTION}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mcpu=${MP_PROCESSOR_OPTION}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,${MP_LINKER_FILE_OPTION}")

# Build the HAL library
add_subdirectory(hal)

# Add the sources and slave image to list of dependencies
add_executable(${PROJECT_NAME}.elf ${SOURCES})

# Add the subdirectory libraries
target_link_libraries(${PROJECT_NAME}.elf master_hal)

# Generate hex file from ELF output
add_custom_target(${PROJECT_NAME}.hex ALL DEPENDS ${PROJECT_NAME}.elf)
add_custom_command(TARGET ${PROJECT_NAME}.hex
    COMMAND ${MP_BIN2ELF} ARGS ${PROJECT_NAME}.elf)

# Generate a note frequency table C file and header
set(NOTE_TABLE_GEN_SCRIPT ${CMAKE_SOURCE_DIR}/scripts/gen_freq_table.py)
add_custom_command(OUTPUT notes.c notes.h
    COMMAND python3 ARGS ${NOTE_TABLE_GEN_SCRIPT} notes.c notes.h
    DEPENDS ${NOTE_TABLE_GEN_SCRIPT})

# Generate a envelope rate table C file and header
set(ENV_TABLE_GEN_SCRIPT ${CMAKE_SOURCE_DIR}/scripts/gen_env_table.py)
add_custom_command(OUTPUT envtable.c envtable.h
    COMMAND python3 ARGS ${ENV_TABLE_GEN_SCRIPT} envtable.c envtable.h
    DEPENDS ${ENV_TABLE_GEN_SCRIPT})

# Add generated header file location
target_include_directories(${PROJECT_NAME}.elf
    PRIVATE include ${CMAKE_CURRENT_BINARY_DIR} ../common/include)

# Generate MDB script for flashing
set(MDB_FLASH_SCRIPT ${PROJECT_NAME}_mdb_flash.txt)
add_custom_target(${MDB_FLASH_SCRIPT} DEPENDS ${PROJECT_NAME}.elf)
add_custom_command(TARGET ${MDB_FLASH_SCRIPT}
    COMMAND python
    ARGS ../../../scripts/gen_mdb_flash.py ${PROJECT_NAME}.elf ${MDB_FLASH_SCRIPT})

# Flash program on hardware and run in MDB
add_custom_target(flash DEPENDS ${MDB_FLASH_SCRIPT})
add_custom_command(TARGET flash COMMAND mdb.sh ARGS ${MDB_FLASH_SCRIPT})
