/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <stddef.h>

#include "../dml_kernels.h"

static inline uint8_t reverse(uint8_t byte)
{
    byte = ((byte & 0x55u) << 1u) | ((byte & 0xAAu) >> 1u);
    byte = ((byte & 0x33u) << 2u) | ((byte & 0xCCu) >> 2u);
    byte = ((byte & 0x0Fu) << 4u) | ((byte & 0xF0u) >> 4u);

    return byte;
}

static inline uint32_t calculate_crc_32u(uint32_t crc_value, uint8_t data, uint32_t polynomial)
{
    const size_t   byte_width     = 8;
    const size_t   crc_bit_count  = sizeof(crc_value) * byte_width;
    const size_t   crc_byte_shift = crc_bit_count - byte_width;
    const uint32_t high_bit_mask  = 1 << (crc_bit_count - 1);

    crc_value ^= (data << crc_byte_shift);

    for (size_t bit = 0u; bit < byte_width; ++bit)
    {
        crc_value = (crc_value & high_bit_mask) ? ((crc_value << 1) ^ polynomial) : (crc_value << 1);
    }

    return crc_value;
}

uint32_t dml_ref_crc_32u(const uint8_t *src, uint32_t transfer_size, uint32_t crc_value, uint32_t polynomial)
{
    for (size_t byte = 0; byte < transfer_size; ++byte)
    {
        crc_value = calculate_crc_32u(crc_value, src[byte], polynomial);
    }

    return crc_value;
}

uint32_t dml_ref_crc_reflected_u32(const uint8_t *src, uint32_t transfer_size, uint32_t crc_value, uint32_t polynomial)
{
    for (size_t byte = 0; byte < transfer_size; ++byte)
    {
        crc_value = calculate_crc_32u(crc_value, reverse(src[byte]), polynomial);
    }

    return crc_value;
}
