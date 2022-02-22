/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_MIDDLE_LAYER_DISPATCHER_HW_QUEUE_HPP_
#define DML_MIDDLE_LAYER_DISPATCHER_HW_QUEUE_HPP_

#include <atomic>

#include "dml/dmldefs.h"

#if defined(linux)

#include "legacy_headers/hardware_definitions.h"

namespace dml::core::dispatcher
{

    class hw_queue
    {
    public:
        enum class supported_memory_type
        {
            durable,
            non_durable
        };

        using descriptor_t = void;

        hw_queue() noexcept = default;

        hw_queue(const hw_queue &) noexcept = delete;

        auto operator=(const hw_queue &other) noexcept -> hw_queue & = delete;

        hw_queue(hw_queue &&other) noexcept;

        auto operator=(hw_queue &&other) noexcept -> hw_queue &;

        auto initialize_new_queue(descriptor_t *wq_descriptor_ptr) noexcept -> dsahw_status_t;

        [[nodiscard]] auto get_portal_ptr() const noexcept -> void *;

        [[nodiscard]] auto enqueue_descriptor(const dsahw_descriptor_t *desc_ptr) const noexcept -> dsahw_status_t;

        [[nodiscard]] auto priority() const noexcept -> int32_t;

        [[nodiscard]] auto memory_type() const noexcept -> supported_memory_type;

        void set_portal_ptr(void *portal_ptr) noexcept;

        ~hw_queue() noexcept;

    private:
        uint32_t                       version_       = 0u;
        int32_t                        priority_      = 0u;
        supported_memory_type          memory_type_   = supported_memory_type::non_durable;
        uint64_t                       portal_mask_   = 0u; /**< Mask for incrementing portals */
        mutable void                  *portal_ptr_    = nullptr;
        mutable std::atomic<uintptr_t> portal_offset_ = 0u; /**< Portal for enqcmd (mod page size)*/
    };

}  // namespace dml::core::dispatcher
#endif

#endif  //DML_MIDDLE_LAYER_DISPATCHER_HW_QUEUE_HPP_
