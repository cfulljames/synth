# This is the project for the slave core of the DSP.
#
# The build process for the slave core is as follows:
#
# - The slave project is built normally, generating an ELF file.
# - The ELF file is used to generate an assembly image file.
# - The assembly image file is then compiled into a static library.
#
# This static library can then be linked into the master project.

project(synth_slave C ASM)

# Project source files
set(SOURCES
    main.c
    system.c
)

add_executable(${PROJECT_NAME}.elf ${SOURCES})

# Generate an assembly image file (and header) from ELF output
#add_custom_target(${PROJECT_NAME}.s ALL DEPENDS ${PROJECT_NAME}.elf)
add_custom_command(OUTPUT ${PROJECT_NAME}.s
    COMMAND ${MP_BIN2ELF} ARGS --image ${PROJECT_NAME} ${PROJECT_NAME}.elf
    DEPENDS ${PROJECT_NAME}.elf)

# Compile the new assembly image into a library archive
add_library(${PROJECT_NAME} ${PROJECT_NAME}.s)

# Make generated header available
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
