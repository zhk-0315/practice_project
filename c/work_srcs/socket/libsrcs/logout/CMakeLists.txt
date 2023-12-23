cmake_minimum_required(VERSION 3.20)
project(LClogout C)

list(APPEND LIB_SRC_TOP_DIR_LIST ${CMAKE_CURRENT_SOURCE_DIR})

foreach(TOP_DIR IN LISTS LIB_SRC_TOP_DIR_LIST)
    recurse_add_include_dir(${TOP_DIR})
    recurse_src_list(${TOP_DIR} BIN_SRC_LIST *.c)
endforeach()

add_library(${PROJECT_NAME} STATIC ${LIB_SRC_TOP_DIR_LIST})
