cmake_minimum_required(VERSION 2.6)

project(nq_memlib)

set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

set(LIBRARY_OUTPUT_PATH ${ROOT_DIR}/lib/${CMAKE_BUILD_TYPE})

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
