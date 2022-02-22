/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <stddef.h>

#include "../dml_kernels.h"

uint32_t dml_ref_compare(const uint8_t* src1, const uint8_t* src2, uint32_t transfer_size, uint8_t* result)
{
    const uint8_t equal     = 0x0;
    const uint8_t not_equal = 0x1;

    for (size_t index = 0; index < transfer_size; ++index)
    {
        if (src1[index] != src2[index])
        {
            *result = not_equal;
            return (uint32_t)index;
        }
    }

    *result = equal;
    return 0;
}
