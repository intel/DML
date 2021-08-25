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

cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)

project(dmlhl_examples CXX)

if (WIN32)
    add_compile_options(/WX /W3)
else()
    add_compile_options(-Werror -Wall)
endif()

add_executable(dmlhl_mem_move_example mem_move.cpp)
target_link_libraries(dmlhl_mem_move_example PRIVATE dmlhl)

add_executable(dmlhl_fill_example fill.cpp)
target_link_libraries(dmlhl_fill_example PRIVATE dmlhl)

add_executable(dmlhl_dualcast_example dualcast.cpp)
target_link_libraries(dmlhl_dualcast_example PRIVATE dmlhl)

add_executable(dmlhl_compare_example compare.cpp)
target_link_libraries(dmlhl_compare_example PRIVATE dmlhl)

add_executable(dmlhl_compare_pattern_example compare_pattern.cpp)
target_link_libraries(dmlhl_compare_pattern_example PRIVATE dmlhl)

add_executable(dmlhl_delta_example delta.cpp)
target_link_libraries(dmlhl_delta_example PRIVATE dmlhl)

add_executable(dmlhl_crc_example crc.cpp)
target_link_libraries(dmlhl_crc_example PRIVATE dmlhl)

add_executable(dmlhl_copy_crc_example copy_crc.cpp)
target_link_libraries(dmlhl_copy_crc_example PRIVATE dmlhl)

add_executable(dmlhl_cache_flush_example cache_flush.cpp)
target_link_libraries(dmlhl_cache_flush_example PRIVATE dmlhl)
