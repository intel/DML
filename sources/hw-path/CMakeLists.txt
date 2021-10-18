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

add_library(hw_path OBJECT
    src/own_submit_hardware_path.c
    src/hw_completion_records/own_hardware_get_update_dif_result.c
    src/hw_completion_records/own_hardware_get_strip_dif_result.c
    src/hw_completion_records/own_hardware_get_nop_result.c
    src/hw_completion_records/own_hardware_get_mem_move_result.c
    src/hw_completion_records/own_hardware_get_insert_dif_result.c
    src/hw_completion_records/own_hardware_get_fill_result.c
    src/hw_completion_records/own_hardware_get_dualcast_result.c
    src/hw_completion_records/own_hardware_get_drain_result.c
    src/hw_completion_records/own_hardware_get_delta_create_result.c
    src/hw_completion_records/own_hardware_get_delta_apply_result.c
    src/hw_completion_records/own_hardware_get_crc_result.c
    src/hw_completion_records/own_hardware_get_crc_copy_result.c
    src/hw_completion_records/own_hardware_get_compare_result.c
    src/hw_completion_records/own_hardware_get_check_dif_result.c
    src/hw_completion_records/own_hardware_get_cache_flush_result.c
    src/hw_completion_records/own_hardware_get_batch_result.c
    src/hw_config/hw_configuration_driver.c
    src/hw_config/own_hardware_configure.c
    src/hw_descriptors/own_hardware_init_batch_descriptor.c
    src/hw_descriptors/own_hardware_init_cache_flush_descriptor.c
    src/hw_descriptors/own_hardware_init_update_dif_descriptor.c
    src/hw_descriptors/own_hardware_init_strip_dif_descriptor.c
    src/hw_descriptors/own_hardware_init_nop_descriptor.c
    src/hw_descriptors/own_hardware_init_mem_move_descriptor.c
    src/hw_descriptors/own_hardware_init_insert_dif_descriptor.c
    src/hw_descriptors/own_hardware_init_fill_descriptor.c
    src/hw_descriptors/own_hardware_init_dualcast_descriptor.c
    src/hw_descriptors/own_hardware_init_drain_descriptor.c
    src/hw_descriptors/own_hardware_init_delta_create_descriptor.c
    src/hw_descriptors/own_hardware_init_delta_apply_descriptor.c
    src/hw_descriptors/own_hardware_init_crc_descriptor.c
    src/hw_descriptors/own_hardware_init_compare_pattern_descriptor.c
    src/hw_descriptors/own_hardware_init_crc_copy_descriptor.c
    src/hw_descriptors/own_hardware_init_compare_descriptor.c
    src/hw_descriptors/own_hardware_init_check_dif_descriptor.c
    )

target_include_directories(hw_path
        PUBLIC include
        PRIVATE src/hw_descriptors/include
        PRIVATE src/hw_completion_records/include
        PRIVATE src/hw_config/include
        PRIVATE src/include
        PRIVATE ../../include)

target_compile_features(hw_path PRIVATE c_std_11)

target_compile_definitions(hw_path PRIVATE DML_BADARG_CHECK
                                   PRIVATE $<$<BOOL:${LIB_ACCEL_3_2}>: LIB_ACCEL_VERSION_3_2>)
