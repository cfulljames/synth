# This is the project for the hardware abstraction layer.
project(slave_hal C ASM)

# Project source files
set(SOURCES
    dac.c
    system.c
    uart.c
    msi.c
)

# Build a static library
add_library(${PROJECT_NAME} ${SOURCES})

# Provide header files in include directory
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Add common header files
target_include_directories(${PROJECT_NAME} PRIVATE ../../common/include)
