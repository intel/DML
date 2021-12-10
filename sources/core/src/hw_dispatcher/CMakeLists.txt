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

add_library(dml_hw_dispatcher OBJECT
        hw_device.cpp
        hw_device.hpp
        hw_dispatcher.cpp
        hw_dispatcher.hpp
        hw_queue.cpp
        hw_queue.hpp
        numa.cpp
        numa.hpp

        hw_configuration_driver.c

        legacy_headers/hardware_configuration_driver.h
        legacy_headers/hardware_api.h
        legacy_headers/hardware_completion_records_api.h
        legacy_headers/hardware_limits.h
        legacy_headers/hardware_descriptors_api.h
        legacy_headers/own_dsa_accel_constants.h
        legacy_headers/hardware_definitions.h
        legacy_headers/libaccel_config.h
        )

target_compile_features(dml_hw_dispatcher PRIVATE cxx_std_17 c_std_11)

target_compile_options(dml_hw_dispatcher
        PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${DML_CPP_PRIVATE_OPTIONS}>
        )

target_include_directories(dml_hw_dispatcher PUBLIC ../../../../include)

if (DML_HW)
    target_compile_definitions(dml_hw_dispatcher
            PRIVATE DML_HW
            PUBLIC $<$<BOOL:${LOG_HW_INIT}>:LOG_HW_INIT>
            )
endif ()
