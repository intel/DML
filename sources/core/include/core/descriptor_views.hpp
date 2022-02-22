/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_CORE_DESCRIPTOR_VIEW_HPP
#define DML_CORE_DESCRIPTOR_VIEW_HPP

#include <core/types.hpp>
#include <type_traits>

namespace dml::core
{
    template <typename descriptor_t>
    class any_descriptor
    {
        // Requires
        static_assert(std::is_same_v<std::decay_t<descriptor_t>, descriptor>);

    protected:
        // Add const if view is created over constant object
        template <typename type>
        using return_type_t = std::conditional_t<std::is_const_v<descriptor_t>, std::add_const_t<type>, type>;

    private:
        struct offsets
        {
            constexpr static auto flags                       = 4u;
            constexpr static auto op_specific_flags           = 6u;
            constexpr static auto operation                   = 7u;
            constexpr static auto completion_record_address   = 8u;
            constexpr static auto source_address              = 16u;
            constexpr static auto destination_address         = 24u;
            constexpr static auto transfer_size               = 32u;
            constexpr static auto completion_interrupt_handle = 36u;
        };

    public:
        explicit any_descriptor(descriptor_t& dsc): dsc_(dsc)
        {
        }

        [[nodiscard]] auto& operation() noexcept
        {
            return reinterpret_cast<return_type_t<operation_t>&>(dsc_.bytes[offsets::operation]);
        }

        [[nodiscard]] auto& flags() noexcept
        {
            return reinterpret_cast<return_type_t<flags_t>&>(dsc_.bytes[offsets::flags]);
        }

        [[nodiscard]] auto& operation_specific_flags() noexcept
        {
            return reinterpret_cast<return_type_t<operation_specific_flags_t>&>(dsc_.bytes[offsets::op_specific_flags]);
        }

        [[nodiscard]] auto& completion_record_address() noexcept
        {
            return reinterpret_cast<return_type_t<address_t>&>(dsc_.bytes[offsets::completion_record_address]);
        }

        [[nodiscard]] auto& source_address() noexcept
        {
            return reinterpret_cast<return_type_t<address_t>&>(dsc_.bytes[offsets::source_address]);
        }

        [[nodiscard]] auto& destination_address() noexcept
        {
            return reinterpret_cast<return_type_t<address_t>&>(dsc_.bytes[offsets::destination_address]);
        }

        [[nodiscard]] auto& transfer_size() noexcept
        {
            return reinterpret_cast<return_type_t<transfer_size_t>&>(dsc_.bytes[offsets::transfer_size]);
        }

        [[nodiscard]] auto& completion_interrupt_handle() noexcept
        {
            return reinterpret_cast<return_type_t<completion_interrupt_handle_t>&>(dsc_.bytes[offsets::completion_interrupt_handle]);
        }

    protected:
        descriptor_t& dsc_;
    };

    template <typename descriptor_t>
    class nop_descriptor: public any_descriptor<descriptor_t>
    {
        using base = any_descriptor<descriptor_t>;

    public:
        using base::base;

    private:
        using base::source_address;
        using base::destination_address;
        using base::transfer_size;
    };

    template <typename descriptor_t>
    class batch_descriptor: public any_descriptor<descriptor_t>
    {
        using base = any_descriptor<descriptor_t>;

        template <typename type>
        using return_type_t = typename base::template return_type_t<type>;

    public:
        using base::base;

        [[nodiscard]] auto& descriptor_list_address() noexcept
        {
            return this->source_address();
        }

        [[nodiscard]] auto& descriptors_count() noexcept
        {
            return this->transfer_size();
        }

    private:
        using base::source_address;
        using base::transfer_size;
    };

    template <typename descriptor_t>
    class drain_descriptor: public any_descriptor<descriptor_t>
    {
        using base = any_descriptor<descriptor_t>;

        template <typename type>
        using return_type_t = typename base::template return_type_t<type>;

    public:
        using base::base;

        [[nodiscard]] auto& readback_address_1() noexcept
        {
            return this->source_address();
        }

        [[nodiscard]] auto& readback_address_2() noexcept
        {
            return this->destination_address();
        }

    private:
        using base::source_address;
        using base::destination_address;
    };

    template <typename descriptor_t>
    class mem_move_descriptor: public any_descriptor<descriptor_t>
    {
        using base = any_descriptor<descriptor_t>;

    public:
        using base::base;
    };

    template <typename descriptor_t>
    class fill_descriptor: public any_descriptor<descriptor_t>
    {
        using base = any_descriptor<descriptor_t>;

        template <typename type>
        using return_type_t = typename base::template return_type_t<type>;

    public:
        using base::base;

        [[nodiscard]] auto& pattern() noexcept
        {
            return this->source_address();
        }

    private:
        using base::source_address;
    };

