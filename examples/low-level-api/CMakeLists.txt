# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)

project(dml_ll_examples C)

add_subdirectory(samples)

add_executable(dml_example_c_api_multisocket multisocket.c)
target_link_libraries(dml_example_c_api_multisocket PRIVATE dml)
target_compile_options(dml_example_c_api_multisocket PRIVATE ${DML_SECURITY_OPTIONS})