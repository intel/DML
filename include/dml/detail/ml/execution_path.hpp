/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_ML_EXECUTION_PATH
#define DML_ML_EXECUTION_PATH

#include <dml/detail/common/status.hpp>
#include <dml/detail/ml/result.hpp>
#include <dml/detail/ml/impl/core_interconnect.hpp>
#include <tuple>

namespace dml::detail::ml
{
    struct execution_path
    {
        using software = impl::software;

        using hardware = impl::hardware;

        using automatic = impl::automatic;
    };

    template <typename execution_path_t, typename task_view_t>
    [[nodiscard]] static std::tuple<validation_status, submission_status> submit(task_view_t view, std::uint32_t numa_id) noexcept
    {
        if (auto status = execution_path_t::validate(view.get_descriptor()); status != validation_status::success)
        {
            // TODO: Might be nice to use completion record here. Or core::validate might write into record.
            return { status, submission_status::failure };
        }

        return { validation_status::success, execution_path_t::submit(view.get_descriptor(), numa_id) };
    }

    template <typename execution_path_t, typename task_view_t>
    [[nodiscard]] static std::tuple<validation_status, submission_status> execute(task_view_t view, std::uint32_t numa_id) noexcept
    {
        auto statuses = submit<execution_path_t>(view, numa_id);

        if (statuses != std::make_tuple(validation_status::success, submission_status::success))
        {
            return statuses;
        }

        execution_path_t::wait(view.get_descriptor());

        return statuses;
    }

    template <typename execution_path_t, typename task_view_t>
    static void wait(task_view_t view) noexcept
    {
        execution_path_t::wait(view.get_descriptor());
    }

    template <typename execution_path_t, typename task_view_t>
    [[nodiscard]] static bool finished(task_view_t view) noexcept
    {
        return execution_path_t::finished(view.get_descriptor());
    }
}  // namespace dml::detail::ml

#endif  //DML_ML_EXECUTION_PATH
