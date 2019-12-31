cmake_minimum_required(VERSION 3.11.1)

project(synth)

set(SOURCES src/main.c)

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

add_executable(${PROJECT_NAME}.elf ${SOURCES})

# Rule to generate hex file from ELF output
set(MP_BIN2ELF xc16-bin2hex)
add_custom_target(${PROJECT_NAME}.hex ALL DEPENDS ${PROJECT_NAME}.elf)
add_custom_command(TARGET ${PROJECT_NAME}.hex
    COMMAND ${MP_BIN2ELF} ARGS ${PROJECT_NAME}.elf)

# Rule to generate MDB script
set(MDB_FLASH_SCRIPT ${PROJECT_NAME}_mdb_flash.txt)
add_custom_target(${MDB_FLASH_SCRIPT} DEPENDS ${PROJECT_NAME}.elf)
add_custom_command(TARGET ${MDB_FLASH_SCRIPT}
    COMMAND python
    ARGS ../scripts/gen_mdb_flash.py ${PROJECT_NAME}.elf ${MDB_FLASH_SCRIPT})

# Rule to flash program on hardware and run in MDB
add_custom_target(flash DEPENDS ${MDB_FLASH_SCRIPT})
add_custom_command(TARGET flash COMMAND mdb.sh ARGS ${MDB_FLASH_SCRIPT})
