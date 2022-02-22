/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include "../dml_kernels.h"

#if defined(_MSC_BUILD)
#include <intrin.h>
#elif defined(__GNUC__)
#include <x86intrin.h>
#else
#error "Unsupported compiler"
#endif

// Disable optimization for MSVC, because it doesn't support _load_maskXX intrinsic
#ifndef _MSC_BUILD
static inline void fill_big_avx512(uint64_t pattern, uint8_t *const src, uint32_t transfer_size)
{
    // Check pointer alignment
    const size_t unaligned_bytes     = (uintptr_t)src % 64u;
    const size_t unaligned_part_size = (0u == unaligned_bytes) ? 0u : 64u - unaligned_bytes;

    __m512i zmm_pattern = _mm512_set1_epi64(pattern);

    // Fill unaligned part of destination
    if (0u != unaligned_part_size)
    {
        unsigned long long mask  = 0xFFFFFFFFFFFFFFFFu >> unaligned_bytes;
        __mmask64          mmask = _load_mask64(&mask);

        _mm512_mask_storeu_epi8(src, mmask, zmm_pattern);

        pattern     = (pattern << (unaligned_bytes * 8u)) | (pattern >> (64u - (unaligned_bytes * 8u)));
        zmm_pattern = _mm512_set1_epi64(pattern);
    }

    // Fill aligned part of destination
    const size_t aligned_part_size = transfer_size - unaligned_part_size;
    const size_t head_size         = aligned_part_size / sizeof(__m512i);
    const size_t tail_size         = aligned_part_size % sizeof(__m512i);

    uint8_t *const aligned_src = src + unaligned_part_size;
    __m512i       *head_ptr    = (__m512i *)aligned_src;
    __m512i       *tail_ptr    = (__m512i *)head_ptr + head_size;

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
        unsigned long long mask  = ~(0xFFFFFFFFFFFFFFFFu << tail_size);
        __mmask64          mmask = _load_mask64(&mask);
        _mm512_mask_storeu_epi8(tail_ptr, mmask, zmm_pattern);
    }
}

static inline void fill_small_avx512(uint64_t pattern, uint8_t *const src, uint32_t transfer_size)
{
    // Fill 0-63 bytes with 64bit pattern via two _mm256_mask_storeu_epi8 calls.

    const size_t tail_size = transfer_size % sizeof(__m512i);

    __m256i ymm1_pattern = _mm256_set1_epi64x(pattern);

    unsigned long long mask_value = ~(0xFFFFFFFFFFFFFFFFu << tail_size);
    __mmask32          mask_first = _load_mask32((uint32_t *)&mask_value);
    _mm256_mask_storeu_epi8(src, mask_first, ymm1_pattern);

    __mmask32 mask_second = _load_mask32((uint32_t *)&mask_value + 1u);
    _mm256_mask_storeu_epi8(src + 32u, mask_second, ymm1_pattern);
}

void dml_avx512_fill_u64(uint64_t pattern, uint8_t *dst, uint32_t transfer_size)
{
    const size_t small_bound = 64u;

    if (transfer_size < small_bound)
    {
        fill_small_avx512(pattern, dst, transfer_size);
    }
    else
    {
        fill_big_avx512(pattern, dst, transfer_size);
    }
}
#else
void dml_avx512_fill_u64(uint64_t pattern, uint8_t *dst, uint32_t transfer_size)
{
    dml_ref_fill_u64(pattern, dst, transfer_size);
}
#endif
