/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_ML_OPTIONS_HPP
#define DML_ML_OPTIONS_HPP

#include <dml/detail/common/flags.hpp>
#include <dml/detail/common/specific_flags.hpp>
#include <dml/detail/common/types.hpp>

namespace dml::detail::ml
{
    template <typename options_t>
    class options final
    {
        using value_t = std::underlying_type_t<options_t>;

    public:
        constexpr options() noexcept = default;

        template <options_t option>
        [[nodiscard]] constexpr auto enable() const noexcept
        {
            return options(bit_mask_ | to_underlying(option));
        }

        constexpr explicit operator value_t() const noexcept
        {
            return bit_mask_;
        }

        // TODO: Should be private, but job API batch implementation is clunky

    public:
        constexpr explicit options(const value_t bit_mask) noexcept: bit_mask_(bit_mask)
        {
        }

    private:
        value_t bit_mask_{};
    };

    using nop_options = options<nop_flag>;

    using batch_options = options<batch_flag>;

    using drain_options = options<drain_flag>;

    using mem_move_options = options<mem_move_flag>;

    using fill_options = options<fill_flag>;

    using compare_options = options<compare_flag>;

    using compare_pattern_options = options<compare_pattern_flag>;

    using create_delta_options = options<create_delta_flag>;

    using apply_delta_options = options<apply_delta_flag>;

    using dualcast_options = options<dualcast_flag>;

    using crc_options = options<crc_flag>;

    using copy_crc_options = options<copy_crc_flag>;

    using dif_check_options = options<dif_check_flag>;

    using dif_insert_options = options<dif_insert_flag>;

    using dif_strip_options = options<dif_strip_flag>;

    using dif_update_options = options<dif_update_flag>;

    using cache_flush_options = options<cache_flush_flag>;

    using drain_specific_options = options<drain_specific_flag>;

    using dualcast_specific_options = options<dualcast_specific_flag>;

    using crc_specific_options = options<crc_specific_flag>;

    using copy_crc_specific_options = options<crc_specific_flag>;

    using dif_specific_options = options<dif_specific_flag>;

    using dif_source_options = options<dif_source_flag>;

    using dif_destination_options = options<dif_destination_flag>;
}  // namespace dml::detail::ml

#endif  //DML_ML_OPTIONS_HPP