    template <typename descriptor_t>
    class compare_descriptor: public any_descriptor<descriptor_t>
    {
        using base = any_descriptor<descriptor_t>;

        template <typename type>
        using return_type_t = typename base::template return_type_t<type>;

    private:
        struct offsets
        {
            constexpr static auto expected_result = 40u;
        };

    public:
        using base::base;

        [[nodiscard]] auto& source_1_address() noexcept
        {
            return this->source_address();
        }

        [[nodiscard]] auto& source_2_address() noexcept
        {
            return this->destination_address();
        }

        [[nodiscard]] auto& expected_result() noexcept
        {
            return reinterpret_cast<return_type_t<result_t>&>(dsc_.bytes[offsets::expected_result]);
        }

    private:
        using base::source_address;
        using base::destination_address;
        using base::dsc_;
    };

    template <typename descriptor_t>
    class compare_pattern_descriptor: public any_descriptor<descriptor_t>
    {
        using base = any_descriptor<descriptor_t>;

        template <typename type>
        using return_type_t = typename base::template return_type_t<type>;

    private:
        struct offsets
        {
            constexpr static auto expected_result = 40u;
        };

    public:
        using base::base;

        [[nodiscard]] auto& pattern() noexcept
        {
            return this->destination_address();
        }

        [[nodiscard]] auto& expected_result() noexcept
        {
            return reinterpret_cast<return_type_t<result_t>&>(dsc_.bytes[offsets::expected_result]);
        }

    private:
        using base::destination_address;
        using base::dsc_;
    };

    template <typename descriptor_t>
    class create_delta_descriptor: public any_descriptor<descriptor_t>
    {
        using base = any_descriptor<descriptor_t>;

        template <typename type>
        using return_type_t = typename base::template return_type_t<type>;

    private:
        struct offsets
        {
            constexpr static auto delta_record_address      = 40u;
            constexpr static auto maximum_delta_record_size = 48u;
            constexpr static auto expected_result_mask      = 56u;
        };

    public:
        using base::base;

        [[nodiscard]] auto& source_1_address() noexcept
        {
            return this->source_address();
        }

        [[nodiscard]] auto& source_2_address() noexcept
        {
            return this->destination_address();
        }

        [[nodiscard]] auto& delta_record_address() noexcept
        {
            return reinterpret_cast<return_type_t<address_t>&>(dsc_.bytes[offsets::delta_record_address]);
        }

        [[nodiscard]] auto& maximum_delta_record_size() noexcept
        {
            return reinterpret_cast<return_type_t<transfer_size_t>&>(dsc_.bytes[offsets::maximum_delta_record_size]);
        }

        [[nodiscard]] auto& expected_result_mask() noexcept
        {
            return reinterpret_cast<return_type_t<result_t>&>(dsc_.bytes[offsets::expected_result_mask]);
        }

    private:
        using base::source_address;
        using base::destination_address;
        using base::dsc_;
    };

    template <typename descriptor_t>
    class apply_delta_descriptor: public any_descriptor<descriptor_t>
    {
        using base = any_descriptor<descriptor_t>;

        template <typename type>
        using return_type_t = typename base::template return_type_t<type>;

    private:
        struct offsets
        {
            constexpr static auto delta_record_size = 40u;
        };

    public:
        using base::base;

        [[nodiscard]] auto& delta_record_address() noexcept
        {
            return this->source_address();
        }

        [[nodiscard]] auto& delta_record_size() noexcept
        {
            return reinterpret_cast<return_type_t<transfer_size_t>&>(dsc_.bytes[offsets::delta_record_size]);
        }

    private:
        using base::source_address;
        using base::dsc_;
    };

    template <typename descriptor_t>
    class dualcast_descriptor: public any_descriptor<descriptor_t>
    {
        using base = any_descriptor<descriptor_t>;

        template <typename type>
        using return_type_t = typename base::template return_type_t<type>;

    private:
        struct offsets
        {
            constexpr static auto destination_2_address = 40u;
        };

    public:
        using base::base;

        [[nodiscard]] auto& destination_1_address() noexcept
        {
            return this->destination_address();
        }

        [[nodiscard]] auto& destination_2_address() noexcept
        {
            return reinterpret_cast<return_type_t<address_t>&>(dsc_.bytes[offsets::destination_2_address]);
        }

    private:
        using base::destination_address;
        using base::dsc_;
    };

    template <typename descriptor_t>
    class crc_descriptor: public any_descriptor<descriptor_t>
    {
        using base = any_descriptor<descriptor_t>;

        template <typename type>
        using return_type_t = typename base::template return_type_t<type>;

    private:
        struct offsets
        {
            constexpr static auto crc_seed         = 40u;
            constexpr static auto crc_seed_address = 48u;
        };

    public:
        using base::base;

