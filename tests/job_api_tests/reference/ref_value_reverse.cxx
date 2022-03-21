/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains a defenitions of reference functions for value reversing
 * @date 6/3/2020
 *
 */

namespace dml::reference
{

    template<>
    [[nodiscard]] inline auto reverse_bytes<uint16_t>(uint16_t value) noexcept -> uint16_t
    {
        union
        {
            uint16_t value;
            uint8_t  bytes[2];
        } received_value, reversed_value;

        received_value.value = value;

        reversed_value.bytes[0] = received_value.bytes[1];
        reversed_value.bytes[1] = received_value.bytes[0];

        return reversed_value.value;
    }


    template<>
    [[nodiscard]] inline auto reverse_bytes<uint32_t>(uint32_t value) noexcept ->uint32_t
    {
        union
        {
            uint32_t value;
            uint8_t  bytes[4];
        } received_value, reversed_value;

        received_value.value = value;

        reversed_value.bytes[0] = received_value.bytes[3];
        reversed_value.bytes[1] = received_value.bytes[2];
        reversed_value.bytes[2] = received_value.bytes[1];
        reversed_value.bytes[3] = received_value.bytes[0];

        return reversed_value.value;
    }



    template<>
    [[nodiscard]] inline auto reverse_bits<uint8_t>(uint8_t value) noexcept -> uint8_t
    {
        value = ((value & 0x55u) << 1u) | ((value & 0xAAu) >> 1u);
        value = ((value & 0x33u) << 2u) | ((value & 0xCCu) >> 2u);
        value = ((value & 0x0Fu) << 4u) | ((value & 0xF0u) >> 4u);

        return value;
    }


    template<>
    [[nodiscard]] inline auto reverse_bits<uint16_t>(uint16_t value) noexcept -> uint16_t
    {
        value = (value & 0x55555555u) << 1u  | (value & 0xAAAAAAAAu) >> 1u;
        value = (value & 0x33333333u) << 2u  | (value & 0xCCCCCCCCu) >> 2u;
        value = (value & 0x0F0F0F0Fu) << 4u  | (value & 0xF0F0F0F0u) >> 4u;
        value = (value & 0x00FF00FFu) << 8u  | (value & 0xFF00FF00u) >> 8u;

        return value;
    }


    template<>
    [[nodiscard]] inline auto reverse_bits<uint32_t>(uint32_t value) noexcept ->uint32_t
    {
        value = (value & 0x55555555u) << 1u  | (value & 0xAAAAAAAAu) >> 1u;
        value = (value & 0x33333333u) << 2u  | (value & 0xCCCCCCCCu) >> 2u;
        value = (value & 0x0F0F0F0Fu) << 4u  | (value & 0xF0F0F0F0u) >> 4u;
        value = (value & 0x00FF00FFu) << 8u  | (value & 0xFF00FF00u) >> 8u;
        value = (value & 0x0000FFFFu) << 16u | (value & 0xFFFF0000u) >> 16u;

        return value;
    }
    
}
