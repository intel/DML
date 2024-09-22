/*******************************************************************************
 * Copyright (C) 2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/*
 * HW device APIs for tests
 */

#if defined(__linux__)

#include "test_hw_device.hpp"

#include <algorithm>
#include <queue>

constexpr uint8_t  accelerator_name[]      = "dsa";                         /**< Accelerator name */
constexpr uint32_t accelerator_name_length = sizeof(accelerator_name) - 2U; /**< Last symbol index */

/**
 * @brief Function to check if the device is an Intel® Data Streaming Accelerator (Intel® DSA) device.
 *
 * @param src_ptr Name of current device
 * @param name Intel DSA device name (in lower case)
 * @param name_size Last symbol index of Intel DSA device name
 * @return bool Indicates if there is a match with the specified name
*/
static inline bool own_search_device_name(const uint8_t* src_ptr, const uint32_t name,
                                          const uint32_t name_size) noexcept {
    const uint8_t null_terminator = '\0';

    for (size_t symbol_idx = 0U; null_terminator != src_ptr[symbol_idx + name_size]; symbol_idx++) {
        const auto* candidate_ptr = reinterpret_cast<const uint32_t*>(src_ptr + symbol_idx);

        // Convert the first 3 bytes to lower case and make the 4th 0xff
        if (name == (*candidate_ptr | DML_TEST_CHAR_MSK)) { return true; }
    }

    return false;
}

namespace dml::test {

auto hw_device::numa_id() const noexcept -> uint64_t {
    return numa_node_id_;
}

auto hw_device::socket_id() const noexcept -> uint64_t {
    return socket_id_;
}

auto hw_device::get_opcfg_enabled() const noexcept -> bool {
    return op_cfg_enabled_;
}

auto hw_device::get_operation_supported_on_wq(const uint32_t wq_idx, const uint32_t operation) const noexcept -> bool {
    return DML_TEST_OC_GET_OP_SUPPORTED(op_configs_[wq_idx], operation);
}

auto hw_device::initialize_new_device(descriptor_t* device_descriptor_ptr) noexcept -> dml_test_hw_accelerator_status {
    // Device initialization stage
    auto*       device_ptr    = reinterpret_cast<accfg_device*>(device_descriptor_ptr);
    const auto* name_ptr      = reinterpret_cast<const uint8_t*>(dml_test_accfg_device_get_devname(device_ptr));
    const bool  is_dsa_device = own_search_device_name(name_ptr, DML_TEST_DSA_DEVICE, accelerator_name_length);

    version_major_ = dml_test_accfg_device_get_version(device_ptr) >> 8U;
    version_minor_ = dml_test_accfg_device_get_version(device_ptr) & 0xFF;

    if (!is_dsa_device) { return DML_TEST_HW_ACCELERATOR_WORK_QUEUES_NOT_AVAILABLE; }
    if (ACCFG_DEVICE_ENABLED != dml_test_accfg_device_get_state(device_ptr)) {
        return DML_TEST_HW_ACCELERATOR_WORK_QUEUES_NOT_AVAILABLE;
    }

    numa_node_id_ = dml_test_accfg_device_get_numa_node(device_ptr);


    // Working queues initialization stage
    auto* wq_ptr = dml_test_accfg_wq_get_first(device_ptr);
    auto  wq_it  = working_queues_.begin();

    while (nullptr != wq_ptr) {
        if (DML_TEST_HW_ACCELERATOR_STATUS_OK == wq_it->initialize_new_queue(wq_ptr)) {
            wq_it++;

            std::push_heap(working_queues_.begin(), wq_it,
                           [](const hw_queue& a, const hw_queue& b) -> bool { return a.priority() < b.priority(); });
        }

        wq_ptr = dml_test_accfg_wq_get_next(wq_ptr);
    }

    // Check number of working queues
    queue_count_ = std::distance(working_queues_.begin(), wq_it);

    if (queue_count_ > 1) {
        auto begin = working_queues_.begin();
        auto end   = begin + queue_count_;

        std::sort_heap(begin, end,
                       [](const hw_queue& a, const hw_queue& b) -> bool { return a.priority() < b.priority(); });
    }

    // Logic for op_cfg_enabled_ value
    op_cfg_enabled_ = working_queues_[0].get_op_configuration_support();

    for (uint32_t wq_idx = 0; wq_idx < queue_count_; wq_idx++) {
        for (uint32_t register_index = 0; register_index < DML_TEST_TOTAL_OP_CFG_BIT_GROUPS; register_index++) {
            op_configs_[wq_idx] = working_queues_[wq_idx].get_op_config_register();
        }
    }

    return DML_TEST_HW_ACCELERATOR_STATUS_OK;
}

auto hw_device::size() const noexcept -> size_t {
    return queue_count_;
}

auto hw_device::begin() const noexcept -> queues_container_t::const_iterator {
    return working_queues_.cbegin();
}

auto hw_device::end() const noexcept -> queues_container_t::const_iterator {
    return working_queues_.cbegin() + queue_count_;
}

} // namespace dml::test

#endif //__linux__
