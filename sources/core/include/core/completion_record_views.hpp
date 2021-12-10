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

#ifndef DML_CORE_COMPLETION_RECORD_VIEWS_HPP
#define DML_CORE_COMPLETION_RECORD_VIEWS_HPP

#include <core/types.hpp>

namespace dml::core
{
    class any_completion_record
    {
    private:
        struct offsets
        {
            constexpr static auto status          = 0u;
            constexpr static auto result          = 1u;
            constexpr static auto bytes_completed = 4u;
            constexpr static auto fault_address   = 8u;
        };

    public:
        explicit any_completion_record(completion_record& record): record_(record)
        {
        }

        [[nodiscard]] status_t& status() const noexcept
        {
            return reinterpret_cast<status_t&>(record_.bytes[offsets::status]);
        }

        [[nodiscard]] result_t& result() const noexcept
        {
            return reinterpret_cast<result_t&>(record_.bytes[offsets::result]);
        }

        [[nodiscard]] transfer_size_t& bytes_completed() const noexcept
        {
            return reinterpret_cast<transfer_size_t&>(record_.bytes[offsets::bytes_completed]);
        }

        [[nodiscard]] address_t& fault_address() const noexcept
        {
            return reinterpret_cast<address_t&>(record_.bytes[offsets::fault_address]);
        }

    protected:
        completion_record& record_;
    };

    class nop_completion_record: public any_completion_record
    {
    public:
        using any_completion_record::any_completion_record;

    private:
        using any_completion_record::result;
        using any_completion_record::bytes_completed;
        using any_completion_record::fault_address;
    };

    class batch_completion_record: public any_completion_record
    {
    public:
        using any_completion_record::any_completion_record;

        [[nodiscard]] transfer_size_t& descriptors_completed() const noexcept
        {
            return this->bytes_completed();
        }

    private:
        using any_completion_record::result;
    };

    class drain_completion_record: public any_completion_record
    {
    public:
        using any_completion_record::any_completion_record;

    private:
        using any_completion_record::result;
        using any_completion_record::bytes_completed;
        using any_completion_record::fault_address;
    };

    class mem_move_completion_record: public any_completion_record
    {
    public:
        using any_completion_record::any_completion_record;
    };

    class fill_completion_record: public any_completion_record
    {
    public:
        using any_completion_record::any_completion_record;

    private:
        using any_completion_record::result;
    };

    class compare_completion_record: public any_completion_record
    {
    public:
        using any_completion_record::any_completion_record;
    };

    class compare_pattern_completion_record: public any_completion_record
    {
    public:
        using any_completion_record::any_completion_record;
    };

    class create_delta_completion_record: public any_completion_record
    {
    private:
        struct offsets
        {
            constexpr static auto delta_record_size = 16u;
        };

    public:
        using any_completion_record::any_completion_record;

        [[nodiscard]] transfer_size_t& delta_record_size() const noexcept
        {
            return reinterpret_cast<transfer_size_t&>(record_.bytes[offsets::delta_record_size]);
        }
    };

    class apply_delta_completion_record: public any_completion_record
    {
    public:
        using any_completion_record::any_completion_record;

    private:
        using any_completion_record::result;
    };

    class dualcast_completion_record: public any_completion_record
    {
    public:
        using any_completion_record::any_completion_record;

    private:
        using any_completion_record::result;
    };

    class crc_completion_record: public any_completion_record
    {
    private:
        struct offsets
        {
            constexpr static auto crc_value = 16u;
        };

    public:
        using any_completion_record::any_completion_record;

        [[nodiscard]] crc_value_t& crc_value() const noexcept
        {
            return reinterpret_cast<crc_value_t&>(record_.bytes[offsets::crc_value]);
        }

    private:
        using any_completion_record::result;
    };

    class dif_check_completion_record: public any_completion_record
    {
    private:
        struct offsets
        {
            constexpr static auto ref_tag      = 16u;
            constexpr static auto app_tag_mask = 20u;
            constexpr static auto app_tag      = 22u;
        };

    public:
        using any_completion_record::any_completion_record;

        [[nodiscard]] dif_status_t& dif_status() const noexcept
        {
            return this->result();
        }

        [[nodiscard]] dif_ref_tag_t& source_ref_tag() const noexcept
        {
            return reinterpret_cast<dif_ref_tag_t&>(record_.bytes[offsets::ref_tag]);
        }

        [[nodiscard]] dif_app_tag_t& source_app_tag() const noexcept
        {
            return reinterpret_cast<dif_app_tag_t&>(record_.bytes[offsets::app_tag]);
        }

        [[nodiscard]] dif_app_tag_t& source_app_tag_mask() const noexcept
        {
            return reinterpret_cast<dif_app_tag_t&>(record_.bytes[offsets::app_tag_mask]);
        }

    private:
        using any_completion_record::result;
    };

    class dif_insert_completion_record: public any_completion_record
    {
    private:
        struct offsets
        {
            constexpr static auto ref_tag      = 24u;
            constexpr static auto app_tag_mask = 28u;
            constexpr static auto app_tag      = 30u;
        };

    public:
        using any_completion_record::any_completion_record;

        [[nodiscard]] dif_ref_tag_t& destination_ref_tag() const noexcept
        {
            return reinterpret_cast<dif_ref_tag_t&>(record_.bytes[offsets::ref_tag]);
        }

        [[nodiscard]] dif_app_tag_t& destination_app_tag() const noexcept
        {
            return reinterpret_cast<dif_app_tag_t&>(record_.bytes[offsets::app_tag]);
        }

        [[nodiscard]] dif_app_tag_t& destination_app_tag_mask() const noexcept
        {
            return reinterpret_cast<dif_app_tag_t&>(record_.bytes[offsets::app_tag_mask]);
        }

    private:
        using any_completion_record::result;
    };

    class dif_strip_completion_record: public any_completion_record
    {
    private:
        struct offsets
        {
            constexpr static auto ref_tag      = 16u;
            constexpr static auto app_tag_mask = 20u;
            constexpr static auto app_tag      = 22u;
        };

    public:
        using any_completion_record::any_completion_record;

        [[nodiscard]] dif_status_t& dif_status() const noexcept
        {
            return this->result();
        }

        [[nodiscard]] dif_ref_tag_t& source_ref_tag() const noexcept
        {
            return reinterpret_cast<dif_ref_tag_t&>(record_.bytes[offsets::ref_tag]);
        }

        [[nodiscard]] dif_app_tag_t& source_app_tag() const noexcept
        {
            return reinterpret_cast<dif_app_tag_t&>(record_.bytes[offsets::app_tag]);
        }

        [[nodiscard]] dif_app_tag_t& source_app_tag_mask() const noexcept
        {
            return reinterpret_cast<dif_app_tag_t&>(record_.bytes[offsets::app_tag_mask]);
        }

    private:
        using any_completion_record::result;
    };

    class dif_update_completion_record: public any_completion_record
    {
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
        using any_completion_record::any_completion_record;

        [[nodiscard]] dif_status_t& dif_status() const noexcept
        {
            return this->result();
        }

        [[nodiscard]] dif_ref_tag_t& source_ref_tag() const noexcept
        {
            return reinterpret_cast<dif_ref_tag_t&>(record_.bytes[offsets::source_ref_tag]);
        }

        [[nodiscard]] dif_app_tag_t& source_app_tag() const noexcept
        {
            return reinterpret_cast<dif_app_tag_t&>(record_.bytes[offsets::source_app_tag]);
        }

        [[nodiscard]] dif_app_tag_t& source_app_tag_mask() const noexcept
        {
            return reinterpret_cast<dif_app_tag_t&>(record_.bytes[offsets::source_app_tag_mask]);
        }

        [[nodiscard]] dif_ref_tag_t& destination_ref_tag() const noexcept
        {
            return reinterpret_cast<dif_ref_tag_t&>(record_.bytes[offsets::destination_ref_tag]);
        }

        [[nodiscard]] dif_app_tag_t& destination_app_tag() const noexcept
        {
            return reinterpret_cast<dif_app_tag_t&>(record_.bytes[offsets::destination_app_tag]);
        }

        [[nodiscard]] dif_app_tag_t& destination_app_tag_mask() const noexcept
        {
            return reinterpret_cast<dif_app_tag_t&>(record_.bytes[offsets::destination_app_tag_mask]);
        }

    private:
        using any_completion_record::result;
    };

    class cache_flush_completion_record: public any_completion_record
    {
    public:
        using any_completion_record::any_completion_record;

    private:
        using any_completion_record::result;
    };
}  // namespace dml::core

#endif  //DML_CORE_COMPLETION_RECORD_VIEWS_HPP
