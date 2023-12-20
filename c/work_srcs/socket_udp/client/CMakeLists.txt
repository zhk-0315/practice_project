cmake_minimum_required(VERSION "3.20")
if(PROJECT_NAME)
    set(PROJECT_NAME udpcli)
    enable_language(C)
else()
    project(udpcli C)
endif()
if(NOT LOCAL_PROJECT_DIR)
    set(LOCAL_PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
endif()

if(NOT CMAKE_INCLUDE_DIR)
    include(${LOCAL_PROJECT_DIR}/include.cmake)
endif()

recurse_src_list(${CMAKE_CURRENT_SOURCE_DIR} C_SRC_LIST SRC_TYPE_LIST)
recurse_include_dir(${CMAKE_CURRENT_SOURCE_DIR})


add_compile_definitions(
)

add_executable(${PROJECT_NAME} ${C_SRC_LIST})
