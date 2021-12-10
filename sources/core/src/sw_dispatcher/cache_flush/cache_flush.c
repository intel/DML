/*
 * Copyright 2021 Intel Corporation.
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

#include "../dml_kernels.h"

#if defined(_MSC_BUILD)
#include <intrin.h>
#elif defined(__GNUC__)
#include <x86intrin.h>
#else
#error "Unsupported compiler"
#endif

static const size_t cache_line_size = 64u;

void dml_clflushopt(uint8_t *dst, uint32_t transfer_size)
{
    const size_t cache_line_count = transfer_size / cache_line_size;

    _mm_mfence();
    for (size_t cache_line_index = 0; cache_line_index < cache_line_count; ++cache_line_index)
    {
        uint8_t *cache_line = dst + (cache_line_size * cache_line_index);

        _mm_clflushopt(cache_line);
    }
    _mm_mfence();
}

void dml_clflush(uint8_t *dst, uint32_t transfer_size)
{
    const size_t cache_line_count = transfer_size / cache_line_size;

    _mm_mfence();
    for (size_t cache_line_index = 0; cache_line_index < cache_line_count; ++cache_line_index)
    {
        uint8_t *cache_line = dst + (cache_line_size * cache_line_index);

        _mm_clflush(cache_line);
    }
    _mm_mfence();
}

void dml_clwb(uint8_t *dst, uint32_t transfer_size)
{
    const size_t cache_line_count = transfer_size / cache_line_size;

    _mm_mfence();
    for (size_t cache_line_index = 0; cache_line_index < cache_line_count; ++cache_line_index)
    {
        uint8_t *cache_line = dst + (cache_line_size * cache_line_index);

        _mm_clwb(cache_line);
    }
    _mm_mfence();
}

void dml_clwb_unsupported(uint8_t *dst, uint32_t transfer_size)
{
    (void)dst;
    (void)transfer_size;
}