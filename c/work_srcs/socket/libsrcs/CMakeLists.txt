cmake_minimum_required(VERSION 3.20)
project(libs)

if(NOT LC_PROJECT_DIR)
    set(LC_PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
endif()

if(NOT CMAKE_INCLUDE_FILE)
    include(${LC_PROJECT_DIR}/inc.cmake)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/logout)
