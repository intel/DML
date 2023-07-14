# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)

project(dml_ll_examples C)

file(GLOB files "*.c")
foreach(source_file ${files})
    # for each source file, remove path and extension, and prepend "ll_"
    get_filename_component(example_name ${source_file} NAME_WE)
    set(example_name "ll_${example_name}")

    # create executable
    add_executable(${example_name} ${source_file})
    target_link_libraries(${example_name} PRIVATE dml)

    if(WIN32)
        target_compile_options(${example_name} PRIVATE "$<$<CONFIG:Release>:-O2>" /WX)
    else()
        target_compile_options(${example_name} PRIVATE "$<$<CONFIG:Release>:-O3>" -Werror -Wall)
    endif()

endforeach()
