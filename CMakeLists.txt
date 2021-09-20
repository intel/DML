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
project(dml VERSION 0.1.3 LANGUAGES C CXX)

set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)

set(LIBRARY_SONAME 0)
set(LIBRARY_NAME dml)

option(DML_HW "Build hardware path or not" OFF)
# TODO: See todo below
option(DML_RECORD_SWITCHES "Enables -frecord-gcc-switches flag" OFF)
option(LIB_ACCEL_3_2 "Use libaccel-3.2" OFF)
option(LOG_HW_INIT "Enables HW initialization log" OFF)
option(EFFICIENT_WAIT "Enables usage of umonitor/umwait" OFF)

include(cmake/CompileOptions.cmake)
include(cmake/git_revision.cmake)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    message(STATUS "CMAKE_BUILD_TYPE is unset, defaulting to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif ()

if (MSVC)
    add_compile_options(
        # Security
        /GS

        # Quality
#        /WX
        # TODO: There are several warning to consider, also MSVC warning level can be set only via CMAKE_CXX_FLAGS
#        /Wall
    )
else()
    add_compile_options(
        # Security
        -fPIC
        -fstack-protector
        --param=ssp-buffer-size=8
        -Wl,-z,relro,-z,now
        -Wl,-z,noexecstack
        -fstack-clash-protection
#        -frecord-gcc-switches TODO: seems unnecessary

        # Quality
        # TODO: I can't fix all of them in one go (>100)
#        -Werror -Wall -Wextra -pedantic
        )

    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        add_compile_definitions(_FORTIFY_SOURCE=2)
    endif ()

    # TODO: They were already included into SDL options, why do we have the choice?
    if (DML_RECORD_SWITCHES)
        add_compile_options(-frecord-gcc-switches)
    endif ()
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Finding Intel DML library sources
file(GLOB DML_C_SRC sources/*.c)

# Adding Intel DML reference library target
add_library(dml ${DML_C_SRC} $<TARGET_OBJECTS:dml_core>
            $<TARGET_OBJECTS:sw_path>
            $<$<BOOL:$<TARGET_PROPERTY:ENABLE_HW_PATH>>:$<TARGET_OBJECTS:hw_path>>)

target_link_libraries(dml ${CMAKE_DL_LIBS})

# Setting external and internal interfaces for DML reference library
target_include_directories(dml
                           PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include/dml>
                           PRIVATE $<TARGET_PROPERTY:dml_core,INTERFACE_INCLUDE_DIRECTORIES>
                           PRIVATE $<$<BOOL:$<TARGET_PROPERTY:ENABLE_HW_PATH>>:$<TARGET_PROPERTY:hw_path,INTERFACE_INCLUDE_DIRECTORIES>>
                           PRIVATE sources/include)

if (DML_HW)
    message(STATUS "HW path: ON")
endif()

target_compile_features(dml PRIVATE c_std_11)

set_target_properties(dml PROPERTIES ENABLE_HW_PATH ${DML_HW})

target_compile_definitions(dml PUBLIC DML
                           PRIVATE DML_BADARG_CHECK
                           PRIVATE $<$<BOOL:$<TARGET_PROPERTY:ENABLE_HW_PATH>>: DML_HW>)

get_git_revision()

# Internal libraries
add_subdirectory(sources)
add_subdirectory(middle-layer)
add_subdirectory(high-level-api)

# Testing
add_subdirectory(examples)

# Configuration files
generate_export_header(dml)

# Install rules
install(TARGETS dml dml_ml dml_common dmlhl
    EXPORT DmlConfig
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include)

export(TARGETS dml dml_ml dml_common dmlhl
       NAMESPACE Dml::
       FILE "${CMAKE_CURRENT_BINARY_DIR}/DmlConfig.cmake")

install(EXPORT DmlConfig
        NAMESPACE Dml::
        DESTINATION ${CMAKE_INSTALL_PREFIX}/dml/cmake)

write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/DmlConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

set_target_properties(${LIBRARY_NAME} PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${LIBRARY_SONAME}
)


install(FILES "${PROJECT_BINARY_DIR}/DmlConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_PREFIX}/dml/cmake)

install(DIRECTORY include
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/dml
        FILES_MATCHING PATTERN *.h)

install(DIRECTORY high-level-api/include/dml
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/
        FILES_MATCHING PATTERN *.hpp)

install(DIRECTORY middle-layer/common/include/dml_common
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/
        FILES_MATCHING PATTERN *.hpp)

install(DIRECTORY middle-layer/include/dml_ml
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/
        FILES_MATCHING PATTERN *.hpp)
