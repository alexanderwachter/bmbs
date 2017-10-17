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