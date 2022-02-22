# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT

add_library(dml_kernels_ref OBJECT
        mem_move.c
        fill.c
        compare.c
        compare_pattern.c
        create_delta.c
        apply_delta.c
        dualcast.c
        crc.c
        )

target_compile_features(dml_kernels_ref PRIVATE c_std_11)

target_compile_options(dml_kernels_ref PRIVATE ${DML_QUALITY_OPTIONS})

if (CMAKE_C_COMPILER_ID MATCHES GNU)
    target_compile_options(dml_kernels_ref PRIVATE -mavx2)
endif ()

if (CMAKE_C_COMPILER_ID MATCHES MSVC)
    target_compile_options(dml_kernels_ref PRIVATE /arch:AVX2)
endif ()
