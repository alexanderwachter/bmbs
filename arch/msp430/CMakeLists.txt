set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
file(GLOB_RECURSE LIB_ARCH "*.c")
add_library(arch OBJECT ${LIB_ARCH})
