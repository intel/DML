/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <core/completion_record_views.hpp>
#include <core/descriptor_views.hpp>
#include <core/operations.hpp>
#include <core/utils.hpp>
#include <dml/detail/common/status.hpp>
#include <dml/detail/common/utils/enum.hpp>

#include "immintrin.h"
#include "kernels.hpp"

namespace dml::core::kernels
{
    void batch(const_view<descriptor, operation::batch> dsc) noexcept
    {
        auto record = make_view<operation::batch>(get_completion_record(dsc));

        const auto operations        = reinterpret_cast<descriptor *>(dsc.descriptor_list_address());
        const auto descriptors_count = dsc.descriptors_count();

        auto status = dml::detail::execution_status::processing;
        auto index  = size_t(0);

        while (index < descriptors_count && status == dml::detail::execution_status::processing)
        {
            const auto &current_dsc    = operations[index];
            auto       &current_record = *reinterpret_cast<completion_record *>(any_descriptor(current_dsc).completion_record_address());

            auto op = operation(any_descriptor(current_dsc).operation());

            switch (op)
            {
                case operation::nop:
                    kernels::nop(make_view<operation::nop>(current_dsc));
                    break;
                case operation::mem_move:
                    kernels::mem_move(make_view<operation::mem_move>(current_dsc));
                    break;
                case operation::fill:
                    kernels::fill(make_view<operation::fill>(current_dsc));
                    break;
                case operation::compare:
                    kernels::compare(make_view<operation::compare>(current_dsc));
                    break;
                case operation::compare_pattern:
                    kernels::compare_pattern(make_view<operation::compare_pattern>(current_dsc));
                    break;
                case operation::create_delta:
                    kernels::create_delta(make_view<operation::create_delta>(current_dsc));
                    break;
                case operation::apply_delta:
                    kernels::apply_delta(make_view<operation::apply_delta>(current_dsc));
                    break;
                case operation::dualcast:
                    kernels::dualcast(make_view<operation::dualcast>(current_dsc));
                    break;
                case operation::crc:
                    kernels::crc(make_view<operation::crc>(current_dsc));
                    break;
                case operation::copy_crc:
                    kernels::copy_crc(make_view<operation::copy_crc>(current_dsc));
                    break;
                case operation::dif_check:
                    kernels::dif_check(make_view<operation::dif_check>(current_dsc));
                    break;
                case operation::dif_insert:
                    kernels::dif_insert(make_view<operation::dif_insert>(current_dsc));
                    break;
                case operation::dif_strip:
                    kernels::dif_strip(make_view<operation::dif_strip>(current_dsc));
                    break;
                case operation::dif_update:
                    kernels::dif_update(make_view<operation::dif_update>(current_dsc));
                    break;
                case operation::cache_flush:
                    kernels::cache_flush(make_view<operation::cache_flush>(current_dsc));
                    break;
                default:
                    status = dml::detail::execution_status::batch_error;
            }

            if (any_completion_record(current_record).status() != to_underlying(dml::detail::execution_status::success))
            {
                status = dml::detail::execution_status::batch_error;
            }
            else
            {
                ++index;
            }
        }

        if (index == descriptors_count)
        {
            status = dml::detail::execution_status::success;
        }

        record.descriptors_completed() = static_cast<transfer_size_t>(index);

        _mm_mfence();
        record.status() = to_underlying(status);
    }
}  // namespace dml::core::kernels
