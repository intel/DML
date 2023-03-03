# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(dml_hl_tests CXX)

add_library(dml_hl_tests OBJECT
    source/mem_move.cpp
    source/mem_copy.cpp
    source/dualcast.cpp
    source/fill.cpp
    source/compare.cpp
    source/compare_pattern.cpp
    source/delta.cpp
    source/crc.cpp
    source/copy_crc.cpp
    source/cache_flush.cpp
    source/batch.cpp
    source/sequence.cpp
    source/data_view.cpp
    )
target_link_libraries(dml_hl_tests PUBLIC dmlhl dml_test_utils gtest gtest_main)
target_compile_features(dml_hl_tests PUBLIC cxx_std_17)

# If you remove reference, please, fix tests for CRC somehow
target_include_directories(dml_hl_tests PUBLIC
    ../job_api_tests/reference/include
    ../job_api_tests/reference
    ../common # Use global config (path)
    ../../include
    )

target_compile_definitions(dml_hl_tests PUBLIC
    DML_FLAG_CRC_READ_SEED=0x10000u
    DML_FLAG_CRC_BYPASS_REFLECTION=0x20000u
    DML_FLAG_CRC_BYPASS_DATA_REFLECTION=0x40000u
    )
