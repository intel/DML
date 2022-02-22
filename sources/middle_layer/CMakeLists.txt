# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT

project(dml_middle_layer CXX)

add_library(dml_middle_layer OBJECT
        src/make_descriptor.cpp
        src/result.cpp
        src/core_interconnect.cpp

        ../../include/dml/detail/ml/options.hpp
        ../../include/dml/detail/ml/make_task.hpp
        ../../include/dml/detail/ml/result.hpp
        ../../include/dml/detail/ml/execution_path.hpp
        ../../include/dml/detail/ml/buffer.hpp
        ../../include/dml/detail/ml/utils.hpp
        ../../include/dml/detail/ml/task.hpp
        ../../include/dml/detail/ml/view.hpp
        ../../include/dml/detail/ml/allocator.hpp
        ../../include/dml/detail/ml/impl/core_interconnect.hpp
        ../../include/dml/detail/ml/impl/make_descriptor.hpp
        )

target_link_libraries(dml_middle_layer
        PRIVATE dml_core
        )
target_include_directories(dml_middle_layer
        PUBLIC ../../include
        )
target_sources(dml_middle_layer
        PUBLIC $<TARGET_OBJECTS:dml_core>
        PUBLIC $<TARGET_PROPERTY:dml_core,INTERFACE_SOURCES>
        )
target_compile_features(dml_middle_layer
        PUBLIC cxx_std_17
        )
target_compile_options(dml_middle_layer
        PRIVATE ${DML_QUALITY_OPTIONS}
        PRIVATE ${DML_CPP_PRIVATE_OPTIONS}
        )
