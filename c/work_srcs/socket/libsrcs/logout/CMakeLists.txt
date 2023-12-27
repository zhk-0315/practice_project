cmake_minimum_required(VERSION 3.20)
project(LClogout LANGUAGES C)

set_lib_compile_env()
list(APPEND SRC_TOP_DIR_LIST ${CMAKE_CURRENT_SOURCE_DIR})

foreach(TOP_DIR IN LISTS SRC_TOP_DIR_LIST)
    recurse_add_include_dir(${TOP_DIR})
    recurse_src_list(${TOP_DIR} SRC_LIST *.c)
endforeach()

add_library(${PROJECT_NAME} STATIC ${SRC_LIST})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${INC_DIR_LIST}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${LIBS_LIST}
)
