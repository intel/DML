# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)

add_subdirectory(dml_job_api)
add_subdirectory(high_level_api)

add_executable(dml_example_c_api_multisocket multisocket.c)
target_link_libraries(dml_example_c_api_multisocket PRIVATE dml)
target_compile_options(dml_example_c_api_multisocket PRIVATE ${DML_SECURITY_OPTIONS})

add_executable(dml_example_c_api_perftest perftest.c)
target_link_libraries(dml_example_c_api_perftest PRIVATE dml pthread)
target_compile_options(dml_example_c_api_perftest PRIVATE ${DML_SECURITY_OPTIONS})

add_executable(dml_example_cpp_api_multisocket multisocket.cpp)
target_link_libraries(dml_example_cpp_api_multisocket PRIVATE dmlhl)
target_compile_options(dml_example_cpp_api_multisocket PRIVATE ${DML_SECURITY_OPTIONS})
