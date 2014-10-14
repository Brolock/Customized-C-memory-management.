cmake_minimum_required(VERSION 2.6)

project(test_nq_memlib)

set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
set(EXECUTABLE_OUTPUT_PATH ${ROOT_DIR}/tests/bin/${CMAKE_BUILD_TYPE})

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++11)
endif()

include_directories(${ROOT_DIR}/include)
link_directories(${ROOT_DIR}/lib/${CMAKE_BUILD_TYPE})

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
