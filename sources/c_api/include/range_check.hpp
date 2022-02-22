/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_RANGE_CHECK_HPP
#define DML_RANGE_CHECK_HPP

#include "job_view.hpp"

namespace dml
{
    inline dml_status_t range_check_mem_move(const uint8_t* src1, const uint8_t* dst1, uint32_t src_size, uint16_t flags) noexcept;

    inline dml_status_t range_check_crc(const uint8_t* src1, const uint32_t* crc_ptr, uint32_t src_size) noexcept;

    inline dml_status_t range_check_copy_crc(const uint8_t* src1, const uint8_t* dst1, const uint32_t* crc_ptr, uint32_t src_size) noexcept;

    inline dml_status_t range_check(job_view job) noexcept
    {
        switch (job.operation())
        {
            case DML_OP_NOP:
                return DML_STATUS_OK;
            case DML_OP_BATCH:
                return DML_STATUS_OK;
            case DML_OP_DRAIN:
                return DML_STATUS_OK;
            case DML_OP_MEM_MOVE:
                return range_check_mem_move(job.source_first(), job.destination_first(), job.source_length(), job.flags());
            case DML_OP_FILL:
                return DML_STATUS_OK;
            case DML_OP_DUALCAST:
                return DML_STATUS_OK;
            case DML_OP_COMPARE:
                return DML_STATUS_OK;
            case DML_OP_COMPARE_PATTERN:
                return DML_STATUS_OK;
            case DML_OP_CRC:
                return range_check_crc(job.source_first(), job.crc_ptr(), job.source_length());
            case DML_OP_COPY_CRC:
                return range_check_copy_crc(job.source_first(), job.destination_first(), job.crc_ptr(), job.source_length());
            case DML_OP_DELTA_CREATE:
                return DML_STATUS_OK;
            case DML_OP_DELTA_APPLY:
                return DML_STATUS_OK;
            case DML_OP_DIF_CHECK:
                return DML_STATUS_OK;
            case DML_OP_DIF_INSERT:
                return DML_STATUS_OK;
            case DML_OP_DIF_STRIP:
                return DML_STATUS_OK;
            case DML_OP_DIF_UPDATE:
                return DML_STATUS_OK;
            case DML_OP_CACHE_FLUSH:
                return DML_STATUS_OK;
            default:
                return DML_STATUS_JOB_OPERATION_ERROR;
        }
    }

    inline dml_status_t range_check_mem_move(const uint8_t* src1, const uint8_t* dst1, uint32_t src_size, uint16_t flags) noexcept
    {
        if (flags & DML_FLAG_COPY_ONLY)
        {
            if ((src1 <= dst1) && ((src1 + src_size) > dst1))
            {
                return DML_STATUS_OVERLAPPING_BUFFER_ERROR;
            }
        }

        return DML_STATUS_OK;
    }

    inline dml_status_t range_check_crc(const uint8_t* src1, const uint32_t* crc_ptr, const uint32_t src_size) noexcept
    {
        static_cast<void>(src1);
        static_cast<void>(src_size);
        if (crc_ptr == nullptr)
        {
            return DML_STATUS_NULL_POINTER_ERROR;
        }
        else
        {
            return DML_STATUS_OK;
        }
    }

    inline dml_status_t range_check_copy_crc(const uint8_t*  src1,
                                             const uint8_t*  dst1,
                                             const uint32_t* crc_ptr,
                                             const uint32_t  src_size) noexcept
    {
        static_cast<void>(src1);
        static_cast<void>(dst1);
        static_cast<void>(src_size);
        if (crc_ptr == nullptr)
        {
            return DML_STATUS_NULL_POINTER_ERROR;
        }
        else
        {
            return DML_STATUS_OK;
        }
    }

}  // namespace dml

#endif  // DML_RANGE_CHECK_HPP
