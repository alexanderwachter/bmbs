cmake_minimum_required(VERSION 3.0.0)
#-----------------------------------------------------------------------
# SUBDIRLIST
#-----------------------------------------------------------------------
macro(SUBDIRLIST result directory)
  file(GLOB tmp RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/${directory}" "${directory}/*")
  foreach(loop ${tmp})
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${directory}/${loop}")
      list(APPEND ${result} ${loop})
    endif()
  endforeach()
  list(REMOVE_DUPLICATES ${result})
endmacro()
#-----------------------------------------------------------------------
# Check arch
#-----------------------------------------------------------------------
SUBDIRLIST(arch_list "arch")
list(REMOVE_ITEM arch_list "include" "template" "CMakeFiles")

if(DEFINED arch)
	set(ARCH ${arch})
endif()

if(NOT DEFINED ARCH)
	message(WARNING "no ARCH defined. Use generic")
	set(ARCH "generic")
endif()

list(FIND arch_list ${ARCH} arch_index)
if(${arch_index} LESS 0)
	message(FATAL_ERROR "ARCH ${ARCH} is not supported")
endif()

#-----------------------------------------------------------------------
# Toolchain (this bust be placed before project())
#-----------------------------------------------------------------------
include_directories(SYSTEM ${CMAKE_CURRENT_BINARY_DIR})

if(NOT DEFINED TOOL_CHAIN)
	message(WARNING "TOOL_CHAIN not set. Use gcc")
	set(TOOL_CHAIN "gcc")
endif()

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/toolchains/${ARCH}/${TOOL_CHAIN}.cmake")
message(STATUS "Used toolchain: ${CMAKE_TOOLCHAIN_FILE}")

#-----------------------------------------------------------------------
#Project info
#-----------------------------------------------------------------------
project("bmbs")
set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")

#-----------------------------------------------------------------------
# General configuration
#-----------------------------------------------------------------------
# this flags are a convenient method to define some flags without the usage
# of a dedicated toolchain file (for clang and gcc based compilers)
# TOOLCHAIN variables simply override them
set(DEFAULT_BUILD_TYPE     MinSizeRel)
set(DEFAULT_C_FLAGS_GCC    "-std=gnu11 -Wall -Wextra -pedantic")
set(DEFAULT_CXX_FLAGS_GCC  "-std=c++11 -Wall -Wextra -pedantic")
set(DOXYGEN_CONFIG_IN_FILE "${PROJECT_SOURCE_DIR}/doc/template.doxy.in")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(doxygenSupport)              # adds doxygen target

#-----------------------------------------------------------------------
# set flags using the toolchain variables
#-----------------------------------------------------------------------
if(TOOLCHAIN_C_FLAGS)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TOOLCHAIN_C_FLAGS}")
endif()

if(TOOLCHAIN_CXX_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TOOLCHAIN_CXX_FLAGS}")
endif()

if(TOOLCHAIN_ASM_FLAGS)
  set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${TOOLCHAIN_ASM_FLAGS}")
endif()

if(TOOLCHAIN_LINKER_FLAGS)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${TOOLCHAIN_LINKER_FLAGS}")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${TOOLCHAIN_LINKER_FLAGS}")
endif()

if(BENCHMARK_LINKER_FLAGS)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${BENCHMARK_LINKER_FLAGS}")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${BENCHMARK_LINKER_FLAGS}")
endif()

if(CMAKE_COMPILER_IS_GNUCC)
	if(NOT DEFINED CMAKE_C_FLAGS)
		set(CMAKE_C_FLAGS "${DEFAULT_C_FLAGS_GCC}")
	endif()
	if(NOT DEFINED CMAKE_CXX_FLAGS)
		set(CMAKE_CXX_FLAGS "${DEFAULT_CXX_FLAGS_GCC}")
	endif()
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${CMAKE_CURRENT_SOURCE_DIR}/lib -I${CMAKE_CURRENT_SOURCE_DIR}/arch/include")

#-----------------------------------------------------------------------
# plug the things together
#-----------------------------------------------------------------------
#message(STATUS "Available ARCHS: ${arch_list}, ARCH: ${ARCH}")

add_executable(bmbs main.c)
add_subdirectory(arch)
add_subdirectory(benchmarks)
add_subdirectory(lib)

target_sources(bmbs PRIVATE $<TARGET_OBJECTS:benchmarks> $<TARGET_OBJECTS:arch>)
target_link_libraries(bmbs lib)

#-----------------------------------------------------------------------
# create config.h
#-----------------------------------------------------------------------
get_directory_property(ARCH_VERSION_MAJOR DIRECTORY arch/${ARCH} DEFINITION VERSION_MAJOR)
get_directory_property(ARCH_VERSION_MINOR DIRECTORY arch/${ARCH} DEFINITION VERSION_MINOR)
get_directory_property(LIB_VERSION_MAJOR DIRECTORY lib DEFINITION VERSION_MAJOR)
get_directory_property(LIB_VERSION_MINOR DIRECTORY lib DEFINITION VERSION_MINOR)
configure_file("info.h.in" "info.h")
