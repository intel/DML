/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include "hw_dispatcher.hpp"

#if defined(linux)

#include "legacy_headers/libaccel_config.h"

#endif

// TODO should be removed at all
#define DML_HWSTS_RET(expr, err_code) \
    {                                 \
        if (expr)                     \
        {                             \
            return (err_code);        \
        }                             \
    }

namespace dml::core::dispatcher
{
    static hw_dispatcher instance{};

    hw_dispatcher::hw_dispatcher() noexcept
    {
#if defined(linux)
        hw_init_status_ = hw_dispatcher::initialize_hw();
        hw_support_     = hw_init_status_ == DML_STATUS_OK;
#else
        hw_support_ = false;
#endif
    }

#if defined(linux)

    auto hw_dispatcher::initialize_hw() noexcept -> dsahw_status_t
    {
        accfg_ctx *ctx_ptr = nullptr;

        DIAG("DML version %s\n", "TODO");
        DIAG("Struct size: %lu B\n", sizeof(device_container_t));

        dsahw_status_t status = dsa_initialize_accelerator_driver(&hw_driver_);
        DML_HWSTS_RET(status != DML_STATUS_OK, status);

        DIAG("creating context\n");
        int32_t context_creation_status = dsa_driver_new_context(&ctx_ptr);
        DML_HWSTS_RET(0u != context_creation_status, DML_STATUS_LIBACCEL_ERROR);

        // Retrieve first device in the system given the passed in context
        DIAG("enumerating devices\n");
        auto *dev_tmp_ptr = dsa_context_get_first_device(ctx_ptr);
        auto  device_it   = devices_.begin();

        while (nullptr != dev_tmp_ptr)
        {
            if (DML_STATUS_OK == device_it->initialize_new_device(dev_tmp_ptr))
            {
                device_it++;
            }

            // Retrieve the "next" device in the system based on given device
            dev_tmp_ptr = dsa_device_get_next(dev_tmp_ptr);
        }

        device_count_ = std::distance(devices_.begin(), device_it);

        if (device_count_ <= 0)
        {
            return DML_STATUS_WORK_QUEUES_NOT_AVAILABLE;
        }

        hw_context_.set_driver_context_ptr(ctx_ptr);

        return DML_STATUS_OK;
    }
#endif

    hw_dispatcher::~hw_dispatcher() noexcept
    {
#if defined(linux)
        // Variables
        auto *context_ptr = hw_context_.get_driver_context_ptr();

        if (context_ptr != nullptr)
        {
            dsa_context_close(context_ptr);
        }

        dsa_finalize_accelerator_driver(&hw_driver_);

        // Zeroing values
        hw_context_.set_driver_context_ptr(nullptr);
#endif
    }

    auto hw_dispatcher::get_instance() noexcept -> hw_dispatcher &
    {
        return instance;
    }

    auto hw_dispatcher::is_hw_support() const noexcept -> bool
    {
        return hw_support_;
    }

#if defined(linux)

    void hw_dispatcher::fill_hw_context(dsahw_context_t *const hw_context_ptr) noexcept
    {
#if defined(linux)
        // Restore context
        hw_context_ptr->dsa_context_ptr = hw_context_.get_driver_context_ptr();

        // Restore device properties
        // We take the first one as all configurations across the platform should be the same for all devices
        devices_[0].fill_hw_context(hw_context_ptr);
#endif
    }

    auto hw_dispatcher::get_hw_init_status() const noexcept -> dsahw_status_t
    {
        return hw_init_status_;
    }

#if defined(linux)

    auto hw_dispatcher::begin() const noexcept -> device_container_t::const_iterator
    {
        return devices_.cbegin();
    }

    auto hw_dispatcher::end() const noexcept -> device_container_t::const_iterator
    {
        return devices_.cbegin() + device_count_;
    }

    void hw_dispatcher::hw_context::set_driver_context_ptr(accfg_ctx *driver_context_ptr) noexcept
    {
        driver_context_ptr_ = driver_context_ptr;
    }

    [[nodiscard]] auto hw_dispatcher::hw_context::get_driver_context_ptr() noexcept -> accfg_ctx *
    {
        return driver_context_ptr_;
    }

#endif

#endif
}  // namespace dml::core::dispatcher
