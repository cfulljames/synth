# This is the project for the assembly code tests.
#
# These tests are built and run just like the normal release code.  Since the
# assembly code runs on the slave core, so do these tests.
#
# The assembly code is in the slave application, so it is included as a library
# along with the slave HAL for support.

project(test_synth C)

# Project source files
set(SOURCES
    test_synth.c
)

# Generate the slave executable.  This will later be transformed into an image
# file using the command below.
add_executable(${PROJECT_NAME}.elf ${SOURCES})

# Generate hex file from ELF output
add_custom_target(${PROJECT_NAME}.hex ALL DEPENDS ${PROJECT_NAME}.elf)
add_custom_command(TARGET ${PROJECT_NAME}.hex
    COMMAND ${MP_BIN2ELF} ARGS ${PROJECT_NAME}.elf)

# Add the slave application and HAL libraries
target_link_libraries(${PROJECT_NAME}.elf slave_hal slave_app unity)

# Add Unity header file location
target_include_directories(${PROJECT_NAME}.elf
    PRIVATE
    ../../lib/unity/src
    include)
