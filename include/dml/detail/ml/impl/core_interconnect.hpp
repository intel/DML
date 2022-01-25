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
