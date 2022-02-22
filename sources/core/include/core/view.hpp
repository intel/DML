/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_CORE_VIEW_HPP
#define DML_CORE_VIEW_HPP

#include <core/descriptor_views.hpp>
#include <core/completion_record_views.hpp>
#include <core/operations.hpp>

namespace dml::core
{
    namespace detail
    {
        template <typename type, operation op>
        constexpr inline auto view_deducer() noexcept
        {
            if constexpr (std::is_same_v<std::decay_t<type>, descriptor>)
            {
                type value{};

                if constexpr (op == operation::nop)
                {
                    return nop_descriptor<type>(value);
                }
                else if constexpr (op == operation::batch)
                {
                    return batch_descriptor<type>(value);
                }
                else if constexpr (op == operation::drain)
                {
                    return drain_descriptor<type>(value);
                }
                else if constexpr (op == operation::mem_move)
                {
                    return mem_move_descriptor<type>(value);
                }
                else if constexpr (op == operation::fill)
                {
                    return fill_descriptor<type>(value);
                }
                else if constexpr (op == operation::compare)
                {
                    return compare_descriptor<type>(value);
                }
                else if constexpr (op == operation::compare_pattern)
                {
                    return compare_pattern_descriptor<type>(value);
                }
                else if constexpr (op == operation::create_delta)
                {
                    return create_delta_descriptor<type>(value);
                }
                else if constexpr (op == operation::apply_delta)
                {
                    return apply_delta_descriptor<type>(value);
                }
                else if constexpr (op == operation::dualcast)
                {
                    return dualcast_descriptor<type>(value);
                }
                else if constexpr (op == operation::crc)
                {
                    return crc_descriptor<type>(value);
                }
                else if constexpr (op == operation::copy_crc)
                {
                    return copy_crc_descriptor<type>(value);
                }
                else if constexpr (op == operation::dif_check)
                {
                    return dif_check_descriptor<type>(value);
                }
                else if constexpr (op == operation::dif_insert)
                {
                    return dif_insert_descriptor<type>(value);
                }
                else if constexpr (op == operation::dif_strip)
                {
                    return dif_strip_descriptor<type>(value);
                }
                else if constexpr (op == operation::dif_update)
                {
                    return dif_update_descriptor<type>(value);
                }
                else if constexpr (op == operation::cache_flush)
                {
                    return cache_flush_descriptor<type>(value);
                }
                else
                {
                    return; // void
                }
            }
            else if constexpr (std::is_same_v<std::decay_t<type>, completion_record>)
            {
                type value{};

                if constexpr (op == operation::nop)
                {
                    return nop_completion_record<type>(value);
                }
                else if constexpr (op == operation::batch)
                {
                    return batch_completion_record<type>(value);
                }
                else if constexpr (op == operation::drain)
                {
                    return drain_completion_record<type>(value);
                }
                else if constexpr (op == operation::mem_move)
                {
                    return mem_move_completion_record<type>(value);
                }
                else if constexpr (op == operation::fill)
                {
                    return fill_completion_record<type>(value);
                }
                else if constexpr (op == operation::compare)
                {
                    return compare_completion_record<type>(value);
                }
                else if constexpr (op == operation::compare_pattern)
                {
                    return compare_pattern_completion_record<type>(value);
                }
                else if constexpr (op == operation::create_delta)
                {
                    return create_delta_completion_record<type>(value);
                }
                else if constexpr (op == operation::apply_delta)
                {
                    return apply_delta_completion_record<type>(value);
                }
                else if constexpr (op == operation::dualcast)
                {
                    return dualcast_completion_record<type>(value);
                }
                else if constexpr (op == operation::crc)
                {
                    return crc_completion_record<type>(value);
                }
                else if constexpr (op == operation::copy_crc)
                {
                    return crc_completion_record<type>(value);
                }
                else if constexpr (op == operation::dif_check)
                {
                    return dif_check_completion_record<type>(value);
                }
                else if constexpr (op == operation::dif_insert)
                {
                    return dif_insert_completion_record<type>(value);
                }
                else if constexpr (op == operation::dif_strip)
                {
                    return dif_strip_completion_record<type>(value);
                }
                else if constexpr (op == operation::dif_update)
                {
                    return dif_update_completion_record<type>(value);
                }
                else if constexpr (op == operation::cache_flush)
                {
                    return cache_flush_completion_record<type>(value);
                }
                else
                {
                    return; // void
                }
            }
            else
            {
                return; // void
            }
        }
    }

    template <typename type, operation op>
    using view = decltype(detail::view_deducer<type, op>());

    template <typename type, operation op>
    using const_view = view<const type, op>;

    template <operation op, typename type>
    [[nodiscard]] inline auto make_view(type& viewable) noexcept
    {
        return view<type, op>(viewable);
    }
}  // namespace dml::core

#endif  //DML_CORE_VIEW_HPP
