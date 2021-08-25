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
 * @brief Contain optimized AVX512 implementation of the follow functions:
 *      - @ref dmlc_fill_with_pattern_8u()
 *
 * @date 10/29/2020
 *
 */

#include "core_memory.h"
#include "own_dmlc_definitions.h"

DML_CORE_OWN_INLINE(void, opt_fill_with_pattern_8u_big, ( uint64_t        pattern,
                                                          uint8_t  *const memory_region_ptr,
                                                          uint32_t        bytes_to_process ) );

DML_CORE_OWN_INLINE(void, opt_fill_with_pattern_8u_small, ( uint64_t        pattern,
                                                           uint8_t  *const memory_region_ptr,
                                                           uint32_t        bytes_to_process ) );

DML_CORE_OWN_INLINE(dmlc_status_t, opt_fill_with_pattern_8u, ( uint64_t        pattern,
                                                               uint8_t  *const memory_region_ptr,
                                                               uint32_t        bytes_to_process ) )
{
    DML_CORE_CHECK_NULL_POINTER(memory_region_ptr);

    if (bytes_to_process < 64u)
    {
        dmlc_own_opt_fill_with_pattern_8u_small(pattern, memory_region_ptr, bytes_to_process);
    }
    else
    {
        dmlc_own_opt_fill_with_pattern_8u_big(pattern, memory_region_ptr, bytes_to_process);
    }

    return DML_STATUS_OK;
}

DML_CORE_OWN_INLINE(void, opt_fill_with_pattern_8u_big, ( uint64_t        pattern,
                                                          uint8_t  *const memory_region_ptr,
                                                          uint32_t        bytes_to_process ) )
{
    // Check pointer alignment
    const size_t unaligned_bytes = (uintptr_t)memory_region_ptr % 64u;
    const size_t unaligned_part_size = (0u == unaligned_bytes) ? 0u : 64u - unaligned_bytes;

    __m512i zmm_pattern = _mm512_set1_epi64(pattern);

    // Fill unaligned part of destination
    if (0u != unaligned_part_size)
    {
        unsigned long long mask = 0xFFFFFFFFFFFFFFFFu >> unaligned_bytes;
        __mmask64 mmask = _load_mask64(&mask);

        _mm512_mask_storeu_epi8(memory_region_ptr, mmask, zmm_pattern);

        pattern = (pattern << (unaligned_bytes * 8u)) | (pattern >> (64u - (unaligned_bytes * 8u)));
        zmm_pattern = _mm512_set1_epi64(pattern);
    }

    // Fill aligned part of destination
    const size_t aligned_part_size = bytes_to_process - unaligned_part_size;
    const size_t head_size = aligned_part_size / sizeof(__m512i);
    const size_t tail_size = aligned_part_size % sizeof(__m512i);

    uint8_t *const aligned_memory_region_ptr = memory_region_ptr + unaligned_part_size;
    __m512i *head_ptr = (__m512i *)aligned_memory_region_ptr;
    __m512i *tail_ptr = (__m512i *)head_ptr + head_size;

    // Fill head part
    if (0u != head_size)
    {
        while (head_ptr != tail_ptr)
        {
            _mm512_store_si512(head_ptr, zmm_pattern);
            head_ptr++;
        }
    }

    // Fill tail part
    if (0u != tail_size)
    {
        unsigned long long mask = ~(0xFFFFFFFFFFFFFFFFu << tail_size);
        __mmask64 mmask = _load_mask64(&mask);
        _mm512_mask_storeu_epi8(tail_ptr, mmask, zmm_pattern);
    }

}

DML_CORE_OWN_INLINE(void, opt_fill_with_pattern_8u_small, ( uint64_t        pattern,
                                                            uint8_t  *const memory_region_ptr,
                                                            uint32_t        bytes_to_process ) )
{
    // Fill 1-63 bytes with 64bit pattern via two _mm256_mask_storeu_epi8 calls.

    const size_t tail_size = bytes_to_process % sizeof(__m512i);

    __m256i ymm1_pattern = _mm256_set1_epi64x(pattern);

    unsigned long long mask_value = ~(0xFFFFFFFFFFFFFFFFu << tail_size);
    __mmask32 mask_first = _load_mask32((uint32_t *)&mask_value);
    _mm256_mask_storeu_epi8(memory_region_ptr, mask_first, ymm1_pattern);

    __mmask32 mask_second = _load_mask32((uint32_t *)&mask_value + 1u);
    _mm256_mask_storeu_epi8(memory_region_ptr + 32u, mask_second, ymm1_pattern);
}
