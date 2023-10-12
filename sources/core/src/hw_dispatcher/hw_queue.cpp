/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#if defined(__linux__)

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "hw_queue.hpp"
#include "legacy_headers/hardware_configuration_driver.h"
#include "legacy_headers/own_dsa_accel_constants.h"

#define DML_HWSTS_RET(expr, err_code) \
    {                                 \
        if (expr)                     \
        {                             \
            return (err_code);        \
        }                             \
    }

namespace dml::core::dispatcher
{
    hw_queue::hw_queue(hw_queue &&other) noexcept
    {
        version_       = other.version_;
        priority_      = other.priority_;
        portal_mask_   = other.portal_mask_;
        portal_ptr_    = other.portal_ptr_;
        portal_offset_ = 0;

        other.portal_ptr_ = nullptr;
    }

    auto hw_queue::operator=(hw_queue &&other) noexcept -> hw_queue &
    {
        if(&other != this){
            version_       = other.version_;
            priority_      = other.priority_;
            portal_mask_   = other.portal_mask_;
            portal_ptr_    = other.portal_ptr_;
            portal_offset_ = 0;
            
            other.portal_ptr_ = nullptr;
        }

        return *this;
    }

    hw_queue::~hw_queue()
    {
#if defined(__linux__)
        // Freeing resources
        if (portal_ptr_ != nullptr)
        {
            munmap(portal_ptr_, 0x1000u);

            portal_ptr_ = nullptr;
        }
#endif
    }

    void hw_queue::set_portal_ptr(void *value_ptr) noexcept
    {
        portal_offset_ = reinterpret_cast<uintptr_t>(value_ptr) & OWN_PAGE_MASK;
        portal_mask_   = reinterpret_cast<uintptr_t>(value_ptr) & (~OWN_PAGE_MASK);
        portal_ptr_    = value_ptr;
    }

    auto hw_queue::get_portal_ptr() const noexcept -> void *
    {
        uint64_t offset = portal_offset_++;
        offset          = (offset << 6) & OWN_PAGE_MASK;
        return reinterpret_cast<void *>(offset | portal_mask_);
    }

    auto hw_queue::enqueue_descriptor(const dsahw_descriptor_t *desc_ptr) const noexcept -> dsahw_status_t
    {
#if defined(__linux__)
        uint8_t retry = 0u;

        void *current_place_ptr = get_portal_ptr();
        asm volatile("sfence\t\n"
                     ".byte 0xf2, 0x0f, 0x38, 0xf8, 0x02\t\n"
                     "setz %0\t\n"
                     : "=r"(retry)
                     : "a"(current_place_ptr), "d"(desc_ptr));

        return static_cast<dsahw_status_t>(retry);
#else
        return DML_STATUS_WORK_QUEUES_NOT_AVAILABLE;
#endif
    }

    auto hw_queue::initialize_new_queue(void *wq_descriptor_ptr) noexcept -> dsahw_status_t
    {
#if defined(__linux__)
        auto *work_queue_ptr        = reinterpret_cast<accfg_wq *>(wq_descriptor_ptr);
        char path[64];
#ifdef LOG_HW_INIT
        auto work_queue_dev_name    = dsa_work_queue_get_device_name(work_queue_ptr);
#endif

        if (ACCFG_WQ_ENABLED != dsa_work_queue_get_state(work_queue_ptr))
        {
            DIAG("     %7s: DISABLED\n", work_queue_dev_name);
            return DML_STATUS_WORK_QUEUES_NOT_AVAILABLE;
        }

        if (ACCFG_WQ_SHARED != dsa_work_queue_get_mode(work_queue_ptr))
        {
            DIAG("     %7s: UNSUPPORTED\n", work_queue_dev_name);
            return DML_STATUS_WORK_QUEUES_NOT_AVAILABLE;
        }

        DIAG("     %7s:\n", work_queue_dev_name);
        auto status = dsa_work_queue_get_device_path(work_queue_ptr, path, 64 - 1);

        DML_HWSTS_RET((0 > status), DML_STATUS_LIBACCEL_ERROR);

        DIAG("     %7s: opening descriptor %s", work_queue_dev_name, path);
        auto fd = open(path, O_RDWR);
        if(0 > fd)
        {
            DIAGA(", access denied\n");
            return DML_STATUS_LIBACCEL_ERROR;
        }

        // Map portal for enqcmd
        auto *region_ptr = mmap(nullptr, 0x1000u, PROT_WRITE, MAP_SHARED | MAP_POPULATE, fd, 0u);
        close(fd);
        if(MAP_FAILED == region_ptr)
        {
            DIAGA(", limited MSI-X mapping failed\n");
            return DML_STATUS_LIBACCEL_ERROR;
        }
        DIAGA("\n");

        auto *group_ptr = dsa_work_queue_get_group(work_queue_ptr);
        if (group_ptr == nullptr) {
            return DML_STATUS_LIBACCEL_ERROR;
        }

        priority_    = dsa_work_queue_get_priority(work_queue_ptr);
        memory_type_ = dsa_group_get_traffic_class_b(group_ptr) ? supported_memory_type::durable
                                                                : supported_memory_type::non_durable;

#if 0
    DIAG("     %7s: size:        %d\n", work_queue_dev_name, accfg_wq_get_size(work_queue_ptr));
    DIAG("     %7s: threshold:   %d\n", work_queue_dev_name, accfg_wq_get_threshold(work_queue_ptr));
    DIAG("     %7s: priority:    %d\n", work_queue_dev_name, priority_);
    DIAG("     %7s: group:       %d\n", work_queue_dev_name, group_id);
    DIAG("     %7s: memtype:     %d\n", work_queue_dev_name, memory_type_);

    for(struct accfg_engine *engine = accfg_engine_get_first(device_ptr);
            engine != NULL; engine = accfg_engine_get_next(engine))
    {
        if(accfg_engine_get_group_id(engine) == group_id)
            DIAG("            %s\n", accfg_engine_get_devname(engine));
    }
#else
        DIAG("     %7s: priority:    %d\n", work_queue_dev_name, priority_);
        DIAG("     %7s: memtype:     %d\n", work_queue_dev_name, static_cast<int>(memory_type_));
#endif

        hw_queue::set_portal_ptr(region_ptr);

        return DML_STATUS_OK;
#else
        return DML_STATUS_WORK_QUEUES_NOT_AVAILABLE;
#endif
    }

    auto hw_queue::priority() const noexcept -> int32_t
    {
        return priority_;
    }

    auto hw_queue::memory_type() const noexcept -> hw_queue::supported_memory_type
    {
        return memory_type_;
    }

}  // namespace dml::core::dispatcher

#endif
