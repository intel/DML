/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <stddef.h>

#include "../dml_kernels.h"

uint32_t dml_ref_create_delta(const uint8_t *src1,
                              const uint8_t *src2,
                              uint32_t       transfer_size,
                              uint8_t       *delta_record,
                              uint32_t       delta_record_max_size,
                              uint8_t       *result)
{
    typedef uint64_t block_t;
    typedef uint16_t offset_t;

    const size_t delta_note_size = sizeof(block_t) + sizeof(offset_t);
    const size_t block_count     = transfer_size / sizeof(block_t);

    uint32_t delta_record_size = 0;

    for (size_t index = 0; index < block_count; ++index)
    {
        const block_t block1 = *(((block_t *)src1) + index);
        const block_t block2 = *(((block_t *)src2) + index);

        if (block1 != block2)
        {
            if ((delta_record_size + delta_note_size) > delta_record_max_size)
            {
                const uint8_t overflow = 0x2;

                *result = overflow;
                return delta_record_size;
            }

            uint8_t *const  delta_position = delta_record + delta_record_size;
            offset_t *const offset         = (offset_t *)delta_position;
            block_t *const  data           = (block_t *)(delta_position + sizeof(offset_t));

            *offset = (offset_t)index;
            *data   = block2;

            delta_record_size += (uint32_t)delta_note_size;
        }
    }

    const uint8_t equal     = 0x0;
    const uint8_t not_equal = 0x1;

    *result = delta_record_size ? not_equal : equal;

    return delta_record_size;
}
