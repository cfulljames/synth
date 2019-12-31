# This is the project for the master core of the DSP.  The master core must load
# the slave core's image, so it is compiled and linked as part of the build
# process.

project(synth_master C ASM)

# Project source files
set(SOURCES
    main.c
    system.c
)

# Include the slave project as part of the master build
add_subdirectory(./../slave slave)

# Add the sources and slave image to list of dependencies
add_executable(${PROJECT_NAME}.elf ${SOURCES})

# Link slave image library into master executable
target_link_libraries(${PROJECT_NAME}.elf synth_slave)

# Generate hex file from ELF output
add_custom_target(${PROJECT_NAME}.hex ALL DEPENDS ${PROJECT_NAME}.elf)
add_custom_command(TARGET ${PROJECT_NAME}.hex
    COMMAND ${MP_BIN2ELF} ARGS ${PROJECT_NAME}.elf)

# Generate MDB script for flashing
set(MDB_FLASH_SCRIPT ${PROJECT_NAME}_mdb_flash.txt)
add_custom_target(${MDB_FLASH_SCRIPT} DEPENDS ${PROJECT_NAME}.elf)
add_custom_command(TARGET ${MDB_FLASH_SCRIPT}
    COMMAND python
    ARGS ../../../scripts/gen_mdb_flash.py ${PROJECT_NAME}.elf ${MDB_FLASH_SCRIPT})

# Flash program on hardware and run in MDB
add_custom_target(flash DEPENDS ${MDB_FLASH_SCRIPT})
add_custom_command(TARGET flash COMMAND mdb.sh ARGS ${MDB_FLASH_SCRIPT})
