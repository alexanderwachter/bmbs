include(CMakeForceCompiler)

# specify the cross compiler
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR "AVR")
set(TOOLCHAIN_PREFIX     "avr-")
set(TOOLCHAIN_ROOT_DIR   "/usr/bin/")
cmake_force_c_compiler(  ${TOOLCHAIN_ROOT_DIR}${TOOLCHAIN_PREFIX}gcc GNU)
cmake_force_cxx_compiler(${TOOLCHAIN_ROOT_DIR}${TOOLCHAIN_PREFIX}g++ GNU)

execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
set(COMPILER_STRING "${CMAKE_C_COMPILER} ${GCC_VERSION}")
string(REGEX REPLACE "\n$" "" COMPILER_STRING "${COMPILER_STRING}")
message(${COMPILER_STRING})

# extend linker invocations so that map files are created
set( CMAKE_C_LINK_EXECUTABLE
     "<CMAKE_C_COMPILER> <FLAGS> <CMAKE_C_LINK_FLAGS> -Wl,-Map,<TARGET>.map <LINK_FLAGS> <OBJECTS> -o <TARGET>.elf <LINK_LIBRARIES>")
set( CMAKE_CXX_LINK_EXECUTABLE
     "<CMAKE_CXX_COMPILER> <FLAGS> <CMAKE_CXX_LINK_FLAGS> -Wl,-Map,<TARGET>.map <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

# some basic configurations for the target platform
if(NOT CPU)
  set(CPU "atmega328p")
  message("No CPU specified, choosing \"${CPU}\" as default")
endif()

if(NOT F_CPU)
  set(F_CPU "16000000UL")
endif()

set(SUPPORT_FILES_LINK_DIRECTORIES "-T${CMAKE_CURRENT_SOURCE_DIR}/arch/${ARCH}/avr5.x")

# defining this variables here overrides the DEFAULT declarations in the
# root CMakeLists.txt
SET(TOOLCHAIN_BUILD_TYPE   "MinSizeRel")

SET(TOOLCHAIN_C_FLAGS      "-std=gnu11 -mmcu=${CPU} -DF_CPU=${F_CPU} -Wall -Wextra -Wno-old-style-declaration -Os -g -pedantic -ffunction-sections -fdata-sections -funsigned-char -mno-interrupts -mcall-prologues ${SUPPORT_FILES_INCLUDE_DIRECTORIES}")
SET(TOOLCHAIN_CXX_FLAGS    "-std=c++11 -mmcu=${CPU} -Wall -Wextra -pedantic -ffunction-sections -fdata-sections ${SUPPORT_FILES_INCLUDE_DIRECTORIES}")
SET(TOOLCHAIN_LINKER_FLAGS "-Wl,-static -Wl,-nostdlib -Wl,-nostartfiles -Wl,-nodefault ${SUPPORT_FILES_LINK_DIRECTORIES}")

IF(NOT TARGET download)
  add_custom_target(download
    COMMAND ${TOOLCHAIN_ROOT_DIR}${TOOLCHAIN_PREFIX}objcopy -O ihex ${CMAKE_CURRENT_BINARY_DIR}/default ${CMAKE_CURRENT_BINARY_DIR}/bmbs.hex
    COMMAND avrdude -F -V -p ATmega328p -P /dev/ttyUSB0 -c avr910 -U flash:w:${CMAKE_CURRENT_BINARY_DIR}/bmbs.hex:i)
  add_dependencies(download bmbs)
ENDIF()
