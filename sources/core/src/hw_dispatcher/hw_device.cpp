/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#if defined(linux)

#include "hw_device.hpp"

#include <algorithm>

#include "legacy_headers/hardware_configuration_driver.h"
#include "legacy_headers/own_dsa_accel_constants.h"

static inline bool own_search_device_name(const char *src_ptr, const uint32_t name, const uint32_t name_size) noexcept
{
    const uint8_t null_terminator = '\0';

    for (size_t symbol_idx = 0u; null_terminator != src_ptr[symbol_idx + name_size]; symbol_idx++)
    {
        const auto *candidate_ptr = reinterpret_cast<const uint32_t *>(src_ptr + symbol_idx);

        // Convert the first 3 bytes to lower case and make the 4th 0xff
        if (name == (*candidate_ptr | CHAR_MSK))
        {
            return true;
        }
    }

    return false;
}

namespace dml::core::dispatcher
{

    void hw_device::fill_hw_context(dsahw_context_t *const hw_context_ptr) const noexcept
    {
        // Restore device properties
        hw_context_ptr->gen_cap.block_on_fault_support       = hw_device::block_on_fault_support();
        hw_context_ptr->gen_cap.overlapping_copy_support     = hw_device::overlapping_copy_support();
        hw_context_ptr->gen_cap.memory_cache_control_support = hw_device::memory_cache_control_support();
        hw_context_ptr->gen_cap.flush_cache_control_support  = hw_device::flush_cache_control_support();
        hw_context_ptr->gen_cap.destination_readback_support = hw_device::destination_readback_support();
        hw_context_ptr->gen_cap.descriptor_readback_support  = hw_device::descriptor_readback_support();
        hw_context_ptr->gen_cap.max_transfer_size            = hw_device::max_transfer_size();
        hw_context_ptr->gen_cap.max_batch_size               = hw_device::max_batch_size();
        hw_context_ptr->gen_cap.message_size                 = hw_device::message_size();
        hw_context_ptr->gen_cap.configuration_support        = hw_device::configuration_support();
    }

    auto hw_device::enqueue_descriptor(const dsahw_descriptor_t *desc_ptr) const noexcept -> dsahw_status_t
    {
        const auto n_queues = std::distance(this->begin(), this->end());

        // Initially set to "end" index
        static thread_local auto last_wq_idx = std::atomic(n_queues);

        // Loop FROM the queue after the one used for last submit
        for (auto idx = last_wq_idx.load() + 1; idx < n_queues; ++idx)
        {
            auto &queue  = *(this->begin() + idx);
            auto  status = queue.enqueue_descriptor(desc_ptr);

            if (DML_STATUS_OK == status)
            {
                last_wq_idx = idx;
                return DML_STATUS_OK;
            }
        }

        // If the loop before didn't submit descriptor, then loop UNTIL the queue that was used for last submit
        for (auto idx = 0; idx <= last_wq_idx; ++idx)
        {
            auto &queue  = *(this->begin() + idx);
            auto  status = queue.enqueue_descriptor(desc_ptr);

            if (DML_STATUS_OK == status)
            {
                last_wq_idx = idx;
                return DML_STATUS_OK;
            }
        }

        return DML_STATUS_WORK_QUEUE_OVERFLOW_ERROR;
    }

    auto hw_device::block_on_fault_support() const noexcept -> uint8_t
    {
        return GC_BLOCK_ON_FAULT(gen_cap_register_);
    }

    auto hw_device::overlapping_copy_support() const noexcept -> uint8_t
    {
        return GC_OVERLAPPING(gen_cap_register_);
    }

    auto hw_device::memory_cache_control_support() const noexcept -> uint8_t
    {
        return GC_CACHE_WRITE(gen_cap_register_);
    }

    auto hw_device::flush_cache_control_support() const noexcept -> uint8_t
    {
        return GC_CACHE_FLUSH(gen_cap_register_);
    }

    auto hw_device::destination_readback_support() const noexcept -> uint8_t
    {
        return GC_DST_READBACK(gen_cap_register_);
    }

    auto hw_device::descriptor_readback_support() const noexcept -> uint8_t
    {
        return GC_DRAIN_READBACK(gen_cap_register_);
    }

    auto hw_device::max_transfer_size() const noexcept -> uint32_t
    {
        return GC_MAX_TRANSFER_SIZE(gen_cap_register_);
    }

    auto hw_device::max_batch_size() const noexcept -> uint32_t
    {
        return GC_MAX_BATCH_SIZE(gen_cap_register_);
    }

