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
        # Core sources
        src/software_device.cpp
        src/hardware_device.cpp
        src/nop.cpp
        src/batch.cpp
        src/drain.cpp
        src/mem_move.cpp
        src/fill.cpp
        src/compare.cpp
        src/compare_pattern.cpp
        src/create_delta.cpp
        src/apply_delta.cpp
        src/dualcast.cpp
        src/crc.cpp
        src/copy_crc.cpp
        src/dif_check.cpp
        src/dif_insert.cpp
        src/dif_strip.cpp
        src/dif_update.cpp
        src/cache_flush.cpp
        src/kernels.hpp
        src/validation.cpp

        include/core/operations.hpp
        include/core/descriptor_views.hpp
        include/core/completion_record_views.hpp
        include/core/validation.hpp
        include/core/device.hpp
        include/core/types.hpp
        include/core/view.hpp
        include/core/utils.hpp
        )

target_link_libraries(dml_core
        PRIVATE dml_hw_dispatcher
        PRIVATE dml_sw_dispatcher
        PRIVATE dml_dif_impl
        )
target_include_directories(dml_core
        PUBLIC include
        PUBLIC ../../include
        )
target_sources(dml_core
        PUBLIC $<TARGET_OBJECTS:dml_sw_dispatcher>
        PUBLIC $<TARGET_PROPERTY:dml_sw_dispatcher,INTERFACE_SOURCES>

        PUBLIC $<TARGET_OBJECTS:dml_hw_dispatcher>
        PUBLIC $<TARGET_PROPERTY:dml_hw_dispatcher,INTERFACE_SOURCES>

        PUBLIC $<TARGET_OBJECTS:dml_dif_impl>
        PUBLIC $<TARGET_PROPERTY:dml_dif_impl,INTERFACE_SOURCES>
        )
target_compile_features(dml_core
        PUBLIC cxx_std_17
        )
target_compile_options(dml_core
        PRIVATE ${DML_QUALITY_OPTIONS}
        PRIVATE ${DML_CPP_PRIVATE_OPTIONS}
        )
target_compile_definitions(dml_core
        PRIVATE $<TARGET_PROPERTY:dml_hw_dispatcher,INTERFACE_COMPILE_DEFINITIONS>
        )

if (DML_HW)
    target_compile_definitions(dml_core PRIVATE DML_HW)
endif ()

add_subdirectory(src/sw_dispatcher)
add_subdirectory(src/hw_dispatcher)
add_subdirectory(src/dif_impl)
