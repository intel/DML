/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @date 05/20/2021
 * @brief Contains internal execute implementation
 */

#ifndef DML_DETAIL_EXECUTE_HPP
#define DML_DETAIL_EXECUTE_HPP

#include <dml/detail/ml/result.hpp>
#include <dml/detail/ml/view.hpp>
#include <dml/hl/detail/utils.hpp>
#include <dml/hl/execution_path.hpp>
#include <dml/hl/status_code.hpp>

#include "make_result.hpp"

namespace dml::detail
{
    /**
     * @brief Provides common execute implementation
     *
     * @tparam execution_path   Type of execution path
     * @tparam operation        Type of operation
     * @tparam make_task_t      Type of callable make functor
     * @tparam range_check_t    Type of callable range check
     *
     * @param numa_id           Numa id for submission
     * @param make_task         Instance of callable make functor
     * @param range_check       Instance of callable range check
     *
     * @return Result of operation
     */
    template <typename execution_path_t, typename operation, typename make_task_t, typename range_check_t = detail::always_success>
    inline auto execute(std::uint32_t numa_id,
                        make_task_t &&make_task,
                        range_check_t range_check = range_check_t()) noexcept
    {
        using execution_path = typename execution_path_t::execution_path;

        if (auto status = range_check(); status != status_code::ok)
        {
            return typename operation::result_type{ status };
        }

        auto task = make_task();

        auto [validation_status, submission_status] = execute<execution_path>(make_view(task), numa_id);

        if (validation_status != detail::validation_status::success)
        {
            return typename operation::result_type{ detail::to_own(validation_status) };
        }

        if (submission_status != detail::submission_status::success)
        {
            return typename operation::result_type{ status_code::error };
        }

        return make_result<typename operation::result_type>(make_view(task).get_completion_record());
    }

}  // namespace dml::detail

#endif  //DML_DETAIL_EXECUTE_HPP
