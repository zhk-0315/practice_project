cmake_minimum_required(VERSION "3.20")

project(socket_udp NONE)

if(NOT LOCAL_PROJECT_DIR)
    set(LOCAL_PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

if(NOT CMAKE_INCLUDE_DIR)
    include(${LOCAL_PROJECT_DIR}/include.cmake)
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/client)
add_subdirectory(${PROJECT_SOURCE_DIR}/server)
