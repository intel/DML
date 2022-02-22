/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_ML_IMPL_CORE_PATH
#define DML_ML_IMPL_CORE_PATH

#include <dml/detail/common/status.hpp>
#include <dml/detail/common/types.hpp>

namespace dml::detail::ml::impl
{
    void rebind(descriptor& dsc, completion_record& record) noexcept;

    struct software
    {
        [[nodiscard]] static validation_status validate(const descriptor& dsc) noexcept;

        [[nodiscard]] static submission_status submit(const descriptor& dsc, std::uint32_t numa_id) noexcept;

        static void wait(const descriptor& dsc) noexcept;

        [[nodiscard]] static bool finished(const descriptor& dsc) noexcept;
    };

    struct hardware
    {
        [[nodiscard]] static validation_status validate(const descriptor& dsc) noexcept;

        [[nodiscard]] static submission_status submit(const descriptor& dsc, std::uint32_t numa_id) noexcept;

        static void wait(const descriptor& dsc) noexcept;

        [[nodiscard]] static bool finished(const descriptor& dsc) noexcept;
    };

    struct automatic
    {
        [[nodiscard]] static validation_status validate(const descriptor& dsc) noexcept;

        [[nodiscard]] static submission_status submit(const descriptor& dsc, std::uint32_t numa_id) noexcept;

        static void wait(descriptor& dsc) noexcept;

        [[nodiscard]] static bool finished(descriptor& dsc) noexcept;
    };
}  // namespace dml::detail::ml::execution_path

#endif  //DML_ML_IMPL_CORE_PATH
