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
    # Sources
    awaiter.cpp
    core.cpp
    )

target_include_directories(dml_middle_layer
    PRIVATE ../../include
    PRIVATE ../cores/include
    PRIVATE dispatcher)

target_compile_features(dml_middle_layer PUBLIC cxx_std_17)

target_compile_definitions(dml_middle_layer PRIVATE $<$<BOOL:${EFFICIENT_WAIT}>: DML_EFFICIENT_WAIT>)

if (DML_HW)
    target_sources(dml_middle_layer PRIVATE
        $<TARGET_OBJECTS:hw_path>
        dispatcher/hw_device.cpp
        dispatcher/hw_dispatcher.cpp
        dispatcher/hw_queue.cpp
        dispatcher/numa.cpp
        device.cpp
        )
    target_include_directories(dml_middle_layer PRIVATE ../hw-path/include)
    target_compile_definitions(dml_middle_layer
        PUBLIC DML_HW
        PRIVATE $<$<BOOL:${LIB_ACCEL_3_2}>: LIB_ACCEL_VERSION_3_2>)
endif ()
