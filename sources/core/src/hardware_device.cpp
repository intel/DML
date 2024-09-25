/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <core/device.hpp>
#include <core/utils.hpp>
#include <limits>
#include <dml/detail/common/status.hpp>

#include "hw_dispatcher/hw_dispatcher.hpp"
#include "hw_dispatcher/topology.hpp"

namespace dml::core
{
#if defined(__linux__)
    static inline auto enqueue(const dispatcher::hw_device &device, const descriptor &dsc) noexcept
    {
        // Write 0 to completion record before submit
        auto &record = get_completion_record(dsc);
        for (auto& byte : record.bytes)
        {
            byte = 0u;
        }

        auto status = device.enqueue_descriptor(reinterpret_cast<const dsahw_descriptor_t *>(&dsc));

        switch (status) {
            case DML_STATUS_OK:
                return dml::detail::submission_status::success;
                break;
            case DML_STATUS_NOT_SUPPORTED_BY_WQ:
                return dml::detail::submission_status::operation_unsupported;
                break;
            default:
                return dml::detail::submission_status::failure;
                break;
        }
    }
#endif

    dml::detail::submission_status hardware_device::submit(const descriptor &dsc, std::uint32_t user_specified_numa_id) noexcept
    {
#if defined(__linux__)
        auto &dispatcher = dispatcher::hw_dispatcher::get_instance();
        const size_t device_count = dispatcher.device_count();

        size_t devices_with_operation_disabled = 0;

        if (dispatcher.is_hw_support())
        {
            if (!dispatcher.is_using_mmap() && operation(any_descriptor(dsc).operation()) == operation::batch)
            {
                return dml::detail::submission_status::failure;
            }
            static thread_local auto current_device_idx = 0u;
            size_t tried_devices = 0u;

            while (tried_devices < device_count)
            {
                const auto &current_device = dispatcher.device(current_device_idx);
                current_device_idx = (current_device_idx + 1) % device_count;

                if (!current_device.is_matching_user_numa_policy(user_specified_numa_id))
                {
                    tried_devices++;
                    continue;
                }

                auto status = enqueue(current_device, dsc);

                if (status != dml::detail::submission_status::success)
                {
                    if (status == dml::detail::submission_status::operation_unsupported)
                    {
                        devices_with_operation_disabled++;
                    }
                    tried_devices++;
                }
                else
                {
                    return status;
                }
            }
            if (devices_with_operation_disabled == device_count)
            {
                return dml::detail::submission_status::operation_unsupported;
            }
            return dml::detail::submission_status::queue_busy;
        }
#else
        static_cast<void>(dsc);
        static_cast<void>(user_specified_numa_id);
#endif

        return dml::detail::submission_status::failure;
    }
}  // namespace dml::core
