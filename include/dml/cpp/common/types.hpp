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
     * @brief Specifies whether result is expected to be "equal" or "not equal".
     */
    enum class comparison_result : uint8_t
    {
        equal     = 0u, /**< @todo */
        not_equal = 1u, /**< @todo */
        overflow  = 2u  /**< @todo */
    };

    /**
     * @}
     */
}  // namespace dml

#endif  //DML_COMMON_TYPES_HPP