    auto hw_device::message_size() const noexcept -> uint16_t
    {
        return GC_INTERRUPT_STORAGE(gen_cap_register_);
    }

    auto hw_device::configuration_support() const noexcept -> uint8_t
    {
        return GC_CONF_SUPPORT(gen_cap_register_);
    }

    auto hw_device::initialize_new_device(descriptor_t *device_descriptor_ptr) noexcept -> dsahw_status_t
    {
#if defined(linux)
        // Device initialization stage
        auto       *device_ptr    = reinterpret_cast<accfg_device *>(device_descriptor_ptr);
        const auto *name_ptr      = dsa_device_get_name(device_ptr);
        const bool  is_dsa_device = own_search_device_name(name_ptr, DSA_DEVICE_ID, DEVICE_NAME_LENGTH);

        version_major_ = dsa_device_get_version(device_ptr)>>8u;
        version_minor_ = dsa_device_get_version(device_ptr)&0xFF;

        DIAG("%5s: ", name_ptr);
        if (!is_dsa_device || version_major_ != 1)
        {
            DIAGA("UNSUPPORTED\n");
            return DML_STATUS_WORK_QUEUES_NOT_AVAILABLE;
        }

        if (ACCFG_DEVICE_ENABLED != dsa_device_get_state(device_ptr)) {
            DIAGA("DISABLED\n");
            return DML_STATUS_WORK_QUEUES_NOT_AVAILABLE;
        }
        DIAGA("\n");

        gen_cap_register_ = dsa_device_get_gen_cap_register(device_ptr);
        numa_node_id_     = dsa_device_get_numa_node(device_ptr);

        DIAG("%5s: version: %d.%d\n", name_ptr, version_major_, version_minor_);
        DIAG("%5s: numa:    %lu\n", name_ptr, numa_node_id_);
        DIAG("%5s: GENCAP: 0x%016lX\n", name_ptr, gen_cap_register_);
        DIAG("%5s: GENCAP: block on fault support:                      %d\n",          name_ptr, block_on_fault_support());
        DIAG("%5s: GENCAP: overlapping copy support:                    %d\n",          name_ptr, overlapping_copy_support());
        DIAG("%5s: GENCAP: cache control support (memory):              %d\n",          name_ptr, memory_cache_control_support());
        DIAG("%5s: GENCAP: cache control support (cache flush):         %d\n",          name_ptr, flush_cache_control_support());
        DIAG("%5s: GENCAP: maximum supported transfer size:             %u\n",          name_ptr, max_transfer_size());
        DIAG("%5s: GENCAP: maximum supported batch size:                %u\n",          name_ptr, max_batch_size());

        // Working queues initialization stage
        auto *wq_ptr = dsa_get_first_work_queue(device_ptr);
        auto  wq_it  = working_queues_.begin();

        DIAG("%5s: getting device WQs\n", name_ptr);
        while (nullptr != wq_ptr)
        {
            if (DML_STATUS_OK == wq_it->initialize_new_queue(wq_ptr))
            {
                wq_it++;

                std::push_heap(working_queues_.begin(),
                               wq_it,
                               [](const hw_queue &a, const hw_queue &b) -> bool
                               {
                                   return a.priority() < b.priority();
                               });
            }

            wq_ptr = dsa_work_queue_get_next(wq_ptr);
        }

        // Check number of working queues
        queue_count_ = std::distance(working_queues_.begin(), wq_it);

        if (queue_count_ > 1)
        {
            auto begin = working_queues_.begin();
            auto end   = begin + queue_count_;

            std::sort_heap(begin,
                           end,
                           [](const hw_queue &a, const hw_queue &b) -> bool
                           {
                               return a.priority() < b.priority();
                           });
        }

        if (queue_count_ == 0)
        {
            return DML_STATUS_WORK_QUEUES_NOT_AVAILABLE;
        }

        return DML_STATUS_OK;
#else
        return DML_STATUS_WORK_QUEUES_NOT_AVAILABLE;
#endif
    }

    auto hw_device::size() const noexcept -> size_t
    {
        return queue_count_;
    }

    auto hw_device::numa_id() const noexcept -> uint64_t
    {
        return numa_node_id_;
    }

    auto hw_device::begin() const noexcept -> queues_container_t::const_iterator
    {
        return working_queues_.cbegin();
    }

    auto hw_device::end() const noexcept -> queues_container_t::const_iterator
    {
        return working_queues_.cbegin() + queue_count_;
    }

}  // namespace dml::core::dispatcher

#endif