        [[nodiscard]] auto& crc_seed() noexcept
        {
            return reinterpret_cast<return_type_t<crc_value_t>&>(dsc_.bytes[offsets::crc_seed]);
        }

        [[nodiscard]] auto& crc_seed_address() noexcept
        {
            return reinterpret_cast<return_type_t<address_t>&>(dsc_.bytes[offsets::crc_seed_address]);
        }

    private:
        using base::destination_address;
        using base::dsc_;
    };

    template <typename descriptor_t>
    class copy_crc_descriptor: public any_descriptor<descriptor_t>
    {
        using base = any_descriptor<descriptor_t>;

        template <typename type>
        using return_type_t = typename base::template return_type_t<type>;

    private:
        struct offsets
        {
            constexpr static auto crc_seed         = 40u;
            constexpr static auto crc_seed_address = 48u;
        };

    public:
        using base::base;

        [[nodiscard]] auto& crc_seed() noexcept
        {
            return reinterpret_cast<return_type_t<crc_value_t>&>(dsc_.bytes[offsets::crc_seed]);
        }

        [[nodiscard]] auto& crc_seed_address() noexcept
        {
            return reinterpret_cast<return_type_t<address_t>&>(dsc_.bytes[offsets::crc_seed_address]);
        }

    private:
        using base::dsc_;
    };

    template <typename descriptor_t>
    class dif_check_descriptor: public any_descriptor<descriptor_t>
    {
        using base = any_descriptor<descriptor_t>;

        template <typename type>
        using return_type_t = typename base::template return_type_t<type>;

    private:
        struct offsets
        {
            constexpr static auto source_dif_flags    = 40u;
            constexpr static auto dif_flags           = 42u;
            constexpr static auto source_ref_tag_seed = 48u;
            constexpr static auto source_app_tag_mask = 52u;
            constexpr static auto source_app_tag      = 54u;
        };

    public:
        using base::base;

        [[nodiscard]] auto& source_dif_flags() noexcept
        {
            return reinterpret_cast<return_type_t<dif_flags_t>&>(dsc_.bytes[offsets::source_dif_flags]);
        }

        [[nodiscard]] auto& dif_flags() noexcept
        {
            return reinterpret_cast<return_type_t<dif_flags_t>&>(dsc_.bytes[offsets::dif_flags]);
        }

        [[nodiscard]] auto& source_ref_tag() noexcept
        {
            return reinterpret_cast<return_type_t<dif_ref_tag_t>&>(dsc_.bytes[offsets::source_ref_tag_seed]);
        }

        [[nodiscard]] auto& source_app_tag() noexcept
        {
            return reinterpret_cast<return_type_t<dif_app_tag_t>&>(dsc_.bytes[offsets::source_app_tag]);
        }

        [[nodiscard]] auto& source_app_tag_mask() noexcept
        {
            return reinterpret_cast<return_type_t<dif_app_tag_t>&>(dsc_.bytes[offsets::source_app_tag_mask]);
        }

    private:
        using base::destination_address;
        using base::dsc_;
    };

    template <typename descriptor_t>
    class dif_insert_descriptor: public any_descriptor<descriptor_t>
    {
        using base = any_descriptor<descriptor_t>;

        template <typename type>
        using return_type_t = typename base::template return_type_t<type>;

    private:
        struct offsets
        {
            constexpr static auto destination_dif_flags    = 41u;
            constexpr static auto dif_flags                = 42u;
            constexpr static auto destination_ref_tag_seed = 56u;
            constexpr static auto destination_app_tag_mask = 60u;
            constexpr static auto destination_app_tag      = 62u;
        };

    public:
        using base::base;

        [[nodiscard]] auto& destination_dif_flags() noexcept
        {
            return reinterpret_cast<return_type_t<dif_flags_t>&>(dsc_.bytes[offsets::destination_dif_flags]);
        }

        [[nodiscard]] auto& dif_flags() noexcept
        {
            return reinterpret_cast<return_type_t<dif_flags_t>&>(dsc_.bytes[offsets::dif_flags]);
        }

        [[nodiscard]] auto& destination_ref_tag() noexcept
        {
            return reinterpret_cast<return_type_t<dif_ref_tag_t>&>(dsc_.bytes[offsets::destination_ref_tag_seed]);
        }

        [[nodiscard]] auto& destination_app_tag() noexcept
        {
            return reinterpret_cast<return_type_t<dif_app_tag_t>&>(dsc_.bytes[offsets::destination_app_tag]);
        }

        [[nodiscard]] auto& destination_app_tag_mask() noexcept
        {
            return reinterpret_cast<return_type_t<dif_app_tag_t>&>(dsc_.bytes[offsets::destination_app_tag_mask]);
        }

    private:
        using base::dsc_;
    };

