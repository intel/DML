/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <core/completion_record_views.hpp>
#include <core/descriptor_views.hpp>
#include <core/operations.hpp>
#include <dml/detail/common/status.hpp>

#include "core/device.hpp"
#include "kernels.hpp"

namespace dml::core
{
    dml::detail::submission_status software_device::submit(const descriptor& dsc) noexcept
    {
        auto  view   = any_descriptor(dsc);
        auto  op     = operation(view.operation());

        switch (op)
        {
            case operation::nop:
                kernels::nop(make_view<operation::nop>(dsc));
                break;
            case operation::batch:
                kernels::batch(make_view<operation::batch>(dsc));
                break;
            case operation::drain:
                kernels::drain(make_view<operation::drain>(dsc));
                break;
            case operation::mem_move:
                kernels::mem_move(make_view<operation::mem_move>(dsc));
                break;
            case operation::fill:
                kernels::fill(make_view<operation::fill>(dsc));
                break;
            case operation::compare:
                kernels::compare(make_view<operation::compare>(dsc));
                break;
            case operation::compare_pattern:
                kernels::compare_pattern(make_view<operation::compare_pattern>(dsc));
                break;
            case operation::create_delta:
                kernels::create_delta(make_view<operation::create_delta>(dsc));
                break;
            case operation::apply_delta:
                kernels::apply_delta(make_view<operation::apply_delta>(dsc));
                break;
            case operation::dualcast:
                kernels::dualcast(make_view<operation::dualcast>(dsc));
                break;
            case operation::crc:
                kernels::crc(make_view<operation::crc>(dsc));
                break;
            case operation::copy_crc:
                kernels::copy_crc(make_view<operation::copy_crc>(dsc));
                break;
            case operation::dif_check:
                kernels::dif_check(make_view<operation::dif_check>(dsc));
                break;
            case operation::dif_insert:
                kernels::dif_insert(make_view<operation::dif_insert>(dsc));
                break;
            case operation::dif_strip:
                kernels::dif_strip(make_view<operation::dif_strip>(dsc));
                break;
            case operation::dif_update:
                kernels::dif_update(make_view<operation::dif_update>(dsc));
                break;
            case operation::cache_flush:
                kernels::cache_flush(make_view<operation::cache_flush>(dsc));
                break;
            default:
                return dml::detail::submission_status::failure;
        }

        return dml::detail::submission_status::success;
    }
}  // namespace dml::core
