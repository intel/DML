/*******************************************************************************
 * Copyright (C) 2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TOOLS_UTILS_HW_DISPATCHER_TEST_HW_DISPATCHER_HPP_
#define DML_TOOLS_UTILS_HW_DISPATCHER_TEST_HW_DISPATCHER_HPP_

#include <array>
#include <stddef.h>

#include "test_hw_device.hpp"
#include "test_hw_status.h"

#if defined(__linux__)
#include "test_hw_configuration_driver.h"
#endif //__linux__

namespace dml::test {
class hw_dispatcher final {

#if defined(__linux__)

    static constexpr uint32_t max_devices = DML_TEST_MAX_NUM_DEV;

    using device_container_t = std::array<hw_device, max_devices>;

    class hw_context final {
    public:
        void set_driver_context_ptr(accfg_ctx* driver_context_ptr) noexcept;

        [[nodiscard]] auto get_driver_context_ptr() noexcept -> accfg_ctx*;

    private:
        accfg_ctx* driver_context_ptr_ = nullptr; /**< DML driver context */
    };

#endif //__linux__

public:
    static auto get_instance() noexcept -> hw_dispatcher&;

    [[nodiscard]] auto is_hw_support() const noexcept -> bool;

    [[nodiscard]] auto get_hw_init_status() const noexcept -> dml_test_hw_accelerator_status;

#if defined(__linux__)

    [[nodiscard]] auto begin() const noexcept -> device_container_t::const_iterator;

    [[nodiscard]] auto end() const noexcept -> device_container_t::const_iterator;

    [[nodiscard]] auto device(unsigned int idx) const noexcept -> const hw_device&;

    [[nodiscard]] auto device_count() const noexcept -> size_t;

#endif //__linux__

    virtual ~hw_dispatcher() noexcept;

protected:
    hw_dispatcher() noexcept;

    auto initialize_hw() noexcept -> dml_test_hw_accelerator_status;

private:
#if defined(__linux__)
    hw_context         hw_context_;
    device_container_t devices_ {};
    size_t             device_count_ = 0;
    dml_test_hw_driver_t hw_driver_ {};
#endif //__linux__

    bool                           hw_support_;
    dml_test_hw_accelerator_status hw_init_status_ = DML_TEST_HW_ACCELERATOR_STATUS_OK;
};

} // namespace dml::test
#endif //DML_TOOLS_UTILS_HW_DISPATCHER_TEST_HW_DISPATCHER_HPP_
