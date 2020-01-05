# This is the project for the slave core of the DSP.
#
# The build process for the slave core is as follows:
#
# - The slave project is built normally, generating an ELF file.
# - The ELF file is used to generate an assembly image file.
# - The assembly image file is then compiled into a static library.
#
# This static library can then be linked into the master project.

project(synth_slave C ASM)

# Project source files
set(SOURCES
    main.c
)

# Set core-specific compiler and linker flags
set(MP_PROCESSOR_OPTION 33CH128MP508S1)
set(MP_LINKER_FILE_OPTION --script=p${MP_PROCESSOR_OPTION}.gld)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=${MP_PROCESSOR_OPTION}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mcpu=${MP_PROCESSOR_OPTION}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,${MP_LINKER_FILE_OPTION}")

# Build the HAL library
add_subdirectory(hal)

# Build the application library
add_subdirectory(app)

# Generate the slave executable.  This will later be transformed into an image
# file using the command below.
add_executable(${PROJECT_NAME}.elf ${SOURCES})

# Add the subdirectory libraries
target_link_libraries(${PROJECT_NAME}.elf slave_hal slave_app)

# Generate an assembly image file (and header) from ELF output
add_custom_command(OUTPUT ${PROJECT_NAME}.s
    COMMAND ${MP_BIN2ELF} ARGS --image ${PROJECT_NAME} ${PROJECT_NAME}.elf
    DEPENDS ${PROJECT_NAME}.elf)

# Compile the new assembly image into a library archive
add_library(${PROJECT_NAME} ${PROJECT_NAME}.s)

# Make generated header available
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