    template <typename descriptor_t>
    class dif_strip_descriptor: public any_descriptor<descriptor_t>
    {
        using base = any_descriptor<descriptor_t>;

        template <typename type>
        using return_type_t = typename base::template return_type_t<type>;

    private:
        struct offsets
        {
            constexpr static auto source_dif_flags    = 40u;
            constexpr static auto dif_flags           = 42u;
            constexpr static auto source_ref_tag_seed = 48u;
            constexpr static auto source_app_tag_mask = 52u;
            constexpr static auto source_app_tag      = 54u;
        };

    public:
        using base::base;

        [[nodiscard]] auto& source_dif_flags() noexcept
        {
            return reinterpret_cast<return_type_t<dif_flags_t>&>(dsc_.bytes[offsets::source_dif_flags]);
        }

        [[nodiscard]] auto& dif_flags() noexcept
        {
            return reinterpret_cast<return_type_t<dif_flags_t>&>(dsc_.bytes[offsets::dif_flags]);
        }

        [[nodiscard]] auto& source_ref_tag() noexcept
        {
            return reinterpret_cast<return_type_t<dif_ref_tag_t>&>(dsc_.bytes[offsets::source_ref_tag_seed]);
        }

        [[nodiscard]] auto& source_app_tag() noexcept
        {
            return reinterpret_cast<return_type_t<dif_app_tag_t>&>(dsc_.bytes[offsets::source_app_tag]);
        }

        [[nodiscard]] auto& source_app_tag_mask() noexcept
        {
            return reinterpret_cast<return_type_t<dif_app_tag_t>&>(dsc_.bytes[offsets::source_app_tag_mask]);
        }

    private:
        using base::dsc_;
    };

    template <typename descriptor_t>
    class dif_update_descriptor: public any_descriptor<descriptor_t>
    {
        using base = any_descriptor<descriptor_t>;

        template <typename type>
        using return_type_t = typename base::template return_type_t<type>;

    private:
        struct offsets
        {
            constexpr static auto source_dif_flags         = 40u;
            constexpr static auto destination_dif_flags    = 41u;
            constexpr static auto dif_flags                = 42u;
            constexpr static auto source_ref_tag_seed      = 48u;
            constexpr static auto source_app_tag_mask      = 52u;
            constexpr static auto source_app_tag           = 54u;
            constexpr static auto destination_ref_tag_seed = 56u;
            constexpr static auto destination_app_tag_mask = 60u;
            constexpr static auto destination_app_tag      = 62u;
        };

    public:
        using base::base;

        [[nodiscard]] auto& source_dif_flags() noexcept
        {
            return reinterpret_cast<return_type_t<dif_flags_t>&>(dsc_.bytes[offsets::source_dif_flags]);
        }

        [[nodiscard]] auto& destination_dif_flags() noexcept
        {
            return reinterpret_cast<return_type_t<dif_flags_t>&>(dsc_.bytes[offsets::destination_dif_flags]);
        }

        [[nodiscard]] auto& dif_flags() noexcept
        {
            return reinterpret_cast<return_type_t<dif_flags_t>&>(dsc_.bytes[offsets::dif_flags]);
        }

        [[nodiscard]] auto& source_ref_tag() noexcept
        {
            return reinterpret_cast<return_type_t<dif_ref_tag_t>&>(dsc_.bytes[offsets::source_ref_tag_seed]);
        }

        [[nodiscard]] auto& source_app_tag() noexcept
        {
            return reinterpret_cast<return_type_t<dif_app_tag_t>&>(dsc_.bytes[offsets::source_app_tag]);
        }

        [[nodiscard]] auto& source_app_tag_mask() noexcept
        {
            return reinterpret_cast<return_type_t<dif_app_tag_t>&>(dsc_.bytes[offsets::source_app_tag_mask]);
        }

        [[nodiscard]] auto& destination_ref_tag() noexcept
        {
            return reinterpret_cast<return_type_t<dif_ref_tag_t>&>(dsc_.bytes[offsets::destination_ref_tag_seed]);
        }

        [[nodiscard]] auto& destination_app_tag() noexcept
        {
            return reinterpret_cast<return_type_t<dif_app_tag_t>&>(dsc_.bytes[offsets::destination_app_tag]);
        }

        [[nodiscard]] auto& destination_app_tag_mask() noexcept
        {
            return reinterpret_cast<return_type_t<dif_app_tag_t>&>(dsc_.bytes[offsets::destination_app_tag_mask]);
        }

    private:
        using base::dsc_;
    };

    template <typename descriptor_t>
    class cache_flush_descriptor: public any_descriptor<descriptor_t>
    {
        using base = any_descriptor<descriptor_t>;

    public:
        using base::base;

    private:
        using base::source_address;
    };
}  // namespace dml::core

#endif  //DML_CORE_DESCRIPTOR_VIEW_HPP
