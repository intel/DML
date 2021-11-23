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

#include <dml/cpp/middle_layer/device.hpp>
#include <hw_dispatcher.hpp>
#include <numa.hpp>

namespace dml::ml
{
#ifdef DML_HW
    submission_status hardware::submit(descriptor &dsc, completion_record &record) noexcept
    {
        static auto                &dispatcher_instance = dispatcher::hw_dispatcher::get_instance();
        static thread_local int32_t numa_id             = util::get_numa_id();

        if (dispatcher_instance.is_hw_support())
        {
            const auto n_devices = std::distance(dispatcher_instance.begin(), dispatcher_instance.end());

            // Initially set to "end" index
            static auto last_device_idx = std::atomic(n_devices);

            // Loop FROM the device after the one used for last submit
            for (auto device_idx = last_device_idx.load() + 1; device_idx < n_devices; ++device_idx)
            {
                auto &device = *(dispatcher_instance.begin() + device_idx);
                if (device.numa_id() != numa_id)
                {
                    continue;
                }

                auto view = views::any_descriptor(dsc);
                view.flags() |=
                    static_cast<flags_t>(flag::completion_record_address_valid) | static_cast<flags_t>(flag::request_completion_record);

                // Use BlockOnFault on hardware, until page fault handling is implemented in software side
                if (view.operation() != static_cast<operation_t>(operation::batch) &&
                    view.operation() != static_cast<operation_t>(operation::drain) &&
                    view.operation() != static_cast<operation_t>(operation::nop))
                {
                    view.flags() |= static_cast<flags_t>(flag::block_on_fault);
                }

                view.completion_record_address() = reinterpret_cast<address_t>(&record);
                record.bytes[0]                  = 0;

                auto status = device.enqueue_descriptor(reinterpret_cast<const dsahw_descriptor_t *>(&dsc));

                if (status == DML_STATUS_OK)
                {
                    last_device_idx = device_idx;
                    return submission_status::success;
                }
            }

            // If the loop before didn't submit descriptor, then loop UNTIL the device that was used for last submit
            for (auto device_idx = 0; device_idx <= last_device_idx; ++device_idx)
            {
                auto &device = *(dispatcher_instance.begin() + device_idx);
                if (device.numa_id() != numa_id)
                {
                    continue;
                }

                auto view = views::any_descriptor(dsc);
                view.flags() |=
                    static_cast<flags_t>(flag::completion_record_address_valid) | static_cast<flags_t>(flag::request_completion_record);

                // Use BlockOnFault on hardware, until page fault handling is implemented in software side
                if (view.operation() != static_cast<operation_t>(operation::batch) &&
                    view.operation() != static_cast<operation_t>(operation::drain) &&
                    view.operation() != static_cast<operation_t>(operation::nop))
                {
                    view.flags() |= static_cast<flags_t>(flag::block_on_fault);
                }

                view.completion_record_address() = reinterpret_cast<address_t>(&record);
                record.bytes[0]                  = 0;

                auto status = device.enqueue_descriptor(reinterpret_cast<const dsahw_descriptor_t *>(&dsc));

                if (status == DML_STATUS_OK)
                {
                    last_device_idx = device_idx;
                    return submission_status::success;
                }
            }
        }

        return submission_status::failure;
    }
#endif
}  // namespace dml::ml
