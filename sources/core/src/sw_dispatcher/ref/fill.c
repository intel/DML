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
