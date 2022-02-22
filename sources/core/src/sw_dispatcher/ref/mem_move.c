/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <stddef.h>

#include "../dml_kernels.h"

static inline void own_copy_forward(const uint8_t *const src, uint8_t *const dst, uint32_t transfer_size)
{
    for (size_t index = 0; index < transfer_size; ++index)
    {
        dst[index] = src[index];
    }
}

static inline void own_copy_backward(const uint8_t *const src, uint8_t *const dst, uint32_t transfer_size)
{
    const size_t last_index  = transfer_size - 1;
    const size_t first_index = 0;

    for (size_t index = last_index; index != first_index; --index)
    {
        dst[index] = src[index];
    }

    dst[first_index] = src[first_index];
}

void dml_ref_mem_move(const uint8_t *src, uint8_t *dst, uint32_t transfer_size)
{
    const uint8_t *const src_begin = src;
    const uint8_t *const src_end   = src + transfer_size;
    const uint8_t *const dst_begin = dst;
    const uint8_t *const dst_end   = dst + transfer_size;

    /*
     * Either:
     * src: |-------|
     * dst:          |-------|
     *
     * OR:
     * src:          |-------|
     * dst: |-------|
     *
     * Assume ranges are exclusive, then equality is taken into account
     *
     * Any copy is safe
     */
    if (src_end <= dst_begin || src_begin >= dst_end)
    {
        own_copy_forward(src, dst, transfer_size);
    }
    /*
     * src:     |-------|
     * dst: |-------|
     *
     * Only forward copy is applicable
     */
    else if (src_begin < dst_end && src_end > dst_end)
    {
        own_copy_forward(src, dst, transfer_size);
    }
    /*
     * src: |-------|
     * dst:     |-------|
     *
     * Only backward copy is applicable
     */
    else if (src_begin < dst_begin && src_end > dst_begin)
    {
        own_copy_backward(src, dst, transfer_size);
    }
    /*
     * src: |-------|
     * dst: |-------|
     *
     * The same memory regions
     */
    else
    {
        // Do nothing
    }
}
