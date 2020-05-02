# Bootloader Hardware Abstraction Layer

project(bootloader_hal C)

set(SOURCES
    traps.c
    uart.c
    system.c
    crc.c
)

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC include)
