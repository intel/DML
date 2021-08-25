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
 * @brief Contains an implementation of @ref idml_sw_strip_dif function
 * @date 4/1/2020
 *
 */


#include "own_dml_software_operations_api.h"
#include "own_dml_definitions.h"
#include "own_dml_software_dif_feature.h"


OWN_FUN(dml_status_t, sw_strip_dif, (dml_job_t *const dml_job_ptr))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr->source_first_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr->destination_first_ptr)

    // General constants
    const uint32_t block_size  = own_dif_block_sizes[dml_job_ptr->dif_config.block_size];
    const uint32_t source_step = block_size + sizeof(own_dif_t);
    const uint32_t block_count = dml_job_ptr->source_length / source_step;

    DML_BAD_ARGUMENT_RETURN((dml_job_ptr->source_length % source_step), DML_STATUS_JOB_LENGTH_ERROR)

    DML_BAD_ARGUMENT_BUFFERS_OVERLAPPING(dml_job_ptr->source_first_ptr,
                                         dml_job_ptr->source_length,
                                         dml_job_ptr->destination_first_ptr,
                                         block_count * block_size)

    //Variables
    const uint8_t *source_ptr = dml_job_ptr->source_first_ptr;
    uint8_t  *destination_ptr = dml_job_ptr->destination_first_ptr;

    // Check source data
    dml_status_t status = idml_sw_check_dif(dml_job_ptr);
    DML_RETURN_IN_CASE_OF_ERROR(status)

    // Process data
    for (uint32_t block = 0; block < block_count; block++)
    {
        dmlc_copy_8u(source_ptr, destination_ptr, block_size);

        source_ptr      += source_step;
        destination_ptr += block_size;
    }

    return DML_STATUS_OK;
}
