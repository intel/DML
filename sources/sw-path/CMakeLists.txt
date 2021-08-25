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

cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)
project(sw_path C)

# Find Kernels Sources
file(GLOB SW_PATH_SRC ./*.c)

# Create library
add_library(sw_path OBJECT ${SW_PATH_SRC})

# Setting external and internal interfaces for DML reference library
get_target_property(DML_SOURCE_DIR dml SOURCE_DIR)
target_include_directories(sw_path
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include
        PRIVATE $<TARGET_PROPERTY:dml,INTERFACE_INCLUDE_DIRECTORIES>
        PRIVATE $<TARGET_PROPERTY:dml_core,INTERFACE_INCLUDE_DIRECTORIES>)

target_compile_features(sw_path PRIVATE c_std_11)

target_compile_definitions(sw_path PRIVATE DML_BADARG_CHECK)
