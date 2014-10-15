cmake_minimum_required(VERSION 2.6)

project(nq_memlib)

set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

set(LIBRARY_OUTPUT_PATH ${ROOT_DIR}/lib)

set(COMPILE_WITH_LOG CACHE BOOL "compiling with log")
set(LOG_WITH_TIME CACHE BOOL "log with time")

if (${COMPILE_WITH_LOG})
    if(${LOG_WITH_TIME})
        set (SUFFIX_LOG "_lt")
        add_definitions(-DWITH_NQ_MEMLOG -DWITH_NQ_TIME)
    else()
        set(SUFFIX_LOG "_l")
        add_definitions(-DWITH_NQ_MEMLOG)
    endif()
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++11)
endif()

include_directories(
    ${ROOT_DIR}/include
)

file(
    GLOB_RECURSE
    source_files
    source/*
    include/*
)

add_library(
    nq_mem
    STATIC
    ${source_files}
)

set_target_properties(nq_mem PROPERTIES
        DEBUG_POSTFIX "${SUFFIX_LOG}_d"
        RELEASE_POSTFIX "${SUFFIX_LOG}"
        RELWITHDEBINFO_POSTFIX "${SUFFIX_LOG}_rdi"
        MINSIZEREL_POSTFIX "${SUFFIX_LOG}_ms"
    )
