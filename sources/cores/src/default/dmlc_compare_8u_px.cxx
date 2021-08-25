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
  * @brief Contain default implementation of the follow functions:
  *      - @ref dmlc_own_compare_8u()
  *      - @ref dmlc_own_compare_with_pattern_8u()
  *
  * @date 07/06/2021
  *
  */

DML_CORE_OWN_INLINE(dmlc_status_t, compare_8u, (const uint8_t* first_vector_ptr,
    const uint8_t* second_vector_ptr,
    const uint32_t size,
    uint32_t* const mismatch_offset_ptr))
{
    for (uint32_t i = 0u; i < size; i++)
    {
        if (first_vector_ptr[i] != second_vector_ptr[i])
        {
            *mismatch_offset_ptr = i;

            return DML_COMPARE_STATUS_NE;
        }
    }

    return DML_COMPARE_STATUS_EQ;
}

DML_CORE_OWN_INLINE(dmlc_status_t, compare_with_pattern_8u, (const uint8_t* memory_region_ptr,
    const pattern_t pattern,
    const uint32_t size,
    uint32_t* const mismatch_offset_ptr))
{
    //Constants
    const uint32_t pattern_size = sizeof(pattern_t);
    const uint32_t pattern_chunk_count = size / pattern_size;
    const uint64_t tail_bytes_count = size % pattern_size;
    const uint64_t* const pattern_region_ptr = (uint64_t*)memory_region_ptr;

    // Compare by pattern chunks
    for (uint32_t i = 0u; i < pattern_chunk_count; i++)
    {
        if (pattern_region_ptr[i] != pattern)
        {
            *mismatch_offset_ptr = i * pattern_size;

            return DML_COMPARE_STATUS_NE;
        }
    }

    memory_region_ptr += size - tail_bytes_count;
    pattern_t byte_pattern = pattern;

    // Compare tail
    for (uint32_t i = 0; i < tail_bytes_count; i++)
    {
        if (memory_region_ptr[i] != (uint8_t)byte_pattern)
        {
            *mismatch_offset_ptr = pattern_chunk_count * pattern_size + i;

            return DML_COMPARE_STATUS_NE;
        }

        byte_pattern >>= OWN_BYTE_BIT_LENGTH;
    }

    return DML_COMPARE_STATUS_EQ;
}
