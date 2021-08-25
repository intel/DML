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

#ifndef DML_COMMON_TYPES_HPP
#define DML_COMMON_TYPES_HPP

/**
 * @date 05/19/2021
 * @brief Contains definition of common types
 */

/**
 * @defgroup dml_cpp_common DML Common C++
 * @brief Common functionality for DML Library C++ code
 */

#include <cstdint>

namespace dml
{
    /**
     * @defgroup dml_cpp_common_types Types
     * @ingroup dml_cpp_common
     * @brief Contains types descriptions
     * @{
     */

    /**
     * @brief Alias type used to represent sizes
     */
    using size_t = std::uint32_t;

    /**
     * @brief Alias type used to represent bytes
     */
    using byte_t = std::uint8_t;

    /**
     * @brief Alias type used to represent result
     */
    using result_t = std::uint8_t;

    /**
     * @brief CRC parameters structure
     */
    struct crc_parameters
    {
        bool bypass_reflection{false};      /**< Do not use reverse bit order and reflection for CRC seed and result */
        bool bypass_data_reflection{false}; /**< Bit 7 of each data byte is the MSB in the CRC computation           */
    };

    /**
     * @brief Specifies whether result is expected to be "equal" or "not equal".
     */
    enum class equality
    {
        not_specified, /**< Expected result is not specified */
        equal,         /**< Expected equality */
        not_equal      /**< Expected inequality */
    };

    /**
     * @}
     */
}  // namespace dml

#endif  //DML_COMMON_TYPES_HPP
