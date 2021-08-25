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

#ifdef DML_HW

#include <algorithm>

#include "hw_device.hpp"
#include "hardware_configuration_driver.h"
#include "own_dsa_accel_constants.h"

static inline bool own_search_device_name(const char *src_ptr,
                                          const uint32_t name,
                                          const uint32_t name_size) noexcept {
    const uint8_t null_terminator = '\0';

    for (size_t symbol_idx = 0u; null_terminator != src_ptr[symbol_idx + name_size]; symbol_idx++) {
        const auto *candidate_ptr = reinterpret_cast<const uint32_t *>(src_ptr + symbol_idx);

        // Convert the first 3 bytes to lower case and make the 4th 0xff
        if (name == (*candidate_ptr | CHAR_MSK)) {
            return true;
        }
    }

    return false;
}

namespace dml::ml::dispatcher {

void hw_device::fill_hw_context(dsahw_context_t *const hw_context_ptr) const noexcept {
    // Restore device properties
    hw_context_ptr->gen_cap.block_on_fault_support       = hw_device::block_on_fault_support();
    hw_context_ptr->gen_cap.overlapping_copy_support     = hw_device::overlapping_copy_support();
    hw_context_ptr->gen_cap.memory_cache_control_support = hw_device::memory_cache_control_support();
    hw_context_ptr->gen_cap.flush_cache_control_support  = hw_device::flush_cache_control_support();
    hw_context_ptr->gen_cap.interrupt_handle_request     = hw_device::interrupt_handle_request();
    hw_context_ptr->gen_cap.destination_readback_support = hw_device::destination_readback_support();
    hw_context_ptr->gen_cap.descriptor_readback_support  = hw_device::descriptor_readback_support();
    hw_context_ptr->gen_cap.max_transfer_size            = hw_device::max_transfer_size();
    hw_context_ptr->gen_cap.max_batch_size               = hw_device::max_batch_size();
    hw_context_ptr->gen_cap.message_size                 = hw_device::message_size();
    hw_context_ptr->gen_cap.configuration_support        = hw_device::configuration_support();
    hw_context_ptr->gen_cap.max_descriptors              = hw_device::max_descriptors();
}

auto hw_device::enqueue_descriptor(const dsahw_descriptor_t *desc_ptr) const noexcept -> dsahw_status_t {
    const auto n_queues = std::distance(this->begin(), this->end());

    // Initially set to "end" index
    static auto last_wq_idx = std::atomic(n_queues);

    // Loop FROM the queue after the one used for last submit
    for (auto idx = last_wq_idx.load() + 1; idx < n_queues; ++idx)
    {
        auto &queue = *(this->begin() + idx);
        auto status = queue.enqueue_descriptor(desc_ptr);

        if (DML_STATUS_OK == status) {
            last_wq_idx = idx;
            return DML_STATUS_OK;
        }
    }

    // If the loop before didn't submit descriptor, then loop UNTIL the queue that was used for last submit
    for (auto idx = 0; idx <= last_wq_idx; ++idx)
    {
        auto &queue = *(this->begin() + idx);
        auto status = queue.enqueue_descriptor(desc_ptr);

        if (DML_STATUS_OK == status) {
            last_wq_idx = idx;
            return DML_STATUS_OK;
        }
    }

    return DML_STATUS_INSTANCE_NOT_FOUND;
}

auto hw_device::block_on_fault_support() const noexcept -> uint8_t {
    return GC_BLOCK_ON_FAULT_SUP(gen_cap_register_);
}

auto hw_device::overlapping_copy_support() const noexcept -> uint8_t {
    return GC_OVERLAPPING_COPY_SUPPORT(gen_cap_register_);
}

auto hw_device::memory_cache_control_support() const noexcept -> uint8_t {
    return GC_M_CACHE_CONTROL_SUPPORT(gen_cap_register_);
}

auto hw_device::flush_cache_control_support() const noexcept -> uint8_t {
    return GC_F_CACHE_CONTROL_SUPPORT(gen_cap_register_);
}

auto hw_device::interrupt_handle_request() const noexcept -> uint8_t {
    return GC_INTERRUPT_HANDLE_REQUEST(gen_cap_register_);
}

auto hw_device::destination_readback_support() const noexcept -> uint8_t {
    return GC_DESTINATION_READBACK_SUPPORT(gen_cap_register_);
}

auto hw_device::descriptor_readback_support() const noexcept -> uint8_t {
    return GC_DESCRIPTOR_READBACK_SUPPORT(gen_cap_register_);
}

auto hw_device::max_transfer_size() const noexcept -> uint32_t {
    return GC_MAX_TRANSFER_SIZE(gen_cap_register_);
}

auto hw_device::max_batch_size() const noexcept -> uint32_t {
    return GC_MAX_BATCH_SIZE(gen_cap_register_);
}

auto hw_device::message_size() const noexcept -> uint16_t {
    return GC_MESSAGE_SIZE(gen_cap_register_);
}

auto hw_device::configuration_support() const noexcept -> uint8_t {
    return GC_CONFIGURATION_SUPPORT(gen_cap_register_);
}

auto hw_device::max_descriptors() const noexcept -> uint8_t {
    return GC_MAX_DESCRIPTORS(gen_cap_register_);
}

auto hw_device::initialize_new_device(descriptor_t *device_descriptor_ptr) noexcept -> dsahw_status_t {
#if defined(linux)
    // Device initialization stage
    auto       *device_ptr   = reinterpret_cast<accfg_device *>(device_descriptor_ptr);
    const auto *name_ptr     = dsa_device_get_name(device_ptr);
    const bool is_dsa_device = own_search_device_name(name_ptr, DSA_DEVICE_ID, DEVICE_NAME_LENGTH);

    if (!is_dsa_device || ACCFG_DEVICE_DISABLED == dsa_device_get_state(device_ptr)) {
        return DML_STATUS_INSTANCE_NOT_FOUND;
    }

    gen_cap_register_ = dsa_device_get_gen_cap_register(device_ptr);
    version_          = dsa_device_get_major_version(device_ptr);
    numa_node_id_     = dsa_device_get_numa_node(device_ptr);

    dsa_group_get_first(device_ptr);

    // Working queues initialization stage
    auto *wq_ptr = dsa_get_first_work_queue(device_ptr);
    auto wq_it   = working_queues_.begin();

    while (nullptr != wq_ptr) {
        if (DML_STATUS_OK == wq_it->initialize_new_queue(wq_ptr, version_)) {
            wq_it++;

            std::push_heap(working_queues_.begin(), wq_it,
                           [](const hw_queue &a, const hw_queue &b) -> bool {
                               return a.priority() < b.priority();
                           });
        }

        wq_ptr = dsa_work_queue_get_next(wq_ptr);
    }

    // Check number of working queues
    queue_count_ = std::distance(working_queues_.begin(), wq_it);

    if (queue_count_ > 1) {
        auto begin = working_queues_.begin();
        auto end   = begin + queue_count_;

        std::sort_heap(begin, end, [](const hw_queue &a, const hw_queue &b) -> bool {
            return a.priority() < b.priority();
        });
    }

    if (queue_count_ == 0) {
        return DML_STATUS_WORK_QUEUES_NOT_AVAILABLE;
    }

    return DML_STATUS_OK;
#else
    return DML_STATUS_INSTANCE_NOT_FOUND;
#endif
}

auto hw_device::size() const noexcept -> size_t {
    return queue_count_;
}

auto hw_device::numa_id() const noexcept -> uint64_t {
    return numa_node_id_;
}

auto hw_device::begin() const noexcept -> queues_container_t::const_iterator {
    return working_queues_.cbegin();
}

auto hw_device::end() const noexcept -> queues_container_t::const_iterator {
    return working_queues_.cbegin() + queue_count_;
}

}

#endif
