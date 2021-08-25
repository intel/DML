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

#include "own/definitions.hpp"
#include "own/types.hpp"

#include <dml_ml/hardware_path.hpp>
#include <dml_ml/result.hpp>
#include <hardware_api.h>

#include "hw_dispatcher.hpp"
#include "numa.hpp"

namespace dml::ml
{
    DML_PACKED_STRUCT_DECLARATION_BEGIN(any_operation_descriptor)
    {
        byte_t       reserved_memory1[4]{};   /**< Not used bytes in the descriptor */
        hw_option    general_flags{};         /**< Contains a common flags for different operations */
        byte_t       reserved_memory2[1]{};   /**< Not used bytes in the descriptor */
        hw_operation operation_type{};        /**< Contains an operation type @ref dml_operation_t */
        result      *completion_record_ptr{}; /**< Pointer to the completion record space */
        byte_t       reserved_memory3[48]{};  /**< Not used bytes in the descriptor */
    };
    DML_PACKED_STRUCT_DECLARATION_END

status_code hardware_path::submit(operation op, result& res) noexcept {
    static auto                 &dispatcher_instance = dispatcher::hw_dispatcher::get_instance();
    static thread_local int32_t numa_id              = util::get_numa_id();

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

        // Use BlockOnFault on hardware, until page fault handling is implemented in software side
        auto dsc = reinterpret_cast<any_operation_descriptor*>(op.data());
        dsc->general_flags = dsc->general_flags | hw_option::block_on_fault;

        op.associate(res);
        auto status = device.enqueue_descriptor(reinterpret_cast<const dsahw_descriptor_t *>(&op));

            if (status == DML_STATUS_OK)
            {
                last_device_idx = device_idx;
                return status_code::ok;
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

            // Use BlockOnFault on hardware, until page fault handling is implemented in software side
            auto dsc = reinterpret_cast<any_operation_descriptor*>(op.data());
            dsc->general_flags = dsc->general_flags | hw_option::block_on_fault;

            op.associate(res);
            auto status = device.enqueue_descriptor(reinterpret_cast<const dsahw_descriptor_t *>(&op));

            if (status == DML_STATUS_OK)
            {
                last_device_idx = device_idx;
                return status_code::ok;
            }
        }

        return status_code::error;
    }

}  // namespace dml::ml
