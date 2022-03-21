/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains a defenition of reference function for crc calculation for uint32_t data type
 * @date 6/3/2020
 *
 */
#include "ref_value_reverse.hpp"
#include "dml/dmldefs.h"

namespace dml::reference
{

    template<typename crc_calculator_t<uint32_t>::flag_type_t flags, typename input_iterator>
    inline auto crc_calculator_t<uint32_t>::calculate(input_iterator first,
                                                      const input_iterator &last,
                                                      value_type_t crc_value) noexcept -> value_type_t
    {
        if constexpr (!(DML_FLAG_CRC_READ_SEED & flags))
        {
            crc_value = 0u;
        }

        if constexpr (!(DML_FLAG_CRC_BYPASS_REFLECTION & flags))
        {
            crc_value = (~crc_value);
            crc_value = dml::reference::reverse_bits<value_type_t>(crc_value);
        }

        if constexpr (!(DML_FLAG_CRC_BYPASS_DATA_REFLECTION & flags))
        {
            while (first != last)
            {
                crc_value = crc_value ^ (dml::reference::reverse_bits<uint8_t>(*first) << m_BYTE_SHIFT);

                for (auto bit = 0u; bit < m_BYTE_BIT_LENGTH; ++bit)
                {
                    crc_value = (crc_value & m_HIGHT_BIT_MASK) ?
                                ((crc_value << 1u) ^ m_POLYNOMIAL) :
                                (crc_value << 1u);
                }

                first++;
            }
        }
        else
        {
            while (first != last)
            {
                crc_value = crc_value ^ ((*first) << m_BYTE_SHIFT);

                for (auto bit = 0u; bit < m_BYTE_BIT_LENGTH; ++bit)
                {
                    crc_value = (crc_value & m_HIGHT_BIT_MASK) ?
                                ((crc_value << 1u) ^ m_POLYNOMIAL) :
                                (crc_value << 1u);
                }

                first++;
            }
        }

        if constexpr (!(DML_FLAG_CRC_BYPASS_REFLECTION & flags))
        {
            crc_value = (~crc_value);
            crc_value = dml::reference::reverse_bits<value_type_t>(crc_value);
        }

        return crc_value;
    }

}
