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

#ifndef DML_WRITE_RESULT_HPP
#define DML_WRITE_RESULT_HPP

#include <dml/cpp/middle_layer/make_descriptor.hpp>

#include "status.hpp"

namespace dml
{
    inline dml_status_t write_result_nop(job_view job) noexcept;

    inline dml_status_t write_result_batch(job_view job) noexcept;

    inline dml_status_t write_result_drain(job_view job) noexcept;

    inline dml_status_t write_result_mem_move(job_view job) noexcept;

    inline dml_status_t write_result_fill(job_view job) noexcept;

    inline dml_status_t write_result_dualcast(job_view job) noexcept;

    inline dml_status_t write_result_compare(job_view job) noexcept;

    inline dml_status_t write_result_crc(job_view job) noexcept;

    inline dml_status_t write_result_create_delta(job_view job) noexcept;

    inline dml_status_t write_result_apply_delta(job_view job) noexcept;

    inline dml_status_t write_result_dif_check(job_view job) noexcept;

    inline dml_status_t write_result_dif_insert(job_view job) noexcept;

    inline dml_status_t write_result_dif_strip(job_view job) noexcept;

    inline dml_status_t write_result_dif_update(job_view job) noexcept;

    inline dml_status_t write_result_cache_flush(job_view job) noexcept;

    inline dml_status_t write_result(job_view job) noexcept
    {
        switch (job.operation())
        {
            case DML_OP_NOP:
                return write_result_nop(job);
            case DML_OP_BATCH:
                return write_result_batch(job);
            case DML_OP_DRAIN:
                return write_result_drain(job);
            case DML_OP_MEM_MOVE:
                return write_result_mem_move(job);
            case DML_OP_FILL:
                return write_result_fill(job);
            case DML_OP_DUALCAST:
                return write_result_dualcast(job);
            case DML_OP_CRC:
                return write_result_crc(job);
            case DML_OP_COPY_CRC:
                return write_result_crc(job);
            case DML_OP_COMPARE:
                return write_result_compare(job);
            case DML_OP_COMPARE_PATTERN:
                return write_result_compare(job);
            case DML_OP_DELTA_CREATE:
                return write_result_create_delta(job);
            case DML_OP_DELTA_APPLY:
                return write_result_apply_delta(job);
            case DML_OP_DIF_CHECK:
                return write_result_dif_check(job);
            case DML_OP_DIF_INSERT:
                return write_result_dif_insert(job);
            case DML_OP_DIF_STRIP:
                return write_result_dif_strip(job);
            case DML_OP_DIF_UPDATE:
                return write_result_dif_update(job);
            case DML_OP_CACHE_FLUSH:
                return write_result_cache_flush(job);
            default:
                return DML_STATUS_JOB_OPERATION_ERROR;
        }
    }

    inline dml_status_t write_result_nop(job_view job) noexcept
    {
        auto result_view = ml::views::nop_result(job.state().record);

        return to_own_status(static_cast<ml::execution_status>(result_view.status()));
    }

    inline dml_status_t write_result_batch(job_view job) noexcept
    {
        auto result_view = ml::views::batch_result(job.state().record);

        return to_own_status(static_cast<ml::execution_status>(result_view.status()));
    }

    inline dml_status_t write_result_drain(job_view job) noexcept
    {
        auto result_view = ml::views::drain_result(job.state().record);

        return to_own_status(static_cast<ml::execution_status>(result_view.status()));
    }

    inline dml_status_t write_result_mem_move(job_view job) noexcept
    {
        auto result_view = ml::views::mem_move_result(job.state().record);

        job.set_result(result_view.result());

        return to_own_status(static_cast<ml::execution_status>(result_view.status()));
    }

    inline dml_status_t write_result_fill(job_view job) noexcept
    {
        auto result_view = ml::views::fill_result(job.state().record);

        return to_own_status(static_cast<ml::execution_status>(result_view.status()));
    }

    inline dml_status_t write_result_dualcast(job_view job) noexcept
    {
        auto result_view = ml::views::fill_result(job.state().record);

        return to_own_status(static_cast<ml::execution_status>(result_view.status()));
    }

    inline dml_status_t write_result_crc(job_view job) noexcept
    {
        auto result_view = ml::views::crc_result(job.state().record);

        job.set_crc(result_view.crc_value());

        return to_own_status(static_cast<ml::execution_status>(result_view.status()));
    }

    inline dml_status_t write_result_compare(job_view job) noexcept
    {
        auto result_view = ml::views::compare_result(job.state().record);

        job.set_offset(result_view.bytes_completed());
        job.set_result(result_view.result());

        return to_own_status(static_cast<ml::execution_status>(result_view.status()));
    }

    inline dml_status_t write_result_create_delta(job_view job) noexcept
    {
        auto result_view = ml::views::create_delta_result(job.state().record);

        //job.set_offset();
        job.set_destination_length(result_view.delta_record_size());
        job.set_result(result_view.result());
        job.set_offset(*reinterpret_cast<uint16_t *>(job.destination_first()));

        return to_own_status(static_cast<ml::execution_status>(result_view.status()));
    }

    inline dml_status_t write_result_apply_delta(job_view job) noexcept
    {
        auto result_view = ml::views::apply_delta_result(job.state().record);

        return to_own_status(static_cast<ml::execution_status>(result_view.status()));
    }

    inline dml_status_t write_result_dif_check(job_view job) noexcept
    {
        auto result_view = ml::views::dif_check_result(job.state().record);

        job.set_offset(result_view.bytes_completed());
        job.set_result(result_view.dif_status());

        return to_own_status(static_cast<ml::execution_status>(result_view.status()));
    }

    inline dml_status_t write_result_dif_insert(job_view job) noexcept
    {
        auto result_view = ml::views::dif_insert_result(job.state().record);

        job.set_offset(result_view.bytes_completed());

        return to_own_status(static_cast<ml::execution_status>(result_view.status()));
    }

    inline dml_status_t write_result_dif_strip(job_view job) noexcept
    {
        auto result_view = ml::views::dif_strip_result(job.state().record);

        job.set_offset(result_view.bytes_completed());
        job.set_result(result_view.dif_status());

        return to_own_status(static_cast<ml::execution_status>(result_view.status()));
    }

    inline dml_status_t write_result_dif_update(job_view job) noexcept
    {
        auto result_view = ml::views::dif_update_result(job.state().record);

        job.set_offset(result_view.bytes_completed());
        job.set_result(result_view.dif_status());

        return to_own_status(static_cast<ml::execution_status>(result_view.status()));
    }

    inline dml_status_t write_result_cache_flush(job_view job) noexcept
    {
        auto result_view = ml::views::cache_flush_result(job.state().record);

        return to_own_status(static_cast<ml::execution_status>(result_view.status()));
    }
}  // namespace dml

#endif  // DML_WRITE_RESULT_HPP
