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

project(dml_middle_layer CXX)

add_library(dml_middle_layer OBJECT
        src/operation.cpp
        src/result.cpp
        src/execution_path.cpp
        src/validation.cpp

        ../../include/dml/detail/ml/options.hpp
        ../../include/dml/detail/ml/operation.hpp
        ../../include/dml/detail/ml/result.hpp
        ../../include/dml/detail/ml/execution_path.hpp
        ../../include/dml/detail/ml/validation.hpp
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
