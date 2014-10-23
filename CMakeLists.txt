cmake_minimum_required(VERSION 2.6)

project(nq_memlib)

set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

set(LIBRARY_OUTPUT_PATH ${ROOT_DIR}/lib)

set(COMPILE_WITH_LOG CACHE BOOL "compiling with log")
set(LOG_WITH_TIME CACHE BOOL "log with time")

#define the suffix in the end of the lib name
if (${COMPILE_WITH_LOG})
    if(${LOG_WITH_TIME})
        set (SUFFIX_LOG "_lt")
        add_definitions(-DWITH_NQ_MEMLOG -DWITH_NQ_LOGTIME)
    else()
        set(SUFFIX_LOG "_l")
        add_definitions(-DWITH_NQ_MEMLOG)
    endif()
elseif (${COMPILE_WITH_MEM_OFF})
    set (SUFFIX_LOG "_off")
    add_definitions(-DWITH_NQ_MEMOFF)
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

file(
    GLOB_RECURSE
    header_files
    include/*
)

add_library(
    nq_mem
    STATIC
    ${source_files}
)

set(LibDir ${ROOT_DIR}/lib)

set_target_properties(nq_mem PROPERTIES
        DEBUG_POSTFIX "${SUFFIX_LOG}_d"
        RELEASE_POSTFIX "${SUFFIX_LOG}"
        RELWITHDEBINFO_POSTFIX "${SUFFIX_LOG}_rd"
        MINSIZEREL_POSTFIX "${SUFFIX_LOG}_rm"

#All the libs are in one single directory under windows
        RUNTIME_OUTPUT_DIRECTORY ${LibDir}
        LIBRARY_OUTPUT_DIRECTORY ${LibDir}
        ARCHIVE_OUTPUT_DIRECTORY ${LibDir}
        PDB_OUTPUT_DIRECTORY ${LibDir}
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${LibDir}
        LIBRARY_OUTPUT_DIRECTORY_DEBUG ${LibDir}
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${LibDir}
        PDB_OUTPUT_DIRECTORY_DEBUG ${LibDir}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${LibDir}
        LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LibDir}
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${LibDir}
        PDB_OUTPUT_DIRECTORY_RELEASE ${LibDir}
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${LibDir}
        LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${LibDir}
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${LibDir}
        PDB_OUTPUT_DIRECTORY_RELWITHDEBINFO ${LibDir}
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${LibDir}
        LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${LibDir}
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${LibDir}
        PDB_OUTPUT_DIRECTORY_MINSIZEREL ${LibDir}
)

install(DIRECTORY "${ROOT_DIR}/lib/"
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/nq_memlib)
install(FILES ${header_files}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/nq_memlib)
