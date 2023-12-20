set(CMAKE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
if(NOT CMAKE_EXPORT_COMPILE_COMMANDS)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
endif()
set(CMAKE_VERBOSE_MAKEFILE ON)

set(WANTS_CMAKE_C_COMPILER "/usr/bin/gcc")
set(WANTS_CMAKE_CXX_COMPILER "/usr/bin/g++")
#set(CMAKE_C_COMPILER )
#set(CMAKE_CXX_COMPILER "/usr/bin/g++")

####################################################

macro(recurse_include_dir TOP_DIRNAME)
    file(GLOB TMP_FILE_LIST ${TOP_DIRNAME}/*.h;${TOP_DIRNAME}/*.hpp)
    if(TMP_FILE_LIST)
        include_directories(${TOP_DIRNAME})
    endif()
    
    file(GLOB TMP_DIR_LIST ${TOP_DIRNAME}/*)
    foreach(TMP_DIR ${TMP_DIR_LIST})
        if(IS_DIRECTORY ${TMP_DIR})
            recurse_include_dir(${TMP_DIR})
        endif()    
    endforeach()
endmacro()

macro(recurse_src_list TOP_DIRNAME SRC_LIST_VARIABLE TYPE_LIST_VARIABLE)
    foreach(TYPE ${${TYPE_LIST_VARIABLE}})
        file(GLOB_RECURSE TMP_FILE_LIST "${TOP_DIRNAME}/${TYPE}")
        list(APPEND ${SRC_LIST_VARIABLE} ${TMP_FILE_LIST})
    endforeach()
endmacro()

####################################################


####################################################
set(LOCAL_COMMON ${LOCAL_PROJECT_DIR}/common)

set(SRC_TYPE_LIST *.c)
recurse_src_list(${LOCAL_COMMON} C_SRC_LIST SRC_TYPE_LIST)
#aux_source_directory(${LOCAL_COMMON} C_SRC_LIST)
recurse_include_dir(${LOCAL_COMMON})

add_compile_options(
    -O0
    -g
    -Wall
    -Werror
)

add_compile_definitions(
    LOCAL_DEBUG
    THREADPOOL
    SAFE_API
)

include_directories(
    $ENV{MY_INC_PATH}
)

link_directories(
    $ENV{MY_LD_PATH}
)

link_libraries(
    Dbgout
    pthread
)