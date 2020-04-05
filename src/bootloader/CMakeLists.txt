# Bootloader project for the master core of the DSP.

project(bootloader C)

set(SOURCES
    main.c
)

# Set project-specific compiler and linker flags
set(MP_PROCESSOR_OPTION 33CH128MP508)
set(MP_LINKER_FILE_OPTION "--script=${CMAKE_CURRENT_SOURCE_DIR}/bootloader.ld")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=${MP_PROCESSOR_OPTION}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mcpu=${MP_PROCESSOR_OPTION}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,${MP_LINKER_FILE_OPTION}")

# Bootloader executable
add_executable(${PROJECT_NAME}.elf ${SOURCES})

# Generate hex file from ELF output
add_custom_target(${PROJECT_NAME}.hex ALL DEPENDS ${PROJECT_NAME}.elf)
add_custom_command(TARGET ${PROJECT_NAME}.hex
    COMMAND ${MP_BIN2ELF} ARGS ${PROJECT_NAME}.elf)
