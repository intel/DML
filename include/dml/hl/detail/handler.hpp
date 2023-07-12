/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

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
        template <typename execution_path_t, typename operation, typename allocator_t>
        void set_path(handler<operation, allocator_t> &h) noexcept
        {
            if (std::is_same_v<execution_path_t, software>){
                h.path_ = handler<operation, allocator_t>::path_e::software_e;
            }
            else if (std::is_same_v<execution_path_t, hardware>){
                h.path_ = handler<operation, allocator_t>::path_e::hardware_e;
            }
            else if (std::is_same_v<execution_path_t, automatic>){
                h.path_ = handler<operation, allocator_t>::path_e::automatic_e;
            }
            else {
                h.path_ = handler<operation, allocator_t>::path_e::software_e;
            }
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
