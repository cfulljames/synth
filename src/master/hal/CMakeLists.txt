# This is the project for the hardware abstraction layer.
project(master_hal C ASM)

# Project source files
set(SOURCES
    system.c
    uart.c
)

# Build a static library
add_library(${PROJECT_NAME} ${SOURCES})

# Provide header files in include directory
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Link the slave core image library
target_link_libraries(${PROJECT_NAME} synth_slave)

# Add common header files
target_include_directories(${PROJECT_NAME} PRIVATE ../../common/include)
