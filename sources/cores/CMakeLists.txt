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
project(dml_core C)

file(GLOB DML_CORE_SRC src/*.c)

foreach(arch px avx512)
    add_library(dml_core_${arch} OBJECT ${DML_CORE_SRC})

    target_include_directories(dml_core_${arch}
                               PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                               PUBLIC $<TARGET_PROPERTY:dml,INTERFACE_INCLUDE_DIRECTORIES>
                               PRIVATE src/include)

    target_compile_features(dml_core_${arch} PRIVATE c_std_11)

    target_compile_definitions(dml_core_${arch} PRIVATE DML_CORES_BADARG_CHECK)
endforeach()

target_compile_options(dml_core_px
    PRIVATE $<$<C_COMPILER_ID:GNU>:-mavx2>
    PRIVATE $<$<C_COMPILER_ID:MSVC>:/arch:AVX2>)
target_compile_definitions(dml_core_px PRIVATE PX)

target_compile_options(dml_core_avx512
    PRIVATE $<$<C_COMPILER_ID:GNU>:-march=skylake-avx512 -mavx512dq -mavx512vl -mavx512bw -mclflushopt -mclwb>
    PRIVATE $<$<C_COMPILER_ID:MSVC>:/arch:AVX512>)
target_compile_definitions(dml_core_avx512 PRIVATE AVX512)

if ("${DML_ARCH}" STREQUAL "avx512")
    add_library(dml_core ALIAS dml_core_avx512)
else()
    add_library(dml_core ALIAS dml_core_px)
endif()
