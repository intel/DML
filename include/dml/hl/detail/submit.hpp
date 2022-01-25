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
 * @brief Contains internal submit implementation
 */

#ifndef DML_DETAIL_SUBMIT_HPP
#define DML_DETAIL_SUBMIT_HPP

#include <dml/hl/detail/handler.hpp>
#include <dml/hl/detail/utils.hpp>
#include <dml/hl/execution_path.hpp>

namespace dml::detail
{
    /**
     * @brief Provides common submit implementation
     *
     * @tparam execution_path        Type of execution path
     * @tparam operation_t           Type of operation
     * @tparam execution_interface_t Type of execution interface
     * @tparam range_check_t         Type of callable range check
     * @tparam make_task_t           Type of callable make functor
     * @param executor               Instance of execution interface
     * @param range_check            Instance of callable range check
     * @param make_task              Instance of callable make functor
     *
     * @return Handler for operation
     */
    template <typename execution_path_t,
              typename operation_t,
              typename execution_interface_t,
              typename make_task_t,
              typename range_check_t = detail::always_success>
    inline auto submit(const execution_interface_t& executor,
                       make_task_t&&           make_task,
                       range_check_t                range_check = range_check_t())
    {
        constexpr auto numa = std::numeric_limits<std::uint32_t>::max();

        using execution_path = typename execution_path_t::execution_path;

        auto op_handler = executor.template make_handler<operation_t>(make_task());

        if (auto status = range_check(); status != status_code::ok)
        {
            detail::set_status(op_handler, status);
            return op_handler;
        }

        auto task_view = detail::get_task_view(op_handler);

        auto [validation_status, submission_status] = submit<execution_path>(task_view, numa);

        if (validation_status != detail::validation_status::success)
        {
            detail::set_status(op_handler, to_own(validation_status));
            return op_handler;
        }

        if (submission_status != detail::submission_status::success)
        {
            detail::set_status(op_handler, status_code::error);
            return op_handler;
        }

        if constexpr (std::is_same_v<execution_path, hardware>)
        {
            detail::set_hw_path(op_handler);
        }

        return op_handler;
    }

}  // namespace dml::detail

#endif  //DML_DETAIL_SUBMIT_HPP
