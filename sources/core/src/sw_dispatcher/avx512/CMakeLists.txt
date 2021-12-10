#
# Copyright 2021 Intel Corporation.
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
