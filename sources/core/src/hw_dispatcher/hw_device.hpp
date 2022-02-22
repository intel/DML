/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_MIDDLE_LAYER_DISPATCHER_HW_DEVICE_HPP_
#define DML_MIDDLE_LAYER_DISPATCHER_HW_DEVICE_HPP_

#include <array>

#include "dml/dmldefs.h"
#include "hw_queue.hpp"

#if defined(linux)
#include "legacy_headers/hardware_definitions.h"
#include "legacy_headers/own_dsa_accel_constants.h"

namespace dml::core::dispatcher
{

    class hw_device final
    {
        static constexpr uint32_t max_working_queues = MAX_WORK_QUEUE_COUNT;

        using queues_container_t = std::array<hw_queue, max_working_queues>;

    public:
        using descriptor_t = void;

        hw_device() noexcept = default;

        void fill_hw_context(dsahw_context_t *hw_context_ptr) const noexcept;

        [[nodiscard]] auto enqueue_descriptor(const dsahw_descriptor_t *desc_ptr) const noexcept -> dsahw_status_t;

        [[nodiscard]] auto initialize_new_device(descriptor_t *device_descriptor_ptr) noexcept -> dsahw_status_t;

        [[nodiscard]] auto size() const noexcept -> size_t;

        [[nodiscard]] auto numa_id() const noexcept -> uint64_t;

        [[nodiscard]] auto begin() const noexcept -> queues_container_t::const_iterator;

        [[nodiscard]] auto end() const noexcept -> queues_container_t::const_iterator;

    protected:
        auto block_on_fault_support() const noexcept -> uint8_t;

        auto overlapping_copy_support() const noexcept -> uint8_t;

        auto memory_cache_control_support() const noexcept -> uint8_t;

        auto flush_cache_control_support() const noexcept -> uint8_t;

        auto destination_readback_support() const noexcept -> uint8_t;

        auto descriptor_readback_support() const noexcept -> uint8_t;

        auto max_transfer_size() const noexcept -> uint32_t;

        auto max_batch_size() const noexcept -> uint32_t;

        auto message_size() const noexcept -> uint16_t;

        auto configuration_support() const noexcept -> uint8_t;

    private:
        queues_container_t working_queues_   = {}; /**< Set of available HW working queues */
        uint32_t           queue_count_      = 0u; /**< Number of working queues that are available */
        uint64_t           gen_cap_register_ = 0u; /**< GENCAP register content */
        uint64_t           numa_node_id_     = 0u; /**< NUMA node id of the device */
        uint32_t           version_major_    = 0u; /**< Major version of discovered device */
        uint32_t           version_minor_    = 0u; /**< Minor version of discovered device */
    };

}  // namespace dml::core::dispatcher

#endif
#endif  //DML_MIDDLE_LAYER_DISPATCHER_HW_DEVICE_HPP_
