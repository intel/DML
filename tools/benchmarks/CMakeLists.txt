# ==========================================================================
# Copyright (C) 2023 Intel Corporation
#
# SPDX-License-Identifier: MIT
# ==========================================================================

cmake_minimum_required(VERSION 3.16.3 FATAL_ERROR)

add_executable(dml_benchmarks 
    src/main.cpp
    src/cases/mem_move.cpp
)
    
target_link_libraries(dml_benchmarks
    PUBLIC dmlhl dml dml_middle_layer dml_hw_dispatcher benchmark stdc++fs)

get_target_property(GBENCH_SOURCE_DIR benchmark SOURCE_DIR)

target_include_directories(dml_benchmarks
    PRIVATE ./include
    PRIVATE ${GBENCH_SOURCE_DIR})

target_compile_options(dml_benchmarks PUBLIC -Wall -march=skylake)

install(TARGETS dml_benchmarks RUNTIME DESTINATION bin)
