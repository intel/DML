# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT

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
