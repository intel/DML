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

#include <dml/detail/ml/task.hpp>

namespace dml
{
    template <typename operation, typename allocator_t>
    class handler;

    namespace detail
    {
        /**
         * @brief Helper to retrieve descriptor from a handler
         *
         * @tparam operation   Type of operation
         * @tparam allocator_t Type of allocator
         * @param h            Instance of @ref handler
         *
         * @return Descriptor
         */
        template <typename operation, typename allocator_t>
        dml::detail::ml::task_view get_task_view(handler<operation, allocator_t> &h) noexcept
        {
            return make_view(h.task_);
        }

        /**
         * @todo
         */
        template <typename operation, typename allocator_t>
        void set_hw_path(handler<operation, allocator_t> &h) noexcept
        {
            h.is_hw = true;
        }

        /**
         * @todo
         */
        template <typename operation, typename allocator_t>
        void set_status(handler<operation, allocator_t> &h, status_code status) noexcept
        {
            h.status_ = status;
        }
    }  // namespace detail
}  // namespace dml

#endif  //DML_DETAIL_HANDLER_HPP
