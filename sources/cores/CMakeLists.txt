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

add_library(dml_core OBJECT
    src/dmlc_fill_8u.c
    src/dmlc_delta_record_8u.c
    src/dmlc_crc_16u_32u.c
    src/dmlc_copy_8u.c
    src/dmlc_compare_8u.c
    src/dmlc_cache_8u.c
    )

target_include_directories(dml_core
    PUBLIC include
    PRIVATE ../../include
    PRIVATE src/include
    )

target_compile_features(dml_core PRIVATE c_std_11)

# TODO: target_compile_options(dml_core PRIVATE ${DML_QUALITY_OPTIONS})

target_compile_definitions(dml_core PRIVATE DML_CORES_BADARG_CHECK)

# TODO: Remove
if ("${DML_ARCH}" STREQUAL "avx512")
    target_compile_options(dml_core PRIVATE ${DML_AVX512_OPTIONS})
    target_compile_definitions(dml_core PRIVATE AVX512)
else()
    target_compile_definitions(dml_core PRIVATE PX)
endif()
