#
# Copyright 2021 Intel Corporation.
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

project(dml_ml CXX)

# I would like to move this to the parent project, but it will brake things
if (DML_HW)
    add_compile_options(-DDML_HW)
endif ()

add_library(dml_ml OBJECT
    source/mem_move.cpp
    source/mem_copy.cpp
    source/fill.cpp
    source/dualcast.cpp
    source/compare.cpp
    source/compare_pattern.cpp
    source/create_delta.cpp
    source/apply_delta.cpp
    source/crc.cpp
    source/copy_crc.cpp
    source/cache_flush.cpp
    source/batch.cpp
    source/operation.cpp
    source/awaiter.cpp
    dispatcher/hw_device.cpp
    dispatcher/hw_dispatcher.cpp
    dispatcher/hw_queue.cpp
    dispatcher/numa.cpp
    )
target_include_directories(dml_ml
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE $<TARGET_PROPERTY:dml_common,INTERFACE_INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:dml_core,INTERFACE_INCLUDE_DIRECTORIES>
    PRIVATE dispatcher)
target_compile_features(dml_ml PUBLIC cxx_std_17)

if (DML_HW)
    target_include_directories(dml_ml
        PRIVATE $<TARGET_PROPERTY:hw_path,INTERFACE_INCLUDE_DIRECTORIES>)
    target_sources(dml_ml PRIVATE source/hardware_path.cpp)
    target_compile_definitions(dml_ml PRIVATE DML_HW
                                      PRIVATE $<$<BOOL:${LIB_ACCEL_3_2}>: LIB_ACCEL_VERSION_3_2>
                                              PRIVATE $<$<BOOL:${EFFICIENT_WAIT}>: DML_EFFICIENT_WAIT>)
else()
    target_compile_definitions(dml_ml PRIVATE $<$<BOOL:${LIB_ACCEL_3_2}>: LIB_ACCEL_VERSION_3_2>
                                      PRIVATE $<$<BOOL:${EFFICIENT_WAIT}>: DML_EFFICIENT_WAIT>)
endif ()

add_subdirectory(common)
