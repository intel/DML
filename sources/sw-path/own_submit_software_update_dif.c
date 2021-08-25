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
 * @brief Contains an implementation of @ref idml_sw_update_dif function
 * @date 4/1/2020
 *
 */

#include "own_dml_software_dif_feature.h"
#include "own_dml_software_operations_api.h"
#include "own_dml_software_common_api.h"


OWN_FUN(dml_status_t, sw_update_dif, (dml_job_t *const dml_job_ptr))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr->source_first_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr->destination_first_ptr)
    DML_BAD_ARGUMENT_BUFFERS_OVERLAPPING(dml_job_ptr->source_first_ptr,
                                         dml_job_ptr->source_length,
                                         dml_job_ptr->destination_first_ptr,
                                         dml_job_ptr->source_length)

    // General constants
    const uint32_t dif_flags   = dml_job_ptr->dif_config.flags;
    const uint32_t block_size  = own_dif_block_sizes[dml_job_ptr->dif_config.block_size];
    const uint32_t step        = block_size + sizeof(own_dif_t);
    const uint32_t block_count = dml_job_ptr->source_length / step;

    DML_BAD_ARGUMENT_RETURN((dml_job_ptr->source_length % step), DML_STATUS_JOB_LENGTH_ERROR)

    // Fix or Increment fields
    const uint8_t ref_tag_update_value  = (dif_flags & DML_DIF_FLAG_DST_FIX_REF_TAG) ? 0u : 1u;
    const uint8_t app_tag_update_value  = (dif_flags & DML_DIF_FLAG_DST_INC_APP_TAG) ? 1u : 0u;
    const uint16_t application_tag_mask = ~(dml_job_ptr->dif_config.destination_application_tag_mask);

    // CRC options
    const uint16_t   crc_seed                  = (dif_flags & DML_DIF_FLAG_INVERT_CRC_SEED) ?
                                                    0xFFFFu : 0u;
    const dml_bool_t invert_crc_result         = (dif_flags & DML_DIF_FLAG_INVERT_CRC_RESULT) ?
                                                    OWN_TRUE : OWN_FALSE;

    // Update options
    const dml_bool_t calculate_crc             = (dif_flags & DML_DIF_FLAG_DST_PASS_GUARD) ?
                                                    OWN_FALSE : OWN_TRUE;
    const dml_bool_t calculate_application_tag = (dif_flags & DML_DIF_FLAG_DST_PASS_APP_TAG) ?
                                                    OWN_FALSE : OWN_TRUE;
    const dml_bool_t calculate_reference_tag   = (dif_flags & DML_DIF_FLAG_DST_PASS_REF_TAG) ?
                                                    OWN_FALSE : OWN_TRUE;

    // Variables
    const uint8_t *source_ptr = dml_job_ptr->source_first_ptr;
    uint8_t *destination_ptr  = dml_job_ptr->destination_first_ptr;
    uint16_t application_tag  = dml_job_ptr->dif_config.destination_application_tag_seed;
    uint32_t reference_tag    = dml_job_ptr->dif_config.destination_reference_tag_seed;

    // Check Source
    const dml_status_t status = idml_sw_check_dif(dml_job_ptr);
    DML_RETURN_IN_CASE_OF_ERROR(status)

    // Process Data
    for (uint32_t block = 0u; block < block_count; block++)
    {
        own_dif_t *const destination_dif_ptr  = (own_dif_t *) (destination_ptr + block_size);

        dmlc_copy_8u(source_ptr, destination_ptr, step);

        // Update DIF
        if (calculate_crc)
        {
            uint16_t crc = crc_seed;
            dmlc_calculate_crc_16u(destination_ptr,block_size, &crc, OWN_DIF_CRC_POLYNOMIAL);
            destination_dif_ptr->guard_tag = idml_sw_reverse_bytes_16u((invert_crc_result) ? ~crc: crc);
        }

        if (calculate_application_tag)
        {
            destination_dif_ptr->application_tag = idml_sw_reverse_bytes_16u(application_tag & application_tag_mask);
        }

        if (calculate_reference_tag)
        {
            destination_dif_ptr->reference_tag = idml_sw_reverse_bytes_32u(reference_tag);
        }

        // Update Variables
        application_tag += app_tag_update_value;
        reference_tag   += ref_tag_update_value;
        source_ptr      += step;
        destination_ptr += step;
    }

    return DML_STATUS_OK;
}

#include "own_software_reverse_functions.cxx"
