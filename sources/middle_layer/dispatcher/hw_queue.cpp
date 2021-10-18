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

#include <fcntl.h>

#if defined( linux )

#include <sys/mman.h>

#endif

#include "hw_queue.hpp"
#include "hardware_configuration_driver.h"
#include "own_dsa_accel_constants.h"

#define DML_HWSTS_RET(expr, err_code) { if( expr ) { return( err_code ); }}
#define DEC_BASE 10u         /**< @todo */
#define DEC_CHAR_BASE ('0')  /**< @todo */
#define DEC_MAX_INT_BUF 16u  /**< @todo */

namespace dml::ml::dispatcher {

static inline uint32_t own_int_to_str(char *path_ptr, uint32_t i, uint32_t max_path, uint32_t number) noexcept {
    uint8_t  ch[DEC_MAX_INT_BUF];
    uint32_t j = 0u;

    // At least once - in case of 0
    do {
        // Convert digit to char
        ch[j++] = DEC_CHAR_BASE + number % DEC_BASE;
        // Next digit
        number /= DEC_BASE;
    } while (0u < number);
    // Return max+1 if "path" buffer overflow
    DML_HWSTS_RET((max_path < i + j), max_path + 1u);
    do {
        // Copy to "path" in correct order
        path_ptr[i++] = ch[--j];
    } while (0u < j);
    // Success - return the next "free" char index in the "path"
    return i;
}

static inline dsahw_status_t own_specify_path(char *path_ptr,
                                              uint32_t max_path,
                                              uint32_t major,
                                              uint32_t minor) noexcept {
    uint32_t i = 0u;

    // Strlen analogue
    while (('\0' != path_ptr[i]) && (i < max_path)) {
        i++;
    }
    // i has index of terminating 0
    // Check for buffer overflow
    DML_HWSTS_RET((max_path < i), DML_STATUS_VERSION_DETECTION_ERROR);
    // Need the next format: "/dev/char/major:minor"
    i = own_int_to_str(path_ptr, i, max_path, major);
    // Check for buffer overflow
    DML_HWSTS_RET((max_path < i + 1u), DML_STATUS_VERSION_DETECTION_ERROR);
    path_ptr[i++] = ':';
    i = own_int_to_str(path_ptr, i, max_path, minor);
    // Check for buffer overflow
    DML_HWSTS_RET((max_path < i), DML_STATUS_VERSION_DETECTION_ERROR);
    path_ptr[i] = '\0';
    return DML_STATUS_OK;
}

hw_queue::hw_queue(hw_queue &&other) noexcept {
    version_       = other.version_;
    priority_      = other.priority_;
    portal_mask_   = other.portal_mask_;
    portal_ptr_    = other.portal_ptr_;
    portal_offset_ = 0;

    other.portal_ptr_ = nullptr;
}

auto hw_queue::operator=(hw_queue &&other) noexcept -> hw_queue & {
    version_       = other.version_;
    priority_      = other.priority_;
    portal_mask_   = other.portal_mask_;
    portal_ptr_    = other.portal_ptr_;
    portal_offset_ = 0;

    other.portal_ptr_ = nullptr;

    return *this;
}

hw_queue::~hw_queue() {
#if defined( linux )
    // Freeing resources
    if (portal_ptr_ != nullptr) {
        munmap(portal_ptr_, 0x1000u);

        portal_ptr_ = nullptr;
    }
#endif
}

void hw_queue::set_portal_ptr(void *value_ptr) noexcept {
    portal_offset_ = reinterpret_cast<uintptr_t>(value_ptr) & OWN_PAGE_MASK;
    portal_mask_   = reinterpret_cast<uintptr_t>(value_ptr) & (~OWN_PAGE_MASK);
    portal_ptr_    = value_ptr;
}

auto hw_queue::get_portal_ptr() const noexcept -> void * {
    uint64_t offset = portal_offset_++;
    offset = (offset << 6) & OWN_PAGE_MASK;
    return reinterpret_cast<void *>(offset | portal_mask_);
}

auto hw_queue::enqueue_descriptor(const dsahw_descriptor_t *desc_ptr) const noexcept -> dsahw_status_t {
#if defined( linux )
    uint8_t retry = 0u;

    void *current_place_ptr = get_portal_ptr();
    asm volatile("sfence\t\n"
                 ".byte 0xf2, 0x0f, 0x38, 0xf8, 0x02\t\n"
                 "setz %0\t\n"
    : "=r"(retry) : "a" (current_place_ptr), "d" (desc_ptr));

    return static_cast<dsahw_status_t>(retry);
#else
    return DML_STATUS_INSTANCE_NOT_FOUND;
#endif
}

auto hw_queue::initialize_new_queue(void *wq_descriptor_ptr, uint32_t major_version) noexcept -> dsahw_status_t {
#if defined( linux )
    auto *work_queue_ptr = reinterpret_cast<accfg_wq *>(wq_descriptor_ptr);
    char path[64]        = "/dev/char/";

    if (ACCFG_WQ_ENABLED != dsa_work_queue_get_state(work_queue_ptr) ||
        ACCFG_WQ_SHARED != dsa_work_queue_get_mode(work_queue_ptr)) {
        return DML_STATUS_WORK_QUEUES_NOT_AVAILABLE;
    }

    auto *group_ptr = dsa_work_queue_get_group(work_queue_ptr);
    if (group_ptr == nullptr) {
        return DML_STATUS_INTERNAL_ERROR;
    }

    version_     = dsa_work_queue_get_minor_version(work_queue_ptr);
    priority_    = dsa_work_queue_get_priority(work_queue_ptr);
    memory_type_ = dsa_group_get_traffic_class_b(group_ptr) ? supported_memory_type::durable
                                                            : supported_memory_type::non_durable;

    // Need the next format: "/dev/char/major:minor"
#if defined(LIB_ACCEL_VERSION_3_2)
    auto status = dsa_work_queue_get_device_path(work_queue_ptr, path, 64 - 1);
#else
    auto status = own_specify_path(path, sizeof(path) - 1u, major_version, version_);
#endif
    DML_HWSTS_RET((0 > status), DML_STATUS_INCORRECT_WORK_QUEUE_ID);

    auto fd = open(path, O_RDWR);
    DML_HWSTS_RET((0 > fd), DML_STATUS_WORK_QUEUE_CONNECTION_ERROR);

    // Map portal for enqcmd
    auto *region_ptr = mmap(nullptr, 0x1000u, PROT_WRITE, MAP_SHARED | MAP_POPULATE, fd, 0u);
    close(fd);

    DML_HWSTS_RET((MAP_FAILED == region_ptr), DML_STATUS_PORTAL_CREATION_ERROR);

    hw_queue::set_portal_ptr(region_ptr);

    return DML_STATUS_OK;
#else
    return DML_STATUS_WORK_QUEUE_CONNECTION_ERROR;
#endif
}

auto hw_queue::priority() const noexcept -> int32_t {
    return priority_;
}

auto hw_queue::memory_type() const noexcept -> hw_queue::supported_memory_type {
    return memory_type_;
}

}

#endif
