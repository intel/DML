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
 * @brief Contains an implementation of DML @ref DML_OP_CRC operation
 * @date 3/12/2020
 *
 */

#define DML_CRC_POLYNOMIAL 0x1EDC6F41u /**< DML polynomial for crc operation */

#define OWN_CRC_BAD_ALIGNMENT(ptr) DML_BAD_ARGUMENT_RETURN((0 != (((uint64_t)ptr) & 0x3u)), DML_STATUS_CRC_ALIGN_ERROR) /**< Checks that the pointer is 4-byte aligned */


OWN_FUN_INLINE(dml_status_t, sw_crc, (dml_job_t *const dml_job_ptr))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr->source_first_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr->crc_checksum_ptr)

    // Variables
    dmlc_status_t status;
    uint8_t  *const source_ptr        = dml_job_ptr->source_first_ptr;
    uint32_t *const crc_ptr           = dml_job_ptr->crc_checksum_ptr;
    const uint32_t  source_size       = dml_job_ptr->source_length;
    const dml_operation_flags_t flags = dml_job_ptr->flags;

    // Read the CRC seed from memory at the CRC Seed Address only in case if DML_FLAG_READ_SEED set
    if(!(flags & DML_FLAG_CRC_READ_SEED))
    {
        *crc_ptr = 0u;
    }
    else
    {
        // Checks that pointer is 4-byte aligned
        OWN_CRC_BAD_ALIGNMENT(dml_job_ptr->crc_checksum_ptr)
    }
    


    // Bypass inversion and use reverse bit order for CRC result
    if (!(flags & DML_FLAG_CRC_BYPASS_REFLECTION))
    {
        *crc_ptr = ~ (*crc_ptr);
        *crc_ptr = idml_sw_bit_reverse_32u(*crc_ptr);
    }

    // Bypass Data Reflection in case if DML_FLAG_DATA_REFLECTION set
    status = (!(flags & DML_FLAG_CRC_BYPASS_DATA_REFLECTION)) ?
        dmlc_calculate_crc_reflected_32u(source_ptr, source_size, crc_ptr, DML_CRC_POLYNOMIAL):
        dmlc_calculate_crc_32u(source_ptr, source_size, crc_ptr, DML_CRC_POLYNOMIAL);


    // Bypass inversion and use reverse bit order for CRC result
    if (!(flags & DML_FLAG_CRC_BYPASS_REFLECTION))
    {
        *crc_ptr = idml_sw_bit_reverse_32u(*crc_ptr);
        *crc_ptr = ~ (*crc_ptr);
    }

    return status;
}