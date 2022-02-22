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

uint32_t dml_avx512_compare(const uint8_t* src1, const uint8_t* src2, uint32_t transfer_size, uint8_t* result)
{
    const uint8_t equal     = 0x0;
    const uint8_t not_equal = 0x1;

    uint32_t  i;
    __mmask64 msk64 = (__mmask64)0;
    for (i = 0u; (i + 64) <= transfer_size; i += 64)
    {
        msk64 =
            _mm512_cmp_epi8_mask(_mm512_loadu_si512((void const*)(src1 + i)), _mm512_loadu_si512((void const*)(src2 + i)), _MM_CMPINT_NE);
        if (msk64)
        {
            *result = not_equal;
            return i + (uint32_t)_tzcnt_u64((uint64_t)msk64);
        }
    }
    {
        uint64_t tail = transfer_size & 63;
        msk64         = ((uint64_t)1 << tail) - (uint64_t)1;
        msk64         = _mm512_cmp_epi8_mask(_mm512_maskz_loadu_epi8(msk64, (void const*)(src1 + i)),
                                     _mm512_maskz_loadu_epi8(msk64, (void const*)(src2 + i)),
                                     _MM_CMPINT_NE);
        if (msk64)
        {
            *result = not_equal;
            return i + (uint32_t)_tzcnt_u64((uint64_t)msk64);
        }
    }

    *result = equal;
    return 0;
}