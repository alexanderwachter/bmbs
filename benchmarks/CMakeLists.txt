#-----------------------------------------------------------------------
# SUBDIRLIST
#-----------------------------------------------------------------------
macro(SUBDIRLIST result directory)
  file(GLOB tmp RELATIVE "${CMAKE_SOURCE_DIR}/${directory}" "*")
  foreach(loop ${tmp})
    if(IS_DIRECTORY "${CMAKE_SOURCE_DIR}/${directory}/${loop}")
      list(APPEND ${result} ${loop})
    endif()
  endforeach()
  list(REMOVE_DUPLICATES ${result})
endmacro()

SUBDIRLIST(benchmark_list "benchmarks")

if(NOT DEFINED BENCHMARKS)
  message("No benchmarks selected. Add all: ${benchmark_list}")
  message("To add specific: -DBENCHMARKS=<benchmark1>;<benchmark2>;<...>")
  set(BENCHMARKS ${benchmark_list})
  unset(BENCHMARKS CACHE)
endif()

message("Benchmarks: ${BENCHMARKS}")

set(bm_files "")

foreach(bm_subdir ${BENCHMARKS})
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${bm_subdir}/CMakeLists.txt")
    add_subdirectory(${bm_subdir})
  else()
    file(GLOB tmp "${bm_subdir}/*.c")
    set(bm_files ${tmp} ${bm_files})
  endif()
endforeach()

add_library(benchmarks OBJECT ${bm_files})
