# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT

add_library(dml_kernels_avx512 OBJECT
        mem_move.c
        fill.c
        compare.c
        compare_pattern.c
        crc.c
        )

target_compile_features(dml_kernels_avx512 PRIVATE c_std_11)

target_compile_options(dml_kernels_avx512 PRIVATE ${DML_QUALITY_OPTIONS})

if (CMAKE_C_COMPILER_ID MATCHES GNU)
    target_compile_options(dml_kernels_avx512 PRIVATE -march=skylake-avx512)
endif ()

if (CMAKE_C_COMPILER_ID MATCHES MSVC)
    target_compile_options(dml_kernels_avx512 PRIVATE /arch:AVX512)
endif ()
