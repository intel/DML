/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <stddef.h>

#include "../dml_kernels.h"

void dml_ref_apply_delta(const uint8_t *delta_record, uint8_t *dst, uint32_t delta_record_size)
{
    typedef uint64_t block_t;
    typedef uint16_t offset_t;

    const size_t delta_note_size   = sizeof(block_t) + sizeof(offset_t);
    const size_t delta_notes_count = delta_record_size / delta_note_size;

    block_t *const dst_u64 = (block_t *)dst;

    for (size_t index = 0; index < delta_notes_count; ++index)
    {
        const uint8_t *const delta_note_position = delta_record + (delta_note_size * index);

        const offset_t offset = *(offset_t *)delta_note_position;
        const block_t  data   = *(block_t *)(delta_note_position + sizeof(offset_t));

        dst_u64[offset] = data;
    }
}
