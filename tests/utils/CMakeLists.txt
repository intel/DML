# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT

add_library(dml_testing_utils STATIC
        src/memory.cpp
        )

target_include_directories(dml_testing_utils PUBLIC include)

target_compile_options(dml_testing_utils PRIVATE ${DML_QUALITY_OPTIONS})

target_compile_features(dml_testing_utils PUBLIC cxx_std_17)
