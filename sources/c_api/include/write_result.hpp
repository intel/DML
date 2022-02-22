/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_WRITE_RESULT_HPP
#define DML_WRITE_RESULT_HPP

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
        return to_own_status(dml::detail::ml::get_status(make_view(job.state().task).get_completion_record()));
    }

    inline dml_status_t write_result_batch(job_view job) noexcept
    {
        return to_own_status(dml::detail::ml::get_status(make_view(job.state().task).get_completion_record()));
    }

    inline dml_status_t write_result_drain(job_view job) noexcept
    {
        return to_own_status(dml::detail::ml::get_status(make_view(job.state().task).get_completion_record()));
    }

    inline dml_status_t write_result_mem_move(job_view job) noexcept
    {
        job.set_result(detail::ml::get_result(make_view(job.state().task).get_completion_record()));

        return to_own_status(dml::detail::ml::get_status(make_view(job.state().task).get_completion_record()));
    }

    inline dml_status_t write_result_fill(job_view job) noexcept
    {
        return to_own_status(dml::detail::ml::get_status(make_view(job.state().task).get_completion_record()));
    }

    inline dml_status_t write_result_dualcast(job_view job) noexcept
    {
        return to_own_status(dml::detail::ml::get_status(make_view(job.state().task).get_completion_record()));
    }

    inline dml_status_t write_result_crc(job_view job) noexcept
    {
        job.set_crc(dml::detail::ml::get_crc_value(make_view(job.state().task).get_completion_record()));

        return to_own_status(dml::detail::ml::get_status(make_view(job.state().task).get_completion_record()));
    }

    inline dml_status_t write_result_compare(job_view job) noexcept
    {
        job.set_offset(dml::detail::ml::get_bytes_completed(make_view(job.state().task).get_completion_record()));
        job.set_result(dml::detail::ml::get_result(make_view(job.state().task).get_completion_record()));

        return to_own_status(dml::detail::ml::get_status(make_view(job.state().task).get_completion_record()));
    }

    inline dml_status_t write_result_create_delta(job_view job) noexcept
    {
        //job.set_offset();
        job.set_destination_length(dml::detail::ml::get_delta_record_size(make_view(job.state().task).get_completion_record()));
        job.set_result(dml::detail::ml::get_result(make_view(job.state().task).get_completion_record()));
        job.set_offset(*reinterpret_cast<uint16_t *>(job.destination_first()));

        return to_own_status(dml::detail::ml::get_status(make_view(job.state().task).get_completion_record()));
    }

    inline dml_status_t write_result_apply_delta(job_view job) noexcept
    {
        return to_own_status(dml::detail::ml::get_status(make_view(job.state().task).get_completion_record()));
    }

    inline dml_status_t write_result_dif_check(job_view job) noexcept
    {
        job.set_offset(dml::detail::ml::get_bytes_completed(make_view(job.state().task).get_completion_record()));
        job.set_result(dml::detail::ml::get_result(make_view(job.state().task).get_completion_record()));

        return to_own_status(dml::detail::ml::get_status(make_view(job.state().task).get_completion_record()));
    }

    inline dml_status_t write_result_dif_insert(job_view job) noexcept
    {
        job.set_offset(dml::detail::ml::get_bytes_completed(make_view(job.state().task).get_completion_record()));

        return to_own_status(dml::detail::ml::get_status(make_view(job.state().task).get_completion_record()));
    }

    inline dml_status_t write_result_dif_strip(job_view job) noexcept
    {
        job.set_offset(dml::detail::ml::get_bytes_completed(make_view(job.state().task).get_completion_record()));
        job.set_result(dml::detail::ml::get_result(make_view(job.state().task).get_completion_record()));

        return to_own_status(dml::detail::ml::get_status(make_view(job.state().task).get_completion_record()));
    }

    inline dml_status_t write_result_dif_update(job_view job) noexcept
    {
        job.set_offset(dml::detail::ml::get_bytes_completed(make_view(job.state().task).get_completion_record()));
        job.set_result(dml::detail::ml::get_result(make_view(job.state().task).get_completion_record()));

        return to_own_status(dml::detail::ml::get_status(make_view(job.state().task).get_completion_record()));
    }

    inline dml_status_t write_result_cache_flush(job_view job) noexcept
    {
        return to_own_status(dml::detail::ml::get_status(make_view(job.state().task).get_completion_record()));
    }
}  // namespace dml

#endif  // DML_WRITE_RESULT_HPP
