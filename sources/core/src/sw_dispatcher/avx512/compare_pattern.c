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

uint32_t dml_avx512_compare_pattern(uint64_t pattern, const uint8_t* src, uint32_t transfer_size, uint8_t* result)
{
    const uint8_t equal     = 0x0;
    const uint8_t not_equal = 0x1;

    const uint32_t        pattern_chunk_count = transfer_size >> 3;
    const uint64_t        tail_bytes_count    = transfer_size & 7;
    const uint64_t* const pattern_region_ptr  = (uint64_t*)src;

    __m512i  x_pattern = _mm512_set1_epi64(pattern);
    uint32_t i;

    for (i = 0u; (i + 8) <= pattern_chunk_count; i += 8)
    {
        __mmask8 msk8 = _mm512_cmp_epi64_mask(_mm512_loadu_si512((void const*)(pattern_region_ptr + i)), x_pattern, _MM_CMPINT_NE);
        if (msk8)
        {
            *result = not_equal;
            return (i + (uint32_t)_tzcnt_u32((uint32_t)msk8)) << 3u;
        }
    }
    {
        uint64_t tail = pattern_chunk_count & 7;
        if (tail)
        {
            __mmask8 msk8 = (__mmask8)((1 << tail) - 1);
            msk8          = _mm512_mask_cmp_epi64_mask(msk8,
                                              _mm512_maskz_loadu_epi64(msk8, (void const*)(pattern_region_ptr + i)),
                                              x_pattern,
                                              _MM_CMPINT_NE);
            if (msk8)
            {
                *result = not_equal;
                return (i + (uint32_t)_tzcnt_u32((uint32_t)msk8)) << 3u;
            }
        }
    }
    if (tail_bytes_count)
    {
        src += transfer_size - tail_bytes_count;
        uint64_t byte_pattern = pattern;
        // Compare tail
        for (i = 0; i < tail_bytes_count; i++)
        {
            if (src[i] != (uint8_t)byte_pattern)
            {
                *result = not_equal;
                return (pattern_chunk_count << 3) + i;
            }
            const size_t byte_width = 8;
            byte_pattern >>= byte_width;
        }
    }

    *result = equal;
    return 0;
}
