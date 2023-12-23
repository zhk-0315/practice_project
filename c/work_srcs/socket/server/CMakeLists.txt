cmake_minimum_required(VERSION 3.20)
project(Srv C)

if(NOT LC_PROJECT_DIR)
    set(LC_PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
endif()

if(NOT CMAKE_INCLUDE_FILE)
    include(${LC_PROJECT_DIR}/inc.cmake)
endif()

list(APPEND BIN_SRC_TOP_DIR_LIST ${CMAKE_CURRENT_SOURCE_DIR})

foreach(TOP_DIR IN LISTS BIN_SRC_TOP_DIR_LIST)
    recurse_add_include_dir(${TOP_DIR})
    recurse_src_list(${TOP_DIR} BIN_SRC_LIST *.c)
endforeach()

add_executable(${PROJECT_NAME} ${BIN_SRC_LIST})