/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_CORE_COMPLETION_RECORD_VIEWS_HPP
#define DML_CORE_COMPLETION_RECORD_VIEWS_HPP

#include <core/types.hpp>
#include <type_traits>

namespace dml::core
{
    template <typename completion_record_t>
    class any_completion_record
    {
        // Requires
        static_assert(std::is_same_v<std::decay_t<completion_record_t>, completion_record>);

    protected:
        // Add const if view is created over constant object
        template <typename type>
        using return_type_t = std::conditional_t<std::is_const_v<completion_record_t>, std::add_const_t<type>, type>;

    private:
        struct offsets
        {
            constexpr static auto status          = 0u;
            constexpr static auto result          = 1u;
            constexpr static auto bytes_completed = 4u;
            constexpr static auto fault_address   = 8u;
        };

    public:
        explicit any_completion_record(completion_record_t& record): record_(record)
        {
        }

        [[nodiscard]] auto& status() noexcept
        {
            return reinterpret_cast<return_type_t<status_t>&>(record_.bytes[offsets::status]);
        }

        [[nodiscard]] auto& result() noexcept
        {
            return reinterpret_cast<return_type_t<result_t>&>(record_.bytes[offsets::result]);
        }

        [[nodiscard]] auto& bytes_completed() noexcept
        {
            return reinterpret_cast<return_type_t<transfer_size_t>&>(record_.bytes[offsets::bytes_completed]);
        }

        [[nodiscard]] auto& fault_address() noexcept
        {
            return reinterpret_cast<return_type_t<address_t>&>(record_.bytes[offsets::fault_address]);
        }

    protected:
        completion_record_t& record_;
    };

    template <typename completion_record_t>
    class nop_completion_record: public any_completion_record<completion_record_t>
    {
        using base = any_completion_record<completion_record_t>;

    public:
        using base::base;

    private:
        using base::result;
        using base::bytes_completed;
        using base::fault_address;
    };

    template <typename completion_record_t>
    class batch_completion_record: public any_completion_record<completion_record_t>
    {
        using base = any_completion_record<completion_record_t>;

        template <typename type>
        using return_type_t = typename base::template return_type_t<type>;

    public:
        using base::base;

        [[nodiscard]] auto& descriptors_completed() noexcept
        {
            return this->bytes_completed();
        }

    private:
        using base::result;
    };

    template <typename completion_record_t>
    class drain_completion_record: public any_completion_record<completion_record_t>
    {
        using base = any_completion_record<completion_record_t>;

    public:
        using base::base;

    private:
        using base::result;
        using base::bytes_completed;
        using base::fault_address;
    };

    template <typename completion_record_t>
    class mem_move_completion_record: public any_completion_record<completion_record_t>
    {
        using base = any_completion_record<completion_record_t>;

    public:
        using base::base;
    };

    template <typename completion_record_t>
    class fill_completion_record: public any_completion_record<completion_record_t>
    {
        using base = any_completion_record<completion_record_t>;

    public:
        using base::base;

    private:
        using base::result;
    };

    template <typename completion_record_t>
    class compare_completion_record: public any_completion_record<completion_record_t>
    {
        using base = any_completion_record<completion_record_t>;

    public:
        using base::base;
    };

    template <typename completion_record_t>
    class compare_pattern_completion_record: public any_completion_record<completion_record_t>
    {
        using base = any_completion_record<completion_record_t>;

    public:
        using base::base;
    };

    template <typename completion_record_t>
    class create_delta_completion_record: public any_completion_record<completion_record_t>
    {
        using base = any_completion_record<completion_record_t>;

        template <typename type>
        using return_type_t = typename base::template return_type_t<type>;

    private:
        struct offsets
        {
            constexpr static auto delta_record_size = 16u;
        };

    public:
        using base::base;

        [[nodiscard]] auto& delta_record_size() noexcept
        {
            return reinterpret_cast<return_type_t<transfer_size_t>&>(record_.bytes[offsets::delta_record_size]);
        }

    private:
        using base::record_;
    };

    template <typename completion_record_t>
    class apply_delta_completion_record: public any_completion_record<completion_record_t>
    {
        using base = any_completion_record<completion_record_t>;

    public:
        using base::base;

    private:
        using base::result;
    };

    template <typename completion_record_t>
    class dualcast_completion_record: public any_completion_record<completion_record_t>
    {
        using base = any_completion_record<completion_record_t>;

    public:
        using base::base;

    private:
        using base::result;
    };

    template <typename completion_record_t>
    class crc_completion_record: public any_completion_record<completion_record_t>
    {
        using base = any_completion_record<completion_record_t>;

        template <typename type>
        using return_type_t = typename base::template return_type_t<type>;

    private:
        struct offsets
        {
            constexpr static auto crc_value = 16u;
        };

    public:
        using base::base;

        [[nodiscard]] auto& crc_value() noexcept
        {
            return reinterpret_cast<return_type_t<crc_value_t>&>(record_.bytes[offsets::crc_value]);
        }

    private:
        using base::record_;
        using base::result;
    };

    template <typename completion_record_t>
    class dif_check_completion_record: public any_completion_record<completion_record_t>
    {
        using base = any_completion_record<completion_record_t>;

        template <typename type>
        using return_type_t = typename base::template return_type_t<type>;

    private:
        struct offsets
        {
            constexpr static auto ref_tag      = 16u;
            constexpr static auto app_tag_mask = 20u;
            constexpr static auto app_tag      = 22u;
        };

    public:
        using base::base;

