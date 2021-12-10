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
