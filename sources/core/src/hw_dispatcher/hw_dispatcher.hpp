/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_MIDDLE_LAYER_DISPATCHER_HW_DISPATCHER_HPP_
#define DML_MIDDLE_LAYER_DISPATCHER_HW_DISPATCHER_HPP_

#include <array>
#include <cstdint>

#include "dml/dmldefs.h"
#include "hw_device.hpp"

#if defined(linux)
#include "legacy_headers/hardware_configuration_driver.h"
#include "legacy_headers/hardware_definitions.h"
#include "legacy_headers/own_dsa_accel_constants.h"
#endif

namespace dml::core::dispatcher
{

    class hw_dispatcher final
    {
#if defined(linux)

        static constexpr uint32_t max_devices = MAX_DEVICE_COUNT;

        using device_container_t = std::array<hw_device, max_devices>;

        class hw_context final
        {
        public:
            void set_driver_context_ptr(accfg_ctx *driver_context_ptr) noexcept;

            [[nodiscard]] auto get_driver_context_ptr() noexcept -> accfg_ctx *;

        private:
            accfg_ctx *driver_context_ptr_ = nullptr; /**< DSA driver context */
        };

#endif

    public:
        static auto get_instance() noexcept -> hw_dispatcher &;

        [[nodiscard]] auto is_hw_support() const noexcept -> bool;

#if defined(linux)

        [[nodiscard]] auto get_hw_init_status() const noexcept -> dsahw_status_t;

        void fill_hw_context(dsahw_context_t *hw_context_ptr) noexcept;

        [[nodiscard]] auto begin() const noexcept -> device_container_t::const_iterator;

        [[nodiscard]] auto end() const noexcept -> device_container_t::const_iterator;

#endif

        ~hw_dispatcher() noexcept;

        hw_dispatcher() noexcept;

    protected:
#if defined(linux)
        auto initialize_hw() noexcept -> dsahw_status_t;

    private:
        hw_context         hw_context_;
        hw_driver_t        hw_driver_{};
        device_container_t devices_{};
        size_t             device_count_ = 0;
#endif

        bool hw_support_;
#if defined(linux)
        dsahw_status_t hw_init_status_;
#endif
    };

}  // namespace dml::core::dispatcher
#endif  //DML_MIDDLE_LAYER_DISPATCHER_HW_DISPATCHER_HPP_