        [[nodiscard]] auto& dif_status() noexcept
        {
            return this->result();
        }

        [[nodiscard]] auto& source_ref_tag() noexcept
        {
            return reinterpret_cast<return_type_t<dif_ref_tag_t>&>(record_.bytes[offsets::ref_tag]);
        }

        [[nodiscard]] auto& source_app_tag() noexcept
        {
            return reinterpret_cast<return_type_t<dif_app_tag_t>&>(record_.bytes[offsets::app_tag]);
        }

        [[nodiscard]] auto& source_app_tag_mask() noexcept
        {
            return reinterpret_cast<return_type_t<dif_app_tag_t>&>(record_.bytes[offsets::app_tag_mask]);
        }

    private:
        using base::result;
        using base::record_;
    };

    template <typename completion_record_t>
    class dif_insert_completion_record: public any_completion_record<completion_record_t>
    {
        using base = any_completion_record<completion_record_t>;

        template <typename type>
        using return_type_t = typename base::template return_type_t<type>;

    private:
        struct offsets
        {
            constexpr static auto ref_tag      = 24u;
            constexpr static auto app_tag_mask = 28u;
            constexpr static auto app_tag      = 30u;
        };

    public:
        using base::base;

        [[nodiscard]] auto& destination_ref_tag() noexcept
        {
            return reinterpret_cast<return_type_t<dif_ref_tag_t>&>(record_.bytes[offsets::ref_tag]);
        }

        [[nodiscard]] auto& destination_app_tag() noexcept
        {
            return reinterpret_cast<return_type_t<dif_app_tag_t>&>(record_.bytes[offsets::app_tag]);
        }

        [[nodiscard]] auto& destination_app_tag_mask() noexcept
        {
            return reinterpret_cast<return_type_t<dif_app_tag_t>&>(record_.bytes[offsets::app_tag_mask]);
        }

    private:
        using base::result;
        using base::record_;
    };

    template <typename completion_record_t>
    class dif_strip_completion_record: public any_completion_record<completion_record_t>
    {
        using base = any_completion_record<completion_record_t>;

        template <typename type>
        using return_type_t = typename base::template return_type_t<type>;

    private:
        struct offsets
        {
            constexpr static auto ref_tag      = 16u;
            constexpr static auto app_tag_mask = 20u;
            constexpr static auto app_tag      = 22u;
        };

    public:
        using base::base;

        [[nodiscard]] auto& dif_status() noexcept
        {
            return this->result();
        }

        [[nodiscard]] auto& source_ref_tag() noexcept
        {
            return reinterpret_cast<return_type_t<dif_ref_tag_t>&>(record_.bytes[offsets::ref_tag]);
        }

        [[nodiscard]] auto& source_app_tag() noexcept
        {
            return reinterpret_cast<return_type_t<dif_app_tag_t>&>(record_.bytes[offsets::app_tag]);
        }

        [[nodiscard]] auto& source_app_tag_mask() noexcept
        {
            return reinterpret_cast<return_type_t<dif_app_tag_t>&>(record_.bytes[offsets::app_tag_mask]);
        }

    private:
        using base::result;
        using base::record_;
    };

    template <typename completion_record_t>
    class dif_update_completion_record: public any_completion_record<completion_record_t>
    {
        using base = any_completion_record<completion_record_t>;

        template <typename type>
        using return_type_t = typename base::template return_type_t<type>;

    private:
        struct offsets
        {
            constexpr static auto source_ref_tag           = 16u;
            constexpr static auto source_app_tag_mask      = 20u;
            constexpr static auto source_app_tag           = 22u;
            constexpr static auto destination_ref_tag      = 24u;
            constexpr static auto destination_app_tag_mask = 28u;
            constexpr static auto destination_app_tag      = 30u;
        };

    public:
        using base::base;

        [[nodiscard]] auto& dif_status() noexcept
        {
            return this->result();
        }

        [[nodiscard]] auto& source_ref_tag() noexcept
        {
            return reinterpret_cast<return_type_t<dif_ref_tag_t>&>(record_.bytes[offsets::source_ref_tag]);
        }

        [[nodiscard]] auto& source_app_tag() noexcept
        {
            return reinterpret_cast<return_type_t<dif_app_tag_t>&>(record_.bytes[offsets::source_app_tag]);
        }

        [[nodiscard]] auto& source_app_tag_mask() noexcept
        {
            return reinterpret_cast<return_type_t<dif_app_tag_t>&>(record_.bytes[offsets::source_app_tag_mask]);
        }

        [[nodiscard]] auto& destination_ref_tag() noexcept
        {
            return reinterpret_cast<return_type_t<dif_ref_tag_t>&>(record_.bytes[offsets::destination_ref_tag]);
        }

        [[nodiscard]] auto& destination_app_tag() noexcept
        {
            return reinterpret_cast<return_type_t<dif_app_tag_t>&>(record_.bytes[offsets::destination_app_tag]);
        }

        [[nodiscard]] auto& destination_app_tag_mask() noexcept
        {
            return reinterpret_cast<return_type_t<dif_app_tag_t>&>(record_.bytes[offsets::destination_app_tag_mask]);
        }

    private:
        using base::result;
        using base::record_;
    };

    template <typename completion_record_t>
    class cache_flush_completion_record: public any_completion_record<completion_record_t>
    {
        using base = any_completion_record<completion_record_t>;

    public:
        using base::base;

    private:
        using base::result;
    };
}  // namespace dml::core

#endif  //DML_CORE_COMPLETION_RECORD_VIEWS_HPP
