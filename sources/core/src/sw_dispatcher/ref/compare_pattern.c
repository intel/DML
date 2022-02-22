/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <stddef.h>

#include "../dml_kernels.h"

uint32_t dml_ref_compare_pattern(uint64_t pattern, const uint8_t *src, uint32_t transfer_size, uint8_t *result)
{
    const uint8_t equal     = 0x0;
    const uint8_t not_equal = 0x1;

    const size_t chunk_size = sizeof(pattern);
    const size_t head_size  = transfer_size / chunk_size;
    const size_t tail_size  = transfer_size % chunk_size;

    const uint64_t *const head = (const uint64_t *)src;
    const uint8_t *const  tail = src + chunk_size * head_size;

    for (size_t index = 0; index < head_size; ++index)
    {
        if (head[index] != pattern)
        {
            *result = not_equal;
            return (uint32_t)(index * chunk_size);
        }
    }

    const uint8_t *const pattern_u8 = (uint8_t *)&pattern;

    for (size_t index = 0; index < tail_size; ++index)
    {
        // No overflow for pattern. See tail_size calculation.
        if (tail[index] != pattern_u8[index])
        {
            *result = not_equal;
            return (uint32_t)(chunk_size * head_size + index);
        }
    }

    *result = equal;
    return 0;
}
