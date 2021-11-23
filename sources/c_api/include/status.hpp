/*
 * Copyright 2021 Intel Corporation.
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they
 * were provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit
 * this software or the related documents without Intel's prior written
 * permission.
 *
 * This software and the related documents are provided as is, with no
 * express or implied warranties, other than those that are expressly
 * stated in the License.
 *
 */

#ifndef DML_STATUS_HPP
#define DML_STATUS_HPP

#include <dml/dml.h>

#include <dml/cpp/middle_layer/status.hpp>

namespace dml
{
    inline dml_status_t to_own_status(ml::execution_status status) noexcept
    {
        switch (status)
        {
            case ml::execution_status::success:
                {
                    return DML_STATUS_OK;
                }
            case ml::execution_status::false_predicate_success:
                {
                    return DML_STATUS_FALSE_PREDICATE_OK;
                }
            case ml::execution_status::page_fault_during_processing:
                {
                    return DML_STATUS_PAGE_FAULT_ERROR;
                }
            case ml::execution_status::page_response_error:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case ml::execution_status::batch_error:
                {
                    return DML_STATUS_BATCH_ERROR;
                }
            case ml::execution_status::batch_page_fault_error:
                {
                    return DML_STATUS_PAGE_FAULT_ERROR;
                }
            case ml::execution_status::offset_order_error:
                {
                    return DML_STATUS_DELTA_ASCENDT_ERROR;
                }
            case ml::execution_status::offset_overflow:
                {
                    return DML_STATUS_DELTA_OFFSET_ERROR;
                }
            case ml::execution_status::dif_control_error:
                {
                    return DML_STATUS_DIF_CHECK_ERROR;
                }
            case ml::execution_status::operation_error:
                {
                    return DML_STATUS_JOB_OPERATION_ERROR;
                }
            case ml::execution_status::flag_error:
                {
                    return DML_STATUS_JOB_FLAGS_ERROR;
                }
            case ml::execution_status::non_zero_reserved_field_error:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case ml::execution_status::invalid_transfer_size_error:
                {
                    return DML_STATUS_JOB_LENGTH_ERROR;
                }
            case ml::execution_status::descriptor_count_error:
                {
                    return DML_STATUS_BATCH_SIZE_ERROR;
                }
            case ml::execution_status::delta_size_error:
                {
                    return DML_STATUS_DELTA_INPUT_SIZE_ERROR;
                }
            case ml::execution_status::buffers_overlap:
                {
                    return DML_STATUS_OVERLAPPING_BUFFER_ERROR;
                }
            case ml::execution_status::dualcast_misalign_error:
                {
                    return DML_STATUS_DUALCAST_ALIGN_ERROR;
                }
            case ml::execution_status::descriptor_list_align_error:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case ml::execution_status::invalid_interrupt_handle:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case ml::execution_status::page_fault_on_translation:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case ml::execution_status::completion_record_align_error:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case ml::execution_status::misalign_address_error:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case ml::execution_status::privilege_error:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case ml::execution_status::traffic_class_error:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case ml::execution_status::readback_translation_error:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case ml::execution_status::operation_readback_timeout:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case ml::execution_status::hardware_timeout:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case ml::execution_status::address_translation_error:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            default:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
        }
    }

    inline dml_status_t to_own_status(ml::submission_status status) noexcept
    {
        switch (status)
        {
            case ml::submission_status::success:
                return DML_STATUS_OK;
            case ml::submission_status::failure:
                return DML_STATUS_INSTANCE_NOT_FOUND;
            default:
                return DML_STATUS_INTERNAL_ERROR;
        }
    }

    inline dml_status_t to_own_status(ml::validation_status status) noexcept
    {
        switch (status)
        {
            case ml::validation_status::success:
                return DML_STATUS_OK;
            case ml::validation_status::address_is_null:
                return DML_STATUS_NULL_POINTER_ERROR;
            case ml::validation_status::size_is_null:
                return DML_STATUS_JOB_LENGTH_ERROR;
            case ml::validation_status::delta_size_is_wrong:
                return DML_STATUS_DELTA_INPUT_SIZE_ERROR;
            case ml::validation_status::buffers_overlap:
                return DML_STATUS_OVERLAPPING_BUFFER_ERROR;
            case ml::validation_status::address_is_misaligned:
                return DML_STATUS_DELTA_ALIGN_ERROR;
            case ml::validation_status::delta_input_size_is_wrong:
                return DML_STATUS_DELTA_ALIGN_ERROR;
            case ml::validation_status::delta_input_size_overflow:
                return DML_STATUS_DELTA_INPUT_SIZE_ERROR;
            case ml::validation_status::delta_record_size_is_wrong:
                return DML_STATUS_DELTA_RECORD_SIZE_ERROR;
            case ml::validation_status::dualcast_address_is_wrong:
                return DML_STATUS_DUALCAST_ALIGN_ERROR;
            case ml::validation_status::batch_size_is_wrong:
                return DML_STATUS_BATCH_SIZE_ERROR;
            case ml::validation_status::dif_size_is_wrong:
                return DML_STATUS_JOB_LENGTH_ERROR;
            case ml::validation_status::unsupported_operation:
                return DML_STATUS_JOB_OPERATION_ERROR;
            default:
                return DML_STATUS_INTERNAL_ERROR;
        }
    }
}  // namespace dml

#endif  // DML_STATUS_HPP
