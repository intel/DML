/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

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
     * @param numa_id                Numa id for submission
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
    inline auto submit(std::uint32_t numa_id,
                       const execution_interface_t& executor,
                       make_task_t&&           make_task,
                       range_check_t                range_check = range_check_t())
    {
        using execution_path = typename execution_path_t::execution_path;

        auto op_handler = executor.template make_handler<operation_t>(make_task());

        if (auto status = range_check(); status != status_code::ok)
        {
            detail::set_status(op_handler, status);
            return op_handler;
        }

        auto task_view = detail::get_task_view(op_handler);

        auto [validation_status, submission_status] = submit<execution_path>(task_view, numa_id);

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
