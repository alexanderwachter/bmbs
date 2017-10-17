include(CMakeForceCompiler)

# specify the cross compiler
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR "MSP430")
set(TOOLCHAIN_PREFIX     "msp430-elf-")
set(TOOLCHAIN_ROOT_DIR   "/opt/msp430-gcc-6.2.1.16_linux64/bin/")
cmake_force_c_compiler(  ${TOOLCHAIN_ROOT_DIR}${TOOLCHAIN_PREFIX}gcc GNU)
cmake_force_cxx_compiler(${TOOLCHAIN_ROOT_DIR}${TOOLCHAIN_PREFIX}g++ GNU)

execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
set(COMPILER_STRING "${CMAKE_C_COMPILER} ${GCC_VERSION}")
string(REGEX REPLACE "\n$" "" COMPILER_STRING "${COMPILER_STRING}")
message(${COMPILER_STRING})

# extend linker invocations so that map files are created
set( CMAKE_C_LINK_EXECUTABLE
     "<CMAKE_C_COMPILER> <FLAGS> <CMAKE_C_LINK_FLAGS> -Wl,-Map,<TARGET>.map <LINK_FLAGS> <OBJECTS> -o <TARGET>.out <LINK_LIBRARIES>")
set( CMAKE_CXX_LINK_EXECUTABLE
     "<CMAKE_CXX_COMPILER> <FLAGS> <CMAKE_CXX_LINK_FLAGS> -Wl,-Map,<TARGET>.map <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

# some basic configurations for the target platform
if(NOT CPU)
  set(CPU "msp430f5529")
  message("No CPU specified, choosing \"${CPU}\" as default")
endif()

#set(SUPPORT_FILES_INCLUDE_DIRECTORIES "-I/home/os/ti/ccsv6/ccs_base/msp430/include_gcc")
#set(SUPPORT_FILES_LINK_DIRECTORIES "-T/home/os/ti/ccsv6/ccs_base/msp430/include_gcc/${CPU}.ld")
set(SUPPORT_FILES_INCLUDE_DIRECTORIES "-I${CMAKE_CURRENT_SOURCE_DIR}/arch/${ARCH}/include_gcc")
set(SUPPORT_FILES_LINK_DIRECTORIES "-T${CMAKE_CURRENT_SOURCE_DIR}/arch/${ARCH}/include_gcc/${CPU}.ld")

# defining this variables here overrides the DEFAULT declarations in the
# root CMakeLists.txt
SET(TOOLCHAIN_BUILD_TYPE   "MinSizeRel")

SET(TOOLCHAIN_C_FLAGS      "-std=gnu11 -mmcu=${CPU} -Wall -Wextra -Wno-old-style-declaration -Os -g -gdwarf-3 -gstrict-dwarf -pedantic -ffunction-sections -fdata-sections ${SUPPORT_FILES_INCLUDE_DIRECTORIES}")
SET(TOOLCHAIN_CXX_FLAGS    "-std=c++11 -mmcu=${CPU} -Wall -Wextra -pedantic -ffunction-sections -fdata-sections ${SUPPORT_FILES_INCLUDE_DIRECTORIES}")
SET(TOOLCHAIN_LINKER_FLAGS "-Wl,-static -Wl,-gc-sections ${SUPPORT_FILES_LINK_DIRECTORIES}")

IF(NOT TARGET download)
  add_custom_target(download
    COMMAND ${TOOLCHAIN_ROOT_DIR}${TOOLCHAIN_PREFIX}objcopy -O ihex ${CMAKE_CURRENT_BINARY_DIR}/bmbs.out ${CMAKE_CURRENT_BINARY_DIR}/bmbs.hex
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tools/MSPFlasher_1.3.11/MSP430Flasher -s -z [RESET,VCC] -w ${CMAKE_CURRENT_BINARY_DIR}/bmbs.hex)
  add_dependencies(download bmbs)
ENDIF()
