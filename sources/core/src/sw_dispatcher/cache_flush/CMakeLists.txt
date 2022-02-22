# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT

add_library(dml_kernels_cache_flush OBJECT
        cache_flush.c
        )

target_compile_features(dml_kernels_cache_flush PRIVATE c_std_11)

target_compile_options(dml_kernels_cache_flush PRIVATE ${DML_QUALITY_OPTIONS})

if (CMAKE_C_COMPILER_ID MATCHES GNU)
    target_compile_options(dml_kernels_cache_flush PRIVATE -mclflushopt -mclwb)
endif ()
