cmake_minimum_required(VERSION 3.11.1)

set(CMAKE_C_COMPILER "xc16-gcc")
set(CMAKE_ASM_COMPILER "xc16-gcc")
set(CMAKE_AR "xc16-ar")
set(CMAKE_RANLIB "xc16-ranlib")

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR pic)
set(CMAKE_CROSSCOMPILING 1)

set(CND_CONF default)

# Compiler flags (based on MPLAB generated makefiles)
set(CMAKE_C_FLAGS "-mno-eds-warn -omf=elf \
    -DXPRJ_default=${CND_CONF} -legacy-libc -O1 -msmart-io=1 -Wall \
    -msfr-warn=off -std=c99 -ffunction-sections")

# Add these flags to generate assembly
#    -fverbose-asm -Wa,-adhln \

# Assembler flags
set(CMAKE_ASM_FLAGS "-Wall")

# Debug-specific compiler flags
set(CMAKE_C_FLAGS_DEBUG "-g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1")

# Debug-specific assembler flags
set(CMAKE_ASM_FLAGS_DEBUG "-Wa,-g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1")

# Remove default linker flags that include some options that break xc16
set(CMAKE_C_LINK_FLAGS "")
set(CMAKE_ASM_LINK_FLAGS "")

# Linker flags (based on MPLAB generated makefiles)
set(CMAKE_EXE_LINKER_FLAGS "\
    -Wl,--local-stack,--defsym=__MPLAB_BUILD=1 \
    -Wl,--stack=16,--check-sections,--data-init,--pack-data,--handles \
    -Wl,--gc-sections,--fill-upper=0,--stackguard=16,--no-force-link \
    -Wl,--smart-io,--report-mem \
    -Wl,--memorysummary,memoryfile.xml")

# Add debug linker flags to keep debug memory regions free and define debug
# symbols.
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "\
    -mreserve=data@0x1000:0x101B \
    -mreserve=data@0x101C:0x101D \
    -mreserve=data@0x101E:0x101F \
    -mreserve=data@0x1020:0x1021 \
    -mreserve=data@0x1022:0x1023 \
    -mreserve=data@0x1024:0x1027 \
    -mreserve=data@0x1028:0x104F \
    -Wl,--defsym=__MPLAB_DEBUG=1 \
    -Wl,--defsym=__DEBUG=1 \
    -Wl,-D__DEBUG=__DEBUG \
    -Wl,--defsym=__MPLAB_DEBUGGER_PK3=1 \
    -Wl,--defsym=__S1DEBUG=1")

# Executable for generating hex files and application images
set(MP_BIN2ELF xc16-bin2hex)

# Top level project
project(synth C ASM)

# Bootloader project, not built by default.  Use `make bootloader.hex` to build.
add_subdirectory(src/bootloader EXCLUDE_FROM_ALL)

# The slave application image is built and linked into the master image.
add_subdirectory(src/master)

# Generate combined hex file with bootloader.
set(HEX_CAT_SCRIPT ${CMAKE_SOURCE_DIR}/scripts/combine_hex.py)
set(COMBINED_HEX synth_master_with_bootloader.hex)
set(INPUT_HEX src/master/synth_master.hex src/bootloader/bootloader.hex)
add_custom_target(${COMBINED_HEX} DEPENDS ${INPUT_HEX})
add_custom_command(TARGET ${COMBINED_HEX}
    COMMAND ${HEX_CAT_SCRIPT} ${INPUT_HEX} ${COMBINED_HEX})
