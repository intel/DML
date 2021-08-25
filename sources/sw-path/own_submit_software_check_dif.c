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
 * @brief Contains an implementation of @ref idml_sw_check_dif function
 * @date 4/1/2020
 *
 */

#include "own_dml_definitions.h"
#include "own_dml_software_dif_feature.h"
#include "own_dml_software_common_api.h"


/**
 * @brief Determines if an F condition is met for all tags
 */
#define OWN_F_TAGS_DETECTED(dif_ptr, flags)              \
    (DML_DIF_FLAG_SRC_F_DETECT_TAGS & flags          \
     && (dif_ptr->application_tag == DML_MAX_16U)    \
     && (dif_ptr->reference_tag   == DML_MAX_32U))


/**
 * @brief Determines if an F condition is met for application tag
 */
#define OWN_F_APPLICATION_TAG_DETECTED(dif_ptr, flags) \
    (DML_DIF_FLAG_SRC_F_DETECT_APP_TAG & dif_flags \
     && (dif_ptr->application_tag == DML_MAX_16U))


OWN_FUN(dml_status_t, sw_check_dif, (dml_job_t *const dml_job_ptr))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr->source_first_ptr)

    // General constants
    const uint32_t dif_flags   = dml_job_ptr->dif_config.flags;
    const uint32_t block_size  = own_dif_block_sizes[dml_job_ptr->dif_config.block_size];
    const uint32_t source_step = block_size + sizeof(own_dif_t);
    const uint32_t block_count = dml_job_ptr->source_length / source_step;

    DML_BAD_ARGUMENT_RETURN((dml_job_ptr->source_length % source_step), DML_STATUS_JOB_LENGTH_ERROR)

    // Seed calculation properties
    const uint16_t   crc_seed             = (dif_flags & DML_DIF_FLAG_INVERT_CRC_SEED) ? 0xFFFFu
                                                                                       : 0u;
    const dml_bool_t invert_crc_result    = (dif_flags & DML_DIF_FLAG_INVERT_CRC_RESULT) ? OWN_TRUE
                                                                                         : OWN_FALSE;
    // Check properties
    const dml_bool_t check_guard          = (dif_flags & DML_DIF_FLAG_SRC_GUARD_CHECK_DISABLE) ?
                                                OWN_FALSE : OWN_TRUE;
    const dml_bool_t check_reference_tag  = (dif_flags & DML_DIF_FLAG_SRC_REF_TAG_CHECK_DISABLE) ?
                                                OWN_FALSE : OWN_TRUE;

    // Tags update properties
    const uint8_t  ref_tag_update_value = (dif_flags & DML_DIF_FLAG_SRC_FIX_REF_TAG) ? 0u : 1u;
    const uint8_t  app_tag_update_value = (dif_flags & DML_DIF_FLAG_SRC_INC_APP_TAG) ? 1u : 0u;
    const uint16_t application_tag_mask = ~(dml_job_ptr->dif_config.source_application_tag_mask);

    //Variables
    const uint8_t *source_ptr  = dml_job_ptr->source_first_ptr;
    uint16_t application_tag   = dml_job_ptr->dif_config.source_application_tag_seed;
    uint32_t reference_tag     = dml_job_ptr->dif_config.source_reference_tag_seed;
    uint32_t check_accumulator = 0u;

    // Process data
    for (uint32_t block = 0; block < block_count; block++)
    {
        const own_dif_t *const dif_ptr = (own_dif_t *)(source_ptr + block_size);

        // F Detect Condition
        if ((DML_DIF_FLAG_SRC_F_DETECT_ALL & dif_flags)
             && (DML_DIF_FLAG_SRC_F_ENABLE_ERROR & dif_flags)
             && (DML_MAX_64U == (*(uint64_t *) dif_ptr)))
        {
            dml_job_ptr->offset = block * source_step;
            dml_job_ptr->result = DML_DIF_CHECK_ALL_BITS_SET_DETECT_ERROR;

            return DML_STATUS_DIF_CHECK_ERROR;
        }

        // Check Fields
        if (!OWN_F_TAGS_DETECTED(dif_ptr, dif_flags) && !OWN_F_APPLICATION_TAG_DETECTED(dif_ptr, dif_flags))
        {
            // Check guard
            if (check_guard)
            {
                uint16_t crc = crc_seed;
                dmlc_calculate_crc_16u(source_ptr, block_size, &crc, OWN_DIF_CRC_POLYNOMIAL);
                crc = idml_sw_reverse_bytes_16u((invert_crc_result) ? ~crc : crc);

                if (crc != dif_ptr->guard_tag)
                {
                    check_accumulator |= DML_DIF_CHECK_GUARD_MISMATCH;
                }
            }

            // Check reference tag
            if (check_reference_tag &&
                idml_sw_reverse_bytes_32u(reference_tag) != dif_ptr->reference_tag)
            {
                check_accumulator |= DML_DIF_CHECK_REFERENCE_TAG_MISMATCH;
            }

            // Check Application tag
            if (application_tag_mask != DML_MAX_16U
                 && idml_sw_reverse_bytes_16u(application_tag & application_tag_mask) != dif_ptr->application_tag)
            {
                check_accumulator |= DML_DIF_CHECK_APPLICATION_TAG_MISMATCH;
            }
        }

        if (check_accumulator)
        {
            dml_job_ptr->offset = block * source_step;
            dml_job_ptr->result = check_accumulator;

            return DML_STATUS_DIF_CHECK_ERROR;
        }

        // Update variables
        source_ptr      += source_step;
        application_tag += app_tag_update_value;
        reference_tag   += ref_tag_update_value;
    }

    // All is OK
    dml_job_ptr->result = 0;

    return DML_STATUS_OK;
}

#include "own_software_reverse_functions.cxx"
