/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains a declarations of reference functions for value reversing
 * @date 6/3/2020
 *
 */
#ifndef DML_REF_VALUE_REVERSE
#define DML_REF_VALUE_REVERSE


#include <cstdint>


namespace dml::reference
{

    /**
     * @brief Reverses byte order of significant value between little-endian and big-endian forms
     *
     * @tparam value_type    type of the value to reverse
     *
     * @param[in] value    value to reverse
     *
     * @return reversed value
     */
    template<typename value_type>
    [[nodiscard]] inline auto reverse_bytes(value_type value) noexcept -> value_type;


    /**
      * @brief Reverses byte order of 16-bit value between little-endian and big-endian forms
      *
      * @param[in] value    value to reverse
      *
      * @return reversed value
      */
    template<>
    [[nodiscard]] inline auto reverse_bytes<uint16_t>(uint16_t value) noexcept -> uint16_t;


    /**
      * @brief Reverses byte order of 32-bit value between little-endian and big-endian forms
      *
      * @param[in] value    value to reverse
      *
      * @return reversed value
      */
    template<>
    [[nodiscard]] inline auto reverse_bytes<uint32_t>(uint32_t value) noexcept -> uint32_t;



    /**
     * @brief Reverses bit order of significant value
     *
     * @tparam value_type    type of the value to reverse
     *
     * @param[in] value    value to reverse
     *
     * @return reversed value
     */
    template<typename value_type>
    [[nodiscard]] inline auto reverse_bits(value_type value) noexcept -> value_type;


    /**
      * @brief Reverses bit order of 8-bit value
      *
      * @param[in] value    value to reverse
      *
      * @return reversed value
      */
    template<>
    [[nodiscard]] inline auto reverse_bits<uint8_t>(uint8_t value) noexcept -> uint8_t;


    /**
      * @brief Reverses bit order of 16-bit value
      *
      * @param[in] value    value to reverse
      *
      * @return reversed value
      */
    template<>
    [[nodiscard]] inline auto reverse_bits<uint16_t>(uint16_t value) noexcept -> uint16_t;


    /**
      * @brief Reverses bit order of 32-bit value
      *
      * @param[in] value    value to reverse
      *
      * @return reversed value
      */
    template<>
    [[nodiscard]] inline auto reverse_bits<uint32_t>(uint32_t value) noexcept -> uint32_t;

}


#include "ref_value_reverse.cxx"


#endif // DML_REF_VALUE_REVERSE
