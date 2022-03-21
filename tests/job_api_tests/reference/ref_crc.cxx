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
namespace dml::reference
{

    template<typename value_type, 
             typename crc_calculator_t<value_type>::flag_type_t flags, 
             typename input_iterator>
    inline auto calculate_crc(input_iterator first, 
                              const input_iterator &last, 
                              const value_type crc_seed) 
        noexcept -> typename crc_calculator_t<value_type>::value_type_t
    {
        return crc_calculator_t<value_type>::template calculate<flags, input_iterator>(first, last, crc_seed);
    }

}
