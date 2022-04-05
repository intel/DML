/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <core/device.hpp>
#include <core/utils.hpp>
#include <core/validation.hpp>
#include <optimization_dispatcher.hpp>
#include <dml/detail/common/flags.hpp>
#include <dml/detail/ml/impl/core_interconnect.hpp>

#include "partial_completion.hpp"
#include "accumulate_records.hpp"

namespace dml::detail::ml::impl
{
    void rebind(descriptor& dsc, completion_record& record) noexcept
    {
        constexpr auto completion_record_flags =
            to_underlying(flag::completion_record_address_valid) |
            to_underlying(flag::request_completion_record);

        core::any_descriptor(dsc).completion_record_address() =
            reinterpret_cast<address_t>(&record);
        core::any_descriptor(dsc).flags() |= completion_record_flags;
    }

    [[nodiscard]] validation_status software::validate(const descriptor& dsc) noexcept
    {
        return core::validate(dsc);
    }

    submission_status software::submit(const descriptor& dsc, std::uint32_t numa_id) noexcept
    {
        static_cast<void>(numa_id);

        return core::software_device().submit(dsc);
    }

    void software::wait(const descriptor& dsc, bool umwait) noexcept
    {
        if (finished(dsc))
        {
            return;
        }

        auto& record = core::get_completion_record(dsc);

        if (umwait)
        {
            core::dispatch::wait_umwait(&record.bytes[0]);
        }
        else
        {
            core::dispatch::wait_busy_poll(&record.bytes[0]);
        }
    }

    bool software::finished(const descriptor& dsc) noexcept
    {
        return core::get_completion_record(dsc).bytes[0];
    }

    [[nodiscard]] validation_status hardware::validate(const descriptor& dsc) noexcept
    {
        return software::validate(dsc);
    }

    submission_status hardware::submit(const descriptor& dsc, std::uint32_t numa_id) noexcept
    {
        return core::hardware_device().submit(dsc, numa_id);
    }

    void hardware::wait(const descriptor& dsc, bool umwait) noexcept
    {
        software::wait(dsc, umwait);
    }

    bool hardware::finished(const descriptor& dsc) noexcept
    {
        return software::finished(dsc);
    }

    [[nodiscard]] validation_status automatic::validate(const descriptor& dsc) noexcept
    {
        return hardware::validate(dsc);
    }

    submission_status automatic::submit(const descriptor& dsc, std::uint32_t numa_id) noexcept
    {
        auto status = core::hardware_device().submit(dsc, numa_id);

        // SW Fallback
        if (status != submission_status::success)
        {
            return core::software_device().submit(dsc);
        }

        return status;
    }

    void automatic::wait(descriptor& dsc, bool umwait) noexcept
    {
        constexpr auto page_fault_mask =
            to_underlying(execution_status::page_fault_during_processing);

        hardware::wait(dsc, umwait);

        auto& record = core::get_completion_record(dsc);
        auto  status = core::any_completion_record(record).status();
        if ((status & page_fault_mask) == page_fault_mask)
        {
            // Clone
            auto prev_record = record;

            update_for_continuation(dsc);

            // Must not fail
            static_cast<void>(software::submit(dsc, 0));

            software::wait(dsc, umwait);

            accumulate_records(dsc, prev_record);
        }
    }

    bool automatic::finished(descriptor& dsc) noexcept
    {
        auto is_finished = hardware::finished(dsc);

        auto& record = core::get_completion_record(dsc);
        auto  status = static_cast<execution_status>(core::any_completion_record(record).status());
        if (is_finished && execution_status::page_fault_during_processing == status)
        {
            update_for_continuation(dsc);
            static_cast<void>(software::submit(dsc, 0));
            return software::finished(dsc);
        }

        return is_finished;
    }
}  // namespace dml::detail::ml::impl
