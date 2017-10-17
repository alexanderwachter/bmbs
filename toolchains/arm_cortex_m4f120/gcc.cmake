include(CMakeForceCompiler)

# specify the cross compiler
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR "cortex-m4")
set(TOOLCHAIN_PREFIX     "arm-none-eabi-")
set(TOOLCHAIN_ROOT_DIR   "/usr/bin/")
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
  set(CPU "LM4F120H5QR")
  message("No CPU specified, choosing \"${CPU}\" as default")
endif()

set(SUPPORT_FILES_INCLUDE_DIRECTORIES "-I${CMAKE_CURRENT_SOURCE_DIR}/arch/${ARCH}/inc -I${CMAKE_CURRENT_SOURCE_DIR}/arch/${ARCH}")
set(SUPPORT_FILES_LINKER_FILE "-T${CMAKE_CURRENT_SOURCE_DIR}/arch/${ARCH}/inc/${CPU}.lds")

# defining this variables here overrides the DEFAULT declarations in the
# root CMakeLists.txt
SET(TOOLCHAIN_BUILD_TYPE   "MinSizeRel")

string(TOUPPER "${CPU}" CPU_)

SET(TOOLCHAIN_C_FLAGS      "-std=gnu11 -mcpu=cortex-m4 -DPART_${CPU_} --specs=nosys.specs -mthumb -Wall -Wextra -ffunction-sections -fdata-sections -Os -g -gstrict-dwarf -pedantic ${SUPPORT_FILES_INCLUDE_DIRECTORIES}")
SET(TOOLCHAIN_CXX_FLAGS    "-std=c++11 -mcpu=cortex-m4 -DPART_${CPU_} -mthumb -Wall -Wextra -pedantic -ffunction-sections -fdata-sections ${SUPPORT_FILES_INCLUDE_DIRECTORIES}")
SET(TOOLCHAIN_LINKER_FLAGS "-Wl,-static -Wl,-gc-sections -nostartfiles ${SUPPORT_FILES_LINKER_FILE}")

IF(NOT TARGET download)
  add_custom_target(download
    COMMAND ${TOOLCHAIN_ROOT_DIR}${TOOLCHAIN_PREFIX}objcopy -O binary ${CMAKE_CURRENT_BINARY_DIR}/bmbs.out ${CMAKE_CURRENT_BINARY_DIR}/bmbs.bin
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tools/lm4tools/lm4flash/lm4flash ${CMAKE_CURRENT_BINARY_DIR}/bmbs.bin)
  add_dependencies(download bmbs)
ENDIF()
