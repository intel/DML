# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)

project(job_api_samples C)

# Finding DML job_api_samples library sources
file(GLOB DML_EXAMPLES_SRC *.c)

# Adding DML job_api_samples library target
add_executable(job_api_samples ${DML_EXAMPLES_SRC})

# Setting external and internal interfaces for DML job_api_samples library
target_include_directories(job_api_samples PRIVATE $<TARGET_PROPERTY:dml,INTERFACE_INCLUDE_DIRECTORIES>)

target_link_libraries(job_api_samples PRIVATE dml)

set_target_properties(job_api_samples PROPERTIES C_STANDARD 11)

target_compile_options(job_api_samples PUBLIC
                       "$<$<CONFIG:Debug>:>"
                       "$<$<CONFIG:Release>:>"
                       PRIVATE "$<$<C_COMPILER_ID:GNU>:${DML_GNU_SDL_COMPILE_OPTIONS}>"
                       PRIVATE "$<$<C_COMPILER_ID:MSVC>:${DML_MSVS_SDL_COMPILE_OPTIONS}>")

# Install rules
install(TARGETS job_api_samples RUNTIME DESTINATION bin)
