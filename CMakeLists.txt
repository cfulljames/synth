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
    -DXPRJ_default=${CND_CONF} -legacy-libc -O0 -msmart-io=1 -Wall \
    -msfr-warn=off -std=c99 ")

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
    -Wl,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr \
    -Wl,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link \
    -Wl,--smart-io,-Map=\"${PROJECT_NAME}.map\",--report-mem \
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

# Add master, which will include the slave image
add_subdirectory(src/master)

# Set core-specific compiler and linker flags to use for Unity and unit tests
set(MP_PROCESSOR_OPTION 33CH128MP508S1)
set(MP_LINKER_FILE_OPTION --script=p${MP_PROCESSOR_OPTION}.gld)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=${MP_PROCESSOR_OPTION} -DUNITY_INCLUDE_CONFIG_H")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mcpu=${MP_PROCESSOR_OPTION}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,${MP_LINKER_FILE_OPTION}")

# Build Unity library for unit tests
add_subdirectory(lib/unity/src unity EXCLUDE_FROM_ALL)

# Add unity_config.h to unity build
target_include_directories(unity PRIVATE test/asm/include)

# Add assembly unit tests
add_subdirectory(test/asm EXCLUDE_FROM_ALL)
