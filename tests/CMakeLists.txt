cmake_minimum_required(VERSION 2.6)

project(test_nq_memlib)

set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
set(EXECUTABLE_OUTPUT_PATH ${ROOT_DIR}/tests/bin)

set(COMPILE_WITH_LOG CACHE BOOL "compiling with log")
set(LOG_WITH_TIME CACHE BOOL "log with time")

set(TestsDir ${ROOT_DIR}/tests/bin)

#define the suffix in the end of the lib name
if (${COMPILE_WITH_LOG})
    if(${LOG_WITH_TIME})
        set (SUFFIX_LOG "_lt")
        add_definitions(-DWITH_NQ_MEMLOG -DWITH_NQ_LOGTIME)
    else()
        set(SUFFIX_LOG "_l")
        add_definitions(-DWITH_NQ_MEMLOG)
    endif()
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++11)
endif()

include_directories(${CMAKE_INSTALL_PREFIX}/include/)
link_directories(${CMAKE_INSTALL_PREFIX}/lib/nq_memlib)

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

set_target_properties(test_nq_memlib PROPERTIES
        DEBUG_POSTFIX "${SUFFIX_LOG}_d"
        RELEASE_POSTFIX "${SUFFIX_LOG}"
        RELWITHDEBINFO_POSTFIX "${SUFFIX_LOG}_rd"
        MINSIZEREL_POSTFIX "${SUFFIX_LOG}_rm"

#All the tests files are in one single directory under windows
        RUNTIME_OUTPUT_DIRECTORY ${TestsDir}
        LIBRARY_OUTPUT_DIRECTORY ${TestsDir}
        ARCHIVE_OUTPUT_DIRECTORY ${TestsDir}
        PDB_OUTPUT_DIRECTORY ${TestsDir}
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${TestsDir}
        LIBRARY_OUTPUT_DIRECTORY_DEBUG ${TestsDir}
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${TestsDir}
        PDB_OUTPUT_DIRECTORY_DEBUG ${TestsDir}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${TestsDir}
        LIBRARY_OUTPUT_DIRECTORY_RELEASE ${TestsDir}
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${TestsDir}
        PDB_OUTPUT_DIRECTORY_RELEASE ${TestsDir}
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${TestsDir}
        LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${TestsDir}
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${TestsDir}
        PDB_OUTPUT_DIRECTORY_RELWITHDEBINFO ${TestsDir}
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${TestsDir}
        LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${TestsDir}
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${TestsDir}
        PDB_OUTPUT_DIRECTORY_MINSIZEREL ${TestsDir}
)
