set(CMAKE_INCLUDE_FILE "${CMAKE_CURRENT_LIST_FILE}")
set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# ####################################################
set(COMMON_DIR "${LC_PROJECT_DIR}/common")
set(LIBRARY_OUTPUT_PATH "${LC_PROJECT_DIR}/libsrcs/lib")

# ####################################################
macro(recurse_add_include_dir MACRO_TOP_DIR)
    file(GLOB LC_FILE_LIST ${MACRO_TOP_DIR}/*.h;${MACRO_TOP_DIR}/*.hpp)

    if(LC_FILE_LIST)
        include_directories(${MACRO_TOP_DIR})
    endif()

    file(GLOB LC_ITEM_LIST ${MACRO_TOP_DIR}/*)

    foreach(LC_ITEM IN LISTS LC_ITEM_LIST)
        if(IS_DIRECTORY ${LC_ITEM})
            recurse_add_include_dir(${LC_ITEM})
        endif()
    endforeach()
endmacro()

macro(recurse_src_list MACRO_TOP_DIR _SRC_LIST_VAR ALL_TYPE_LIST)
    set(LC_TYPE_LIST ${ARGV})
    list(FILTER LC_TYPE_LIST INCLUDE REGEX [\*][\.][a-z|A-Z|0-9]+)

    foreach(LC_TYPE IN LISTS LC_TYPE_LIST)
        file(GLOB_RECURSE LC_FILE_LIST ${MACRO_TOP_DIR}/${LC_TYPE})
        list(APPEND ${_SRC_LIST_VAR} ${LC_FILE_LIST})
    endforeach()
endmacro()

# ####################################################
list(APPEND BIN_SRC_TOP_DIR_LIST ${COMMON_DIR})

add_compile_options(
    -g
    -Wall
    -O0

    # -Werror
)