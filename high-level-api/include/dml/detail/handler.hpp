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

/**
 * @date 05/20/2021
 * @brief Contains handler internal functionality
 */

#ifndef DML_DETAIL_HANDLER_HPP
#define DML_DETAIL_HANDLER_HPP

#include <dml_ml/result.hpp>

namespace dml
{
    template <typename operation, typename allocator_t>
    class handler;

    namespace detail
    {
        /**
         * @brief Helper to retrieve Middle Layer result from a handler
         *
         * @tparam operation   Type of operation
         * @tparam allocator_t Type of allocator
         * @param h            Instance of @ref handler
         *
         * @return Middle Layer result object
         */
        template <typename operation, typename allocator_t>
        ml::result &get_ml_result(handler<operation, allocator_t> &h) noexcept
        {
            return h.record_.get();
        }
    }  // namespace detail
}  // namespace dml

#endif  //DML_DETAIL_HANDLER_HPP
