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

#ifndef DML_COMMON_UTILS_HPP
#define DML_COMMON_UTILS_HPP

#include <cstring>

#ifdef DML_USE_CPP20
    #define DML_DESIGNATED_INIT(name, value) .name = value
#else
    #define DML_DESIGNATED_INIT(name, value) value
#endif

#define DML_DESIGNATED_INIT_EMPTY(name) DML_DESIGNATED_INIT(name, {})

namespace dml::ml
{
    /**
     * @todo
     */
    template <typename to_t, typename from_t>
    [[nodiscard]] constexpr to_t bit_cast(const from_t& from) noexcept
    {
        static_assert(sizeof(to_t) == sizeof(from_t));

        auto to = to_t();
        std::memcpy(&to, &from, sizeof(to_t));

        return to;
    }
}  // namespace dml::ml
#endif  //DML_COMMON_UTILS_HPP
