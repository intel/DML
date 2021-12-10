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

add_library(dml_dif_impl OBJECT
        # DIFs
        dml_dif.h
        dml_dif.c
        )

target_link_libraries(dml_dif_impl
        PRIVATE dml_sw_dispatcher
        )
target_include_directories(dml_dif_impl
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ../../../../include
        )
target_compile_features(dml_dif_impl
        PUBLIC c_std_11
        )
target_compile_options(dml_dif_impl
        PRIVATE ${DML_QUALITY_OPTIONS}
        )
