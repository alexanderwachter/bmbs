if(NOT DEFINED ARCH)
	message(WARNING "no ARCH defined. Use generic")
	set(ARCH "generic")
endif()

# add arch specific code
add_subdirectory(${ARCH})
