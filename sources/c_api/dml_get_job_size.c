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
 * @brief Contains an implementation of @ref dml_get_job_size function
 * @date 3/13/2020
 *
 */
#if defined(DML_HW)
    #include "hardware_api.h"
#endif
#include "own_dml_api.h"
#include "own_dml_align.h"
#include "own_dml_internal_state.h"


DML_FUN(dml_status_t, dml_get_job_size, (const dml_path_t dml_path, uint32_t *const job_size_ptr ))
{
    DML_BAD_ARGUMENT_NULL_POINTER(job_size_ptr)
    DML_BAD_ARGUMENT_INCORRECT_PATH(dml_path)

    // Variables
    uint32_t job_size        = 0u;
    uint32_t path_state_size = 0u;

    job_size += DML_ALIGNED_SIZE(sizeof(dml_job_t), DML_DEFAULT_ALIGNMENT) + DML_DEFAULT_ALIGNMENT;
    job_size += DML_ALIGNED_SIZE(sizeof(own_dml_state_t), DML_DEFAULT_ALIGNMENT);
#if defined(DML_HW)
    job_size += DML_ALIGNED_SIZE(OWN_HW_INTERNAL_BUFFERS_SIZE, DML_DEFAULT_ALIGNMENT);
#endif

    // Calculate necessary size for specified path
    switch (dml_path)
    {
#if defined(DML_HW)
        case DML_PATH_HW:
        case DML_PATH_SW:
        case DML_PATH_AUTO:
#endif
        default:
            path_state_size = idml_sw_get_state_size();
    }

    *job_size_ptr = job_size + path_state_size;

    return DML_STATUS_OK;
}
