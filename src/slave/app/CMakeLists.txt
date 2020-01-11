# This is the project for the synthesis engine.
project(slave_app C ASM)

# Project source files
set(SOURCES
    audio.c
)

# Generate a sine table header file (sine.h)
add_custom_command(OUTPUT sine.h
    COMMAND python ARGS ${CMAKE_SOURCE_DIR}/scripts/gen_sine_table.py sine.h)

# Build a static library
add_library(${PROJECT_NAME} ${SOURCES} sine.h)

# Provide header files in include directory and generated header in binary dir
target_include_directories(${PROJECT_NAME}
    PUBLIC include ${CMAKE_CURRENT_BINARY_DIR})

# Add the HAL library
target_link_libraries(${PROJECT_NAME} slave_hal)
