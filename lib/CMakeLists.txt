set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
file(GLOB LIB_SRC "*.c")
add_library(lib STATIC ${LIB_SRC})
