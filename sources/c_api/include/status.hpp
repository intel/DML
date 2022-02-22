/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_STATUS_HPP
#define DML_STATUS_HPP

#include <dml/dml.h>

#include <dml/detail/common/status.hpp>

namespace dml
{
    inline dml_status_t to_own_status(detail::execution_status status) noexcept
    {
        switch (status)
        {
            case detail::execution_status::success:
                {
                    return DML_STATUS_OK;
                }
            case detail::execution_status::false_predicate_success:
                {
                    return DML_STATUS_FALSE_PREDICATE_OK;
                }
            case detail::execution_status::page_fault_during_processing:
                {
                    return DML_STATUS_PAGE_FAULT_ERROR;
                }
            case detail::execution_status::page_response_error:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case detail::execution_status::batch_error:
                {
                    return DML_STATUS_BATCH_ERROR;
                }
            case detail::execution_status::batch_page_fault_error:
                {
                    return DML_STATUS_PAGE_FAULT_ERROR;
                }
            case detail::execution_status::offset_order_error:
                {
                    return DML_STATUS_DELTA_ASCENDT_ERROR;
                }
            case detail::execution_status::offset_overflow:
                {
                    return DML_STATUS_DELTA_OFFSET_ERROR;
                }
            case detail::execution_status::dif_control_error:
                {
                    return DML_STATUS_DIF_CHECK_ERROR;
                }
            case detail::execution_status::operation_error:
                {
                    return DML_STATUS_JOB_OPERATION_ERROR;
                }
            case detail::execution_status::flag_error:
                {
                    return DML_STATUS_JOB_FLAGS_ERROR;
                }
            case detail::execution_status::non_zero_reserved_field_error:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case detail::execution_status::invalid_transfer_size_error:
                {
                    return DML_STATUS_JOB_LENGTH_ERROR;
                }
            case detail::execution_status::descriptor_count_error:
                {
                    return DML_STATUS_BATCH_SIZE_ERROR;
                }
            case detail::execution_status::delta_size_error:
                {
                    return DML_STATUS_DELTA_INPUT_SIZE_ERROR;
                }
            case detail::execution_status::buffers_overlap:
                {
                    return DML_STATUS_OVERLAPPING_BUFFER_ERROR;
                }
            case detail::execution_status::dualcast_misalign_error:
                {
                    return DML_STATUS_DUALCAST_ALIGN_ERROR;
                }
            case detail::execution_status::descriptor_list_align_error:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case detail::execution_status::invalid_interrupt_handle:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case detail::execution_status::page_fault_on_translation:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case detail::execution_status::completion_record_align_error:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case detail::execution_status::misalign_address_error:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case detail::execution_status::privilege_error:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case detail::execution_status::traffic_class_error:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case detail::execution_status::readback_translation_error:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case detail::execution_status::operation_readback_timeout:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case detail::execution_status::hardware_timeout:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            case detail::execution_status::address_translation_error:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
            default:
                {
                    return DML_STATUS_INTERNAL_ERROR;
                }
        }
    }

    inline dml_status_t to_own_status(detail::submission_status status) noexcept
    {
        switch (status)
        {
            case detail::submission_status::success:
                return DML_STATUS_OK;
            case detail::submission_status::queue_busy:
                return DML_STATUS_WORK_QUEUES_NOT_AVAILABLE;
            case detail::submission_status::failure:
                return DML_STATUS_LIBACCEL_NOT_FOUND;
            default:
                return DML_STATUS_INTERNAL_ERROR;
        }
    }

    inline dml_status_t to_own_status(detail::validation_status status) noexcept
    {
        switch (status)
        {
            case detail::validation_status::success:
                return DML_STATUS_OK;
            case detail::validation_status::null_address:
                return DML_STATUS_NULL_POINTER_ERROR;
            case detail::validation_status::null_size:
                return DML_STATUS_JOB_LENGTH_ERROR;
            case detail::validation_status::wrong_size:
                return DML_STATUS_DELTA_INPUT_SIZE_ERROR;
            case detail::validation_status::overlapping:
                return DML_STATUS_OVERLAPPING_BUFFER_ERROR;
            case detail::validation_status::misalignment:
                return DML_STATUS_DELTA_ALIGN_ERROR;
            case detail::validation_status::large_size:
                return DML_STATUS_DELTA_INPUT_SIZE_ERROR;
            case detail::validation_status::wrong_delta_size:
                return DML_STATUS_DELTA_RECORD_SIZE_ERROR;
            case detail::validation_status::wrong_dualcast_address:
                return DML_STATUS_DUALCAST_ALIGN_ERROR;
            case detail::validation_status::wrong_batch_size:
                return DML_STATUS_BATCH_SIZE_ERROR;
            case detail::validation_status::wrong_dif_size:
                return DML_STATUS_JOB_LENGTH_ERROR;
            case detail::validation_status::dif_strip_adjacent:
                return DML_STATUS_DIF_STRIP_ADJACENT_ERROR;
            case detail::validation_status::unsupported_operation:
                return DML_STATUS_JOB_OPERATION_ERROR;
            default:
                return DML_STATUS_INTERNAL_ERROR;
        }
    }
}  // namespace dml

#endif  // DML_STATUS_HPP
