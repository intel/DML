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
