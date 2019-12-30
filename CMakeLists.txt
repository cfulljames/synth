cmake_minimum_required(VERSION 3.11.1)

project(synth C)

set(SOURCES src/main.c)

add_executable(${PROJECT_NAME} ${SOURCES})

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR pic)
set(CMAKE_CROSSCOMPILING 1)

set(MP_PROCESSOR_OPTION 33CH128MP508)
set(CND_CONF default)
set(MP_LINKER_FILE_OPTION --script=p33CH128MP508.gld)

set(CMAKE_C_COMPILER xc16-gcc)

# Compiler flags (based on MPLAB generated makefiles)
set(CMAKE_C_FLAGS "-mcpu=${MP_PROCESSOR_OPTION} -g -mno-eds-warn -omf=elf \
    -DXPRJ_default=${CND_CONF} -legacy-libc -O0 -msmart-io=1 -Wall \
    -msfr-warn=off -std=c99 ")

# Debug-specific compiler flags
set(CMAKE_C_FLAGS_DEBUG "-D__DEBUG -D__MPLAB_DEBUGGER_PK3=1")

# Default C link flags include some options that break xc16
set(CMAKE_C_LINK_FLAGS "")

# Linker flags (based on MPLAB generated makefiles)
set(CMAKE_EXE_LINKER_FLAGS "\
    -Wl,--local-stack,--defsym=__MPLAB_BUILD=1,${MP_LINKER_FILE_OPTION} \
    -Wl,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr \
    -Wl,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link \
    -Wl,--smart-io,-Map=\"${PROJECT_NAME}.map\",--report-mem \
    -Wl,--memorysummary,memoryfile.xml")
