/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <stddef.h>

#include "../dml_kernels.h"

void dml_ref_dualcast(const uint8_t *src, uint8_t *dst1, uint8_t *dst2, uint32_t transfer_size)
{
    for (size_t index = 0; index < transfer_size; ++index)
    {
        dst1[index] = src[index];
        dst2[index] = src[index];
    }
}
