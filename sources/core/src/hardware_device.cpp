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
#include "hw_dispatcher/numa.hpp"

namespace dml::core
{
#if defined(linux)
    static inline auto enqueue(const dispatcher::hw_device &device, const descriptor &dsc) noexcept
    {
        // Write 0 to completion record before submit
        auto &record = get_completion_record(dsc);
        for (auto& byte : record.bytes)
        {
            byte = 0u;
        }

        auto status = device.enqueue_descriptor(reinterpret_cast<const dsahw_descriptor_t *>(&dsc));

        return status == DML_STATUS_OK ? dml::detail::submission_status::success
                                       : dml::detail::submission_status::failure;
    }
#endif

    dml::detail::submission_status hardware_device::submit(const descriptor &dsc, std::uint32_t numa_id) noexcept
    {
#if defined(linux)
        const auto own_numa_id = (numa_id == std::numeric_limits<decltype(numa_id)>::max()) ? util::get_numa_id() : numa_id;

        auto &dispatcher = dispatcher::hw_dispatcher::get_instance();

        if (dispatcher.is_hw_support())
        {
            static thread_local auto current_device_idx = 0u;

            auto device_count  = std::distance(dispatcher.begin(), dispatcher.end());
            auto tried_devices = 0u;

            while (tried_devices < device_count)
            {
                auto &current_device = *(dispatcher.begin() + current_device_idx);
                current_device_idx   = (current_device_idx + 1) % device_count;

                if (own_numa_id != current_device.numa_id())
                {
                    tried_devices++;
                    continue;
                }

                auto status = enqueue(current_device, dsc);

                if (status != dml::detail::submission_status::success)
                {
                    tried_devices++;
                }
                else
                {
                    return status;
                }
            }

            return dml::detail::submission_status::queue_busy;
        }
#else
        static_cast<void>(dsc);
        static_cast<void>(numa_id);
#endif

        return dml::detail::submission_status::failure;
    }
}  // namespace dml::core
