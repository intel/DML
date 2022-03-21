# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(dml_test_utils CXX)

add_library(dml_test_utils STATIC
    source/allocator.cpp
    source/data_storage.cpp
    )
target_include_directories(dml_test_utils PUBLIC include)
target_compile_features(dml_test_utils PUBLIC cxx_std_17)
