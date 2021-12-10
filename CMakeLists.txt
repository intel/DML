#
# Copyright 2020-2021 Intel Corporation.
#
# This software and the related documents are Intel copyrighted materials,
# and your use of them is governed by the express license under which they
# were provided to you ("License"). Unless the License provides otherwise,
# you may not use, modify, copy, publish, distribute, disclose or transmit
# this software or the related documents without Intel's prior written
# permission.
#
# This software and the related documents are provided as is, with no
# express or implied warranties, other than those that are expressly
# stated in the License.
#

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(Dml VERSION 0.1.6 LANGUAGES C CXX)

set(PROJECT_SOVERSION 0)

option(DML_HW "Build hardware path or not" OFF)
option(DML_SUPPRESS_WARNINGS "Suppress all compiler warnings" OFF)

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    option(DML_RECORD_SWITCHES "Enables -frecord-gcc-switches flag" OFF)
else()
    set(DML_RECORD_SWITCHES OFF CACHE BOOL "Disable recording of switches for non-gnu compiler" FORCE)
endif()

# TODO: Remove all options below
option(LOG_HW_INIT "Enables HW initialization log" OFF)
option(EFFICIENT_WAIT "Enables usage of umonitor/umwait" OFF)

include(cmake/CompileOptions.cmake)
include(cmake/utils.cmake)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE is unset, defaulting to Release")
    set(CMAKE_BUILD_TYPE Release)
endif ()

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    include(cmake/msvc.cmake)
elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    include(cmake/gnu.cmake)
else ()
    message(FATAL_ERROR "Unknown compiler ID: ${CMAKE_C_COMPILER_ID}")
endif ()

# Project-wide settings
add_compile_options(
    ${DML_SECURITY_OPTIONS}
    $<$<BOOL:${DML_RECORD_SWITCHES}>:-frecord-gcc-switches>)
add_compile_definitions(${DML_SECURITY_DEFINITIONS})

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# TODO: Remove when option is removed
if (DML_HW)
    message(STATUS "HW path: ON")
    message(STATUS "Hardware initialization logging: ${LOG_HW_INIT}")
endif()

# TODO: Remove
get_git_revision()

add_subdirectory(sources)
add_subdirectory(examples)

# Install rules
install(EXPORT ${PROJECT_NAME}Targets
        NAMESPACE Dml::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    "include(\${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}Targets.cmake)\n")

write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN *.h PATTERN *.hpp)
