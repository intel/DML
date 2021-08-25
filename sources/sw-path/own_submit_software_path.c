/*
 * Copyright 2020-2021 Intel Corporation.
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

/**
 * @brief Contains an implementation of DML operation dispatcher.
 * @date 3/2/2020
 *
 */

#include "own_dml_software_operations_api.h"
#include "own_dml_software_common_api.h"
#include "own_dml_software_flow_control_api.h"
#include "own_dml_batch.h"

OWN_FUN(dml_status_t, sw_submit_job, (dml_job_t *const dml_job_ptr))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)

    switch (dml_job_ptr->operation)
    {
        case DML_OP_MEM_MOVE:
            return idml_sw_memory_move(dml_job_ptr);

        case DML_OP_FILL:
            return idml_sw_fill(dml_job_ptr);

        case DML_OP_COMPARE:
            return idml_sw_compare(dml_job_ptr);

        case DML_OP_COMPARE_PATTERN:
            return idml_sw_compare_with_pattern(dml_job_ptr);

        case DML_OP_DELTA_CREATE:
            return idml_sw_create_delta(dml_job_ptr);

        case DML_OP_DELTA_APPLY:
            return idml_sw_apply_delta(dml_job_ptr);

        case DML_OP_DUALCAST:
            return idml_sw_dualcast_copy(dml_job_ptr);

        case DML_OP_CRC:
            return idml_sw_crc(dml_job_ptr);

        case DML_OP_COPY_CRC:
            return idml_sw_copy_with_crc(dml_job_ptr);

        case DML_OP_CACHE_FLUSH:
            return idml_sw_cache_flush(dml_job_ptr);

        case DML_OP_DIF_CHECK:
            return idml_sw_check_dif(dml_job_ptr);

        case DML_OP_DIF_INSERT:
            return idml_sw_insert_dif(dml_job_ptr);

        case DML_OP_DIF_STRIP:
            return idml_sw_strip_dif(dml_job_ptr);

        case DML_OP_DIF_UPDATE:
            return idml_sw_update_dif(dml_job_ptr);

        case DML_OP_BATCH:
            return idml_sw_batch(dml_job_ptr);

        case DML_OP_DRAIN:
        case DML_OP_NOP:
            return DML_STATUS_OK;

        default:
            return DML_STATUS_JOB_OPERATION_ERROR;
    }
}

// Include flow control functions
#include "own_software_check_result.cxx"

// Include common functions
#include "own_software_reverse_functions.cxx"

// Include dml operations implementation
#include "own_submit_software_mem_move.cxx"
#include "own_submit_software_fill.cxx"
#include "own_submit_software_compare.cxx"
#include "own_submit_software_compare_pattern.cxx"
#include "own_submit_software_create_delta.cxx"
#include "own_submit_software_apply_delta.cxx"
#include "own_submit_software_dualcast_copy.cxx"
#include "own_submit_software_crc.cxx"
#include "own_submit_software_copy_with_crc.cxx"
#include "own_submit_software_cache_flush.cxx"
#include "own_submit_software_batch.cxx"
