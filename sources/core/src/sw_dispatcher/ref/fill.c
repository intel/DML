/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <stddef.h>

#include "../dml_kernels.h"

void dml_ref_fill_u64(uint64_t pattern, uint8_t *dst, uint32_t transfer_size)
{
    const uint8_t *const pattern_bytes = (const uint8_t *)&pattern;

    for (size_t index = 0; index < transfer_size; ++index)
    {
        dst[index] = pattern_bytes[index % sizeof(pattern)];
    }
}
