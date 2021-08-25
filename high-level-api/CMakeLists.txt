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

project(dmlhl CXX)

# I would like to move this to the parent project, but it will brake things
add_library(dmlhl STATIC
    $<TARGET_OBJECTS:dml_ml>
    $<TARGET_OBJECTS:dml_core>)
target_include_directories(dmlhl
    PUBLIC $<INSTALL_INTERFACE:include>
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<TARGET_PROPERTY:dml_ml,INTERFACE_INCLUDE_DIRECTORIES>
    PUBLIC $<TARGET_PROPERTY:dml_common,INTERFACE_INCLUDE_DIRECTORIES>
    )
target_compile_features(dmlhl PUBLIC cxx_std_17)

if (DML_HW)
    target_link_libraries(dmlhl PRIVATE ${CMAKE_DL_LIBS})
    target_sources(dmlhl PRIVATE $<TARGET_OBJECTS:hw_path>)
    target_compile_definitions(dmlhl PUBLIC DML_HW)
endif ()
