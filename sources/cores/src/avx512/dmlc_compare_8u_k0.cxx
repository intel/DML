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
    uint32_t    i;
    __mmask64   msk64 = (__mmask64)0;
    for (i = 0u; (i + 64) <= size; i += 64) {
        msk64 = _mm512_cmp_epi8_mask(
            _mm512_loadu_si512((void const*)(first_vector_ptr + i)),
            _mm512_loadu_si512((void const*)(second_vector_ptr + i)),
            _MM_CMPINT_NE);
        if (msk64) {
            *mismatch_offset_ptr = i + (uint32_t)_tzcnt_u64((uint64_t)msk64);
            return DML_COMPARE_STATUS_NE;
        }
    }
    {
        uint64_t    tail = size & 63;
        msk64 = ((uint64_t)1 << tail) - (uint64_t)1;
        msk64 = _mm512_cmp_epi8_mask(
            _mm512_maskz_loadu_epi8(msk64, (void const*)(first_vector_ptr + i)),
            _mm512_maskz_loadu_epi8(msk64, (void const*)(second_vector_ptr + i)),
            _MM_CMPINT_NE);
        if (msk64) {
            *mismatch_offset_ptr = i + (uint32_t)_tzcnt_u64((uint64_t)msk64);
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
#if (DML_SIZE_PATTERN_T == 64)
    const uint32_t pattern_chunk_count = size >> 3;
    const uint64_t tail_bytes_count = size & 7;
    const uint64_t* const pattern_region_ptr = (uint64_t*)memory_region_ptr;

    __m512i     x_pattern = _mm512_set1_epi64(pattern);
    uint32_t    i;
    __mmask8    msk8 = (__mmask8)0;

    for (i = 0u; (i + 8) <= pattern_chunk_count; i += 8) {
        msk8 = _mm512_cmp_epi64_mask(_mm512_loadu_si512((void const*)(pattern_region_ptr + i)),
            x_pattern,
            _MM_CMPINT_NE);
        if (msk8) {
            *mismatch_offset_ptr = (i + (uint32_t)_tzcnt_u32((uint32_t)msk8)) << 3u;
            return DML_COMPARE_STATUS_NE;
        }
    }
    {
        uint64_t    tail = pattern_chunk_count & 7;
        if (tail) {
            msk8 = (__mmask8)((1 << tail) - 1);
            msk8 = _mm512_mask_cmp_epi64_mask(msk8,
                _mm512_maskz_loadu_epi64(msk8, (void const*)(pattern_region_ptr + i)),
                x_pattern,
                _MM_CMPINT_NE);
            if (msk8) {
                *mismatch_offset_ptr = (i + (uint32_t)_tzcnt_u32((uint32_t)msk8)) << 3u;
                return DML_COMPARE_STATUS_NE;
            }
        }
    }
    if (tail_bytes_count) {
        memory_region_ptr += size - tail_bytes_count;
        pattern_t byte_pattern = pattern;
        // Compare tail
        for (uint32_t i = 0; i < tail_bytes_count; i++)
        {
            if (memory_region_ptr[i] != (uint8_t)byte_pattern)
            {
                *mismatch_offset_ptr = (pattern_chunk_count << 3) + i;

                return DML_COMPARE_STATUS_NE;
            }
            byte_pattern >>= OWN_BYTE_BIT_LENGTH;
        }
    }
    return DML_COMPARE_STATUS_EQ;
#else
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
#endif
}


