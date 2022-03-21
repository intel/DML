/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains a defenitions of reference functions for crc calculation
 * @date 6/3/2020
 *
 */
#ifndef DML_REF_CRC
#define DML_REF_CRC


#include <cstdint>


namespace dml::reference
{

    /**
     * @brief Class that performs crc calculation
     */
    template<typename value_type>
    class crc_calculator_t
    {
    public:
        /**
         * @brief Wrapper for value type
         */
        using value_type_t = value_type;
        /**
         * @brief Wrapper for flag type
         */
        using flag_type_t  = uint32_t;


        
        /**
         * @brief Calculates crc value from input range
         */
        template<flag_type_t flags, typename input_iterator>
        [[nodiscard]] static auto calculate(input_iterator first, 
                                            const input_iterator &last, 
                                            value_type_t crc_seed) noexcept -> value_type_t;
    };


    /**
     * @brief Specialization for uint32_t datatype
     */
    template<>
    class crc_calculator_t<uint32_t>
    {
    public:
        /**
         * @brief Wrapper for value type
         */
        using value_type_t = uint32_t;
        /**
         * @brief Wrapper for flag type
         */
        using flag_type_t  = uint32_t;



        /**
         * @brief Calculates crc value from input range
         */
        template<flag_type_t flags, typename input_iterator>
        [[nodiscard]] static auto calculate(input_iterator first, 
                                            const input_iterator &last, 
                                            value_type_t crc_seed) noexcept -> value_type_t;

    private:
        /**
         * @brief Bit length of a byte
         */
        static constexpr auto m_BYTE_BIT_LENGTH = value_type_t(8u);
        /**
         * @brief Top bit of input datatype
         */
        static constexpr auto m_HIGHT_BIT_MASK  = value_type_t(0x80000000u);
        /**
         * @brief Shift value for crc calculation
         */
        static constexpr auto m_BYTE_SHIFT      = value_type_t(24u);
        /**
         * @brief Polynomial for calulation
         */
        static constexpr auto m_POLYNOMIAL      = value_type_t(0x1EDC6F41u);
    };


    /**
     * @brief Calculates crc value from input range without dependency of static class functions
     */
    template<typename value_type, 
             typename crc_calculator_t<value_type>::flag_type_t flags, 
             typename input_iterator>
    [[nodiscard]] inline auto calculate_crc(input_iterator first, 
                                            const input_iterator &last, 
                                            const value_type crc_seed) 
        noexcept -> typename crc_calculator_t<value_type>::value_type_t;

}


#include "ref_crc.cxx"
#include "ref_crc_32u.cxx"


#endif // DML_REF_CRC
