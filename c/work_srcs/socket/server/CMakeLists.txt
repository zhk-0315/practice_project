cmake_minimum_required(VERSION 3.20)
project(srv C)

if(NOT LC_PROJECT_DIR)
    set(LC_PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
endif()

if(NOT CMAKE_INCLUDE_FILE)
    include(${LC_PROJECT_DIR}/inc.cmake)
endif()

set_bin_compile_env()
list(APPEND SRC_TOP_DIR_LIST ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND LIBS_LIST -lsqlite3)

foreach(TOP_DIR IN LISTS SRC_TOP_DIR_LIST)
    recurse_add_include_dir(${TOP_DIR})
    recurse_src_list(${TOP_DIR} SRC_LIST *.c)
endforeach()

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${INC_DIR_LIST}
)

target_link_directories(${PROJECT_NAME} PUBLIC
    ${LIBS_DIRS_LIST}
)
target_link_libraries(${PROJECT_NAME} PUBLIC
    ${LIBS_LIST}
)
