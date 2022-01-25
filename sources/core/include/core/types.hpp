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

#ifndef DML_CORE_TYPES_HPP
#define DML_CORE_TYPES_HPP

#include <dml/detail/common/types.hpp>

namespace dml::core
{
    using byte_t = dml::detail::byte_t;

    using descriptor = detail::descriptor;

    using completion_record = detail::completion_record;

    using status_t = dml::detail::status_t;

    using transfer_size_t = dml::detail::transfer_size_t;

    using operation_t = dml::detail::operation_t;

    using flags_t = dml::detail::flags_t;

    using operation_specific_flags_t = dml::detail::operation_specific_flags_t;

    using completion_interrupt_handle_t = detail::completion_interrupt_handle_t;

    using transfer_size_t = dml::detail::transfer_size_t;

    using address_t = detail::address_t;

    using pattern_t = dml::detail::pattern_t;

    using result_t = dml::detail::result_t;

    using crc_value_t = dml::detail::crc_value_t;

    using dif_flags_t = dml::detail::dif_flags_t;

    using dif_status_t = dml::detail::dif_status_t;

    using dif_ref_tag_t = dml::detail::dif_ref_tag_t;

    using dif_app_tag_t = dml::detail::dif_app_tag_t;
}  // namespace dml::core

#endif  //DML_CORE_TYPES_HPP
