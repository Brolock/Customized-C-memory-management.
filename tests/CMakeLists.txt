cmake_minimum_required(VERSION 2.6)

if (${COMPILE_WITH_LOG})
    if(${LOG_WITH_TIME})
        set (SUFFIX_LOG "_lt")
        add_definitions(-DWITH_NQ_MEMLOG -DWITH_NQ_LOGTIME)
    else()
        set(SUFFIX_LOG "_l")
        add_definitions(-DWITH_NQ_MEMLOG)
    endif()
endif()

project(test_nq_memlib)

set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
set(EXECUTABLE_OUTPUT_PATH ${ROOT_DIR}/tests/bin)

set(COMPILE_WITH_LOG CACHE BOOL "compiling with log")
set(LOG_WITH_TIME CACHE BOOL "log with time")

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++11)
endif()

include_directories(${ROOT_DIR}/include)
link_directories(${ROOT_DIR}/lib/)

# executable declaration
file(
    GLOB_RECURSE
    source_files
    source/*
)

add_executable(
    test_nq_memlib
    ${source_files}
)

target_link_libraries(test_nq_memlib
    debug nq_mem${SUFFIX_LOG}_d
    optimized nq_mem${SUFFIX_LOG}
    )
