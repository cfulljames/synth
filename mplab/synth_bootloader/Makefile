#
#  There exist several targets which are by default empty and which can be
#  used for execution of your targets. These targets are usually executed
#  before and after some main targets. They are:
#
#     .build-pre:              called before 'build' target
#     .build-post:             called after 'build' target
#     .clean-pre:              called before 'clean' target
#     .clean-post:             called after 'clean' target
#     .clobber-pre:            called before 'clobber' target
#     .clobber-post:           called after 'clobber' target
#     .all-pre:                called before 'all' target
#     .all-post:               called after 'all' target
#     .help-pre:               called before 'help' target
#     .help-post:              called after 'help' target
#
#  Targets beginning with '.' are not intended to be called on their own.
#
#  Main targets can be executed directly, and they are:
#
#     build                    build a specific configuration
#     clean                    remove built files from a configuration
#     clobber                  remove all built files
#     all                      build all configurations
#     help                     print help mesage
#
#  Targets .build-impl, .clean-impl, .clobber-impl, .all-impl, and
#  .help-impl are implemented in nbproject/makefile-impl.mk.
#
#  Available make variables:
#
#     CND_BASEDIR                base directory for relative paths
#     CND_DISTDIR                default top distribution directory (build artifacts)
#     CND_BUILDDIR               default top build directory (object files, ...)
#     CONF                       name of current configuration
#     CND_ARTIFACT_DIR_${CONF}   directory of build artifact (current configuration)
#     CND_ARTIFACT_NAME_${CONF}  name of build artifact (current configuration)
#     CND_ARTIFACT_PATH_${CONF}  path to build artifact (current configuration)
#     CND_PACKAGE_DIR_${CONF}    directory of package (current configuration)
#     CND_PACKAGE_NAME_${CONF}   name of package (current configuration)
#     CND_PACKAGE_PATH_${CONF}   path to package (current configuration)
#
# NOCDDL


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib


# Configure user path settings
include ../conf.mk
PATH:=${USER_CMAKE_PATH}:${USER_PYTHON3_PATH}:${USER_TOOLCHAIN_PATH}:${PATH}

# Directory to run CMake
BUILD_DIR:=output

# Set build type-specific settings
ifeq (${TYPE_IMAGE}, DEBUG_RUN)
CMAKE_BUILD_TYPE:=Debug
BUILD_NAME_SUFFIX:=debug
else
CMAKE_BUILD_TYPE:=Release
BUILD_NAME_SUFFIX:=production
endif

# Distribution directory
DIST_DIR:=dist/${CONF}/${BUILD_NAME_SUFFIX}

# CMake build artifacts directory
CMAKE_OUTPUT_DIR:=${BUILD_DIR}/src/bootloader

# Hex file to copy after CMake build
CMAKE_HEX_FILE:=${CMAKE_OUTPUT_DIR}/bootloader.hex
CMAKE_ELF_FILE:=${CMAKE_OUTPUT_DIR}/bootloader.elf
CMAKE_UNIFIED_HEX_FILE:=${BUILD_DIR}/synth_master_with_bootloader.hex

# Final hex file
FINAL_HEX_FILE:=${DIST_DIR}/synth_bootloader.${BUILD_NAME_SUFFIX}.hex
FINAL_ELF_FILE:=${DIST_DIR}/synth_bootloader.${BUILD_NAME_SUFFIX}.elf
FINAL_UNIFIED_HEX_FILE:=${DIST_DIR}/synth_bootloader.${BUILD_NAME_SUFFIX}.unified.hex

# build
build:
	${MKDIR} -p ${BUILD_DIR}
	${MKDIR} -p ${DIST_DIR}
	cd ${BUILD_DIR} ; cmake -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} ../../../
	${MAKE} -C ${BUILD_DIR} --no-print-directory synth_master_with_bootloader.hex
	cp ${CMAKE_HEX_FILE} ${FINAL_HEX_FILE}
	cp ${CMAKE_ELF_FILE} ${FINAL_ELF_FILE}
	cp ${CMAKE_UNIFIED_HEX_FILE} ${FINAL_UNIFIED_HEX_FILE}

# clean
clean: .clean-post

.clean-pre:
# Add your pre 'clean' code here...
# WARNING: the IDE does not call this target since it takes a long time to
# simply run make. Instead, the IDE removes the configuration directories
# under build and dist directly without calling make.
# This target is left here so people can do a clean when running a clean
# outside the IDE.

.clean-post: .clean-impl
# Add your post 'clean' code here...


# clobber
clobber: .clobber-post

.clobber-pre:
# Add your pre 'clobber' code here...

.clobber-post: .clobber-impl
# Add your post 'clobber' code here...


# all
all: .all-post

.all-pre:
# Add your pre 'all' code here...

.all-post: .all-impl
# Add your post 'all' code here...


# help
help: .help-post

.help-pre:
# Add your pre 'help' code here...

.help-post: .help-impl
# Add your post 'help' code here...



# include project implementation makefile
include nbproject/Makefile-impl.mk

# include project make variables
include nbproject/Makefile-variables.mk
