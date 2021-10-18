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

/**
 * @date 05/19/2021
 * @brief Contains definitions of @ref dml::ml::completion_record type
 */

#ifndef DML_ML_RESULT_VIEWS_HPP
#define DML_ML_RESULT_VIEWS_HPP

#include "completion_record.hpp"
#include "types.hpp"

namespace dml::ml::views
{
    class any_result
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
        explicit any_result(completion_record &record): record_(record)
        {
        }

        [[nodiscard]] status_t& status() const noexcept
        {
            return reinterpret_cast<status_t &>(record_.bytes[offsets::status]);
        }

        [[nodiscard]] result_t& result() const noexcept
        {
            return reinterpret_cast<result_t &>(record_.bytes[offsets::result]);
        }

        [[nodiscard]] transfer_size_t& bytes_completed() const noexcept
        {
            return reinterpret_cast<transfer_size_t &>(record_.bytes[offsets::bytes_completed]);
        }

        [[nodiscard]] address_t& fault_address() const noexcept
        {
            return reinterpret_cast<address_t &>(record_.bytes[offsets::fault_address]);
        }

    protected:
        completion_record &record_;
    };

    class nop_result: public any_result
    {
    public:
        using any_result::any_result;

    private:
        using any_result::result;
        using any_result::bytes_completed;
        using any_result::fault_address;
    };

    class batch_result: public any_result
    {
    public:
        using any_result::any_result;

        [[nodiscard]] transfer_size_t& descriptors_completed() const noexcept
        {
            return this->bytes_completed();
        }

    private:
        using any_result::result;
    };

    class drain_result: public any_result
    {
    public:
        using any_result::any_result;

    private:
        using any_result::result;
        using any_result::bytes_completed;
        using any_result::fault_address;
    };

    class mem_move_result: public any_result
    {
    public:
        using any_result::any_result;
    };

    class fill_result: public any_result
    {
    public:
        using any_result::any_result;

    private:
        using any_result::result;
    };

    class compare_result: public any_result
    {
    public:
        using any_result::any_result;
    };

    class compare_pattern_result: public any_result
    {
    public:
        using any_result::any_result;
    };

    class create_delta_result: public any_result
    {
    private:
        struct offsets
        {
            constexpr static auto delta_record_size = 16u;
        };

    public:
        using any_result::any_result;

        [[nodiscard]] transfer_size_t& delta_record_size() const noexcept
        {
            return reinterpret_cast<transfer_size_t &>(record_.bytes[offsets::delta_record_size]);
        }
    };

    class apply_delta_result: public any_result
    {
    public:
        using any_result::any_result;

    private:
        using any_result::result;
    };

    class dualcast_result: public any_result
    {
    public:
        using any_result::any_result;

    private:
        using any_result::result;
    };

    class crc_result: public any_result
    {
    private:
        struct offsets
        {
            constexpr static auto crc_value = 16u;
        };

    public:
        using any_result::any_result;

        [[nodiscard]] crc_value_t& crc_value() const noexcept
        {
            return reinterpret_cast<crc_value_t &>(record_.bytes[offsets::crc_value]);
        }

    private:
        using any_result::result;
    };

    class dif_check_result: public any_result
    {
    private:
        struct offsets
        {
            constexpr static auto reference_tag = 16u;
            constexpr static auto application_tag_mask = 20u;
            constexpr static auto application_tag = 22u;
        };

    public:
        using any_result::any_result;

        [[nodiscard]] dif_status_t& dif_status() const noexcept
        {
            return this->result();
        }

        [[nodiscard]] dif_reference_tag_t& source_reference_tag() const noexcept
        {
            return reinterpret_cast<dif_reference_tag_t &>(record_.bytes[offsets::reference_tag]);
        }

        [[nodiscard]] dif_application_tag_t& source_application_tag() const noexcept
        {
            return reinterpret_cast<dif_application_tag_t &>(record_.bytes[offsets::application_tag]);
        }

        [[nodiscard]] dif_application_tag_t& source_application_tag_mask() const noexcept
        {
            return reinterpret_cast<dif_application_tag_t &>(record_.bytes[offsets::application_tag_mask]);
        }

    private:
        using any_result::result;
    };

    class dif_insert_result: public any_result
    {
    private:
        struct offsets
        {
            constexpr static auto reference_tag = 24u;
            constexpr static auto application_tag_mask = 28u;
            constexpr static auto application_tag = 30u;
        };

    public:
        using any_result::any_result;

        [[nodiscard]] dif_reference_tag_t& destination_reference_tag() const noexcept
        {
            return reinterpret_cast<dif_reference_tag_t &>(record_.bytes[offsets::reference_tag]);
        }

        [[nodiscard]] dif_application_tag_t& destination_application_tag() const noexcept
        {
            return reinterpret_cast<dif_application_tag_t &>(record_.bytes[offsets::application_tag]);
        }

        [[nodiscard]] dif_application_tag_t& destination_application_tag_mask() const noexcept
        {
            return reinterpret_cast<dif_application_tag_t &>(record_.bytes[offsets::application_tag_mask]);
        }

    private:
        using any_result::result;
    };

    class dif_strip_result: public any_result
    {
    private:
        struct offsets
        {
            constexpr static auto reference_tag = 16u;
            constexpr static auto application_tag_mask = 20u;
            constexpr static auto application_tag = 22u;
        };

    public:
        using any_result::any_result;

        [[nodiscard]] dif_status_t& dif_status() const noexcept
        {
            return this->result();
        }

        [[nodiscard]] dif_reference_tag_t& source_reference_tag() const noexcept
        {
            return reinterpret_cast<dif_reference_tag_t &>(record_.bytes[offsets::reference_tag]);
        }

        [[nodiscard]] dif_application_tag_t& source_application_tag() const noexcept
        {
            return reinterpret_cast<dif_application_tag_t &>(record_.bytes[offsets::application_tag]);
        }

        [[nodiscard]] dif_application_tag_t& source_application_tag_mask() const noexcept
        {
            return reinterpret_cast<dif_application_tag_t &>(record_.bytes[offsets::application_tag_mask]);
        }

    private:
        using any_result::result;
    };

    class dif_update_result: public any_result
    {
    private:
        struct offsets
        {
            constexpr static auto source_reference_tag = 16u;
            constexpr static auto source_application_tag_mask = 20u;
            constexpr static auto source_application_tag = 22u;
            constexpr static auto destination_reference_tag = 24u;
            constexpr static auto destination_application_tag_mask = 28u;
            constexpr static auto destination_application_tag = 30u;
        };

    public:
        using any_result::any_result;

        [[nodiscard]] dif_status_t& dif_status() const noexcept
        {
            return this->result();
        }

        [[nodiscard]] dif_reference_tag_t& source_reference_tag() const noexcept
        {
            return reinterpret_cast<dif_reference_tag_t &>(record_.bytes[offsets::source_reference_tag]);
        }

        [[nodiscard]] dif_application_tag_t& source_application_tag() const noexcept
        {
            return reinterpret_cast<dif_application_tag_t &>(record_.bytes[offsets::source_application_tag]);
        }

        [[nodiscard]] dif_application_tag_t& source_application_tag_mask() const noexcept
        {
            return reinterpret_cast<dif_application_tag_t &>(record_.bytes[offsets::source_application_tag_mask]);
        }

        [[nodiscard]] dif_reference_tag_t& destination_reference_tag() const noexcept
        {
            return reinterpret_cast<dif_reference_tag_t &>(record_.bytes[offsets::destination_reference_tag]);
        }

        [[nodiscard]] dif_application_tag_t& destination_application_tag() const noexcept
        {
            return reinterpret_cast<dif_application_tag_t &>(record_.bytes[offsets::destination_application_tag]);
        }

        [[nodiscard]] dif_application_tag_t& destination_application_tag_mask() const noexcept
        {
            return reinterpret_cast<dif_application_tag_t &>(record_.bytes[offsets::destination_application_tag_mask]);
        }

    private:
        using any_result::result;
    };

    class cache_flush_result: public any_result
    {
    public:
        using any_result::any_result;

    private:
        using any_result::result;
    };
}  // namespace dml::ml::views

#endif  //DML_ML_RESULT_VIEWS_HPP
