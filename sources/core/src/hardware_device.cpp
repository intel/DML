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

#include <core/descriptor_views.hpp>
#include <core/operations.hpp>
#include <dml/detail/common/flags.hpp>
#include <dml/detail/common/status.hpp>

#include "core/device.hpp"
#include "hw_dispatcher/hw_dispatcher.hpp"
#include "hw_dispatcher/numa.hpp"

namespace dml::core
{
#ifdef DML_HW
    static inline auto enqueue(const dispatcher::hw_device &device, descriptor &dsc, completion_record &record) noexcept
    {
        auto view = any_descriptor(dsc);
        view.flags() |= static_cast<flags_t>(dml::detail::flag::completion_record_address_valid) |
                        static_cast<flags_t>(dml::detail::flag::request_completion_record);

        // Use BlockOnFault on hardware, until page fault handling is implemented in software side
        if (view.operation() != static_cast<operation_t>(operation::batch) &&
            view.operation() != static_cast<operation_t>(operation::drain) && view.operation() != static_cast<operation_t>(operation::nop))
        {
            view.flags() |= static_cast<flags_t>(dml::detail::flag::block_on_fault);
        }

        view.completion_record_address() = reinterpret_cast<address_t>(&record);
        record.bytes[0]                  = 0;

        auto status = device.enqueue_descriptor(reinterpret_cast<const dsahw_descriptor_t *>(&dsc));

        return status == DML_STATUS_OK ? dml::detail::submission_status::success : dml::detail::submission_status::failure;
    }
#endif

    dml::detail::submission_status hardware_device::submit(descriptor &dsc, completion_record &completion_record) noexcept
    {
#ifdef DML_HW
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

                if (util::get_numa_id() != current_device.numa_id())
                {
                    tried_devices++;
                    continue;
                }

                auto status = enqueue(current_device, dsc, completion_record);

                if (status != detail::submission_status::success)
                {
                    tried_devices++;
                }
                else
                {
                    return status;
                }
            }

            return detail::submission_status::queue_busy;
        }
#else
        static_cast<void>(dsc);
        static_cast<void>(completion_record);
#endif

        return dml::detail::submission_status::failure;
    }
}  // namespace dml::core
