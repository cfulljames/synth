# Bootloader project for the master core of the DSP.

project(bootloader C)

set(SOURCES
    main.c
    traps.c
    uart.c
    system.c
    log.c
    crc.c
)

# Set project-specific compiler and linker flags
set(MP_PROCESSOR_OPTION 33CH128MP508)
set(MP_LINKER_FILE_OPTION "--script=bootloader.ld")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=${MP_PROCESSOR_OPTION}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mcpu=${MP_PROCESSOR_OPTION}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
    -Wl,${MP_LINKER_FILE_OPTION} \
    -Wl,-Map=${PROJECT_NAME}.map \
    -Wl,--no-isr,--no-ivt \
")

# Bootloader executable
add_executable(${PROJECT_NAME}.elf ${SOURCES})

# Header files
target_include_directories(${PROJECT_NAME}.elf PRIVATE include)

# Copy linker scripts to build directory.
add_custom_command(
    TARGET ${PROJECT_NAME}.elf PRE_LINK
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/bootloader.ld
            ${CMAKE_CURRENT_BINARY_DIR}/bootloader.ld
)

add_custom_command(
    TARGET ${PROJECT_NAME}.elf PRE_LINK
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/src/common/memory_map.ld
            ${CMAKE_CURRENT_BINARY_DIR}/memory_map.ld
)

# Generate hex file from ELF output.
add_custom_target(${PROJECT_NAME}.hex ALL DEPENDS ${PROJECT_NAME}.elf)
add_custom_command(TARGET ${PROJECT_NAME}.hex
    COMMAND ${MP_BIN2ELF} ARGS ${PROJECT_NAME}.elf)
