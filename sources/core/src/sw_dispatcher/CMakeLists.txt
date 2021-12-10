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

add_library(dml_sw_dispatcher OBJECT
        dml_cpuid.h
        dml_kernels.h
        optimization_dispatcher.hpp

        optimization_dispatcher.cpp
        dml_cpuid.c
        )

target_include_directories(dml_sw_dispatcher
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        )
target_compile_features(dml_sw_dispatcher
        PUBLIC cxx_std_17
        PUBLIC c_std_11
        )
target_sources(dml_sw_dispatcher
        PUBLIC $<TARGET_OBJECTS:dml_kernels_ref>
        PUBLIC $<TARGET_OBJECTS:dml_kernels_avx512>
        PUBLIC $<TARGET_OBJECTS:dml_kernels_cache_flush>
        )
target_compile_options(dml_sw_dispatcher
        PRIVATE ${DML_QUALITY_OPTIONS}
        PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${DML_CPP_PRIVATE_OPTIONS}>
        )

if (DML_ARCH STREQUAL avx512)
    target_compile_definitions(dml_sw_dispatcher PRIVATE DML_AVX512)
endif ()

add_subdirectory(ref)
add_subdirectory(avx512)
add_subdirectory(cache_flush)
