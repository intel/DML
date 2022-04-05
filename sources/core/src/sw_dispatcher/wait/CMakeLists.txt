# Copyright (C) 2022 Intel Corporation
#
# SPDX-License-Identifier: MIT

add_library(dml_kernels_wait OBJECT
        wait.cpp
        )

target_compile_features(dml_kernels_wait PRIVATE c_std_11)

target_compile_options(dml_kernels_wait PRIVATE ${DML_QUALITY_OPTIONS})
