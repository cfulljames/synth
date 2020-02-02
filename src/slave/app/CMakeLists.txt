# This is the project for the synthesis engine.
project(slave_app C ASM)

# Project source files
set(SOURCES
    audio.c
    envelope.c
    synth.s
    sine.c
    oscillator.c
)

# Generate a sine table assembly file (sine.s)
set(SINE_TABLE_GEN_SCRIPT ${CMAKE_SOURCE_DIR}/scripts/gen_sine_table.py)
add_custom_command(OUTPUT sine.c sine.h sine.inc
    COMMAND python3 ARGS ${SINE_TABLE_GEN_SCRIPT} sine.c sine.h sine.inc
    DEPENDS ${SINE_TABLE_GEN_SCRIPT})

# Build a static library
add_library(${PROJECT_NAME} ${SOURCES})

# Provide header files in include directory and generated header in binary dir
target_include_directories(${PROJECT_NAME}
    PUBLIC include ${CMAKE_CURRENT_BINARY_DIR})

# Add the HAL library
target_link_libraries(${PROJECT_NAME} slave_hal)
