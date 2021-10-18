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

#ifndef DML_ML_DESCRIPTOR_VIEWS_HPP
#define DML_ML_DESCRIPTOR_VIEWS_HPP

#include "descriptor.hpp"
#include "types.hpp"

namespace dml::ml::views
{
    class any_descriptor
    {
    private:
        struct offsets
        {
            constexpr static auto flags = 4u;
            constexpr static auto op_specific_flags = 6u;
            constexpr static auto operation = 7u;
            constexpr static auto completion_record_address = 8u;
            constexpr static auto source_address = 16u;
            constexpr static auto destination_address = 24u;
            constexpr static auto transfer_size = 32u;
            constexpr static auto completion_interrupt_handle = 36u;
        };

    public:
        explicit any_descriptor(descriptor& dsc) : dsc_(dsc) {}

        [[nodiscard]] operation_t& operation() noexcept
        {
            return reinterpret_cast<operation_t&>(dsc_.bytes[offsets::operation]);
        }

        [[nodiscard]] flags_t& flags() noexcept
        {
            return reinterpret_cast<flags_t&>(dsc_.bytes[offsets::flags]);
        }

        [[nodiscard]] operation_specific_flags_t& operation_specific_flags() noexcept
        {
            return reinterpret_cast<operation_specific_flags_t &>(dsc_.bytes[offsets::op_specific_flags]);
        }

        [[nodiscard]] address_t& completion_record_address() noexcept
        {
            return reinterpret_cast<address_t &>(dsc_.bytes[offsets::completion_record_address]);
        }

        [[nodiscard]] address_t& source_address() noexcept
        {
            return reinterpret_cast<address_t &>(dsc_.bytes[offsets::source_address]);
        }

        [[nodiscard]] address_t& destination_address() noexcept
        {
            return reinterpret_cast<address_t &>(dsc_.bytes[offsets::destination_address]);
        }

        [[nodiscard]] transfer_size_t& transfer_size() noexcept
        {
            return reinterpret_cast<transfer_size_t &>(dsc_.bytes[offsets::transfer_size]);
        }

        [[nodiscard]] completion_interrupt_handle_t& completion_interrupt_handle() noexcept
        {
            return reinterpret_cast<completion_interrupt_handle_t &>(dsc_.bytes[offsets::completion_interrupt_handle]);
        }


    protected:
        descriptor& dsc_;
    };

    class nop_descriptor: public any_descriptor
    {
    public:
        using any_descriptor::any_descriptor;

    private:
        using any_descriptor::source_address;
        using any_descriptor::destination_address;
        using any_descriptor::transfer_size;
    };

    class batch_descriptor: public any_descriptor
    {
    public:
        using any_descriptor::any_descriptor;

        [[nodiscard]] address_t& descriptor_list_address() noexcept
        {
            return this->source_address();
        }

        [[nodiscard]] transfer_size_t& descriptors_count() noexcept
        {
            return this->transfer_size();
        }

    private:
        using any_descriptor::source_address;
        using any_descriptor::transfer_size;
    };

    class drain_descriptor: public any_descriptor
    {
    public:
        using any_descriptor::any_descriptor;

        [[nodiscard]] address_t& readback_address_1() noexcept
        {
            return this->source_address();
        }

        [[nodiscard]] address_t& readback_address_2() noexcept
        {
            return this->destination_address();
        }

    private:
        using any_descriptor::source_address;
        using any_descriptor::destination_address;
    };

    class mem_move_descriptor: public any_descriptor
    {
    public:
        using any_descriptor::any_descriptor;
    };

    class fill_descriptor: public any_descriptor
    {
    public:
        using any_descriptor::any_descriptor;

        [[nodiscard]] pattern_t& pattern() noexcept
        {
            return this->source_address();
        }

    private:
        using any_descriptor::source_address;
    };

    class compare_descriptor: public any_descriptor
    {
    private:
        struct offsets
        {
            constexpr static auto expected_result = 40u;
        };

    public:
        using any_descriptor::any_descriptor;

        [[nodiscard]] address_t& source_1_address() noexcept
        {
            return this->source_address();
        }

        [[nodiscard]] address_t& source_2_address() noexcept
        {
            return this->destination_address();
        }

        [[nodiscard]] result_t& expected_result() noexcept
        {
            return reinterpret_cast<result_t &>(dsc_.bytes[offsets::expected_result]);
        }

    private:
        using any_descriptor::source_address;
        using any_descriptor::destination_address;
    };

    class compare_pattern_descriptor: public any_descriptor
    {
    private:
        struct offsets
        {
            constexpr static auto expected_result = 40u;
        };

    public:
        using any_descriptor::any_descriptor;

        [[nodiscard]] pattern_t& pattern() noexcept
        {
            return this->destination_address();
        }

        [[nodiscard]] result_t& expected_result() noexcept
        {
            return reinterpret_cast<result_t &>(dsc_.bytes[offsets::expected_result]);
        }

    private:
        using any_descriptor::destination_address;
    };

    class create_delta_descriptor: public any_descriptor
    {
    private:
        struct offsets
        {
            constexpr static auto delta_record_address = 40u;
            constexpr static auto maximum_delta_record_size = 48u;
            constexpr static auto expected_result_mask = 56u;
        };

    public:
        using any_descriptor::any_descriptor;

        [[nodiscard]] address_t& source_1_address() noexcept
        {
            return this->source_address();
        }

        [[nodiscard]] address_t& source_2_address() noexcept
        {
            return this->destination_address();
        }

        [[nodiscard]] address_t& delta_record_address() noexcept
        {
            return reinterpret_cast<address_t &>(dsc_.bytes[offsets::delta_record_address]);
        }

        [[nodiscard]] transfer_size_t& maximum_delta_record_size() noexcept
        {
            return reinterpret_cast<transfer_size_t &>(dsc_.bytes[offsets::maximum_delta_record_size]);
        }

        [[nodiscard]] result_t& expected_result_mask() noexcept
        {
            return reinterpret_cast<result_t &>(dsc_.bytes[offsets::expected_result_mask]);
        }

    private:
        using any_descriptor::source_address;
        using any_descriptor::destination_address;
    };

    class apply_delta_descriptor: public any_descriptor
    {
    private:
        struct offsets
        {
            constexpr static auto delta_record_size = 40u;
        };

    public:
        using any_descriptor::any_descriptor;

        [[nodiscard]] address_t& delta_record_address() noexcept
        {
            return this->source_address();
        }

        [[nodiscard]] transfer_size_t& delta_record_size() noexcept
        {
            return reinterpret_cast<transfer_size_t &>(dsc_.bytes[offsets::delta_record_size]);
        }

    private:
        using any_descriptor::source_address;
    };

    class dualcast_descriptor: public any_descriptor
    {
    private:
        struct offsets
        {
            constexpr static auto destination_2_address = 40u;
        };

    public:
        using any_descriptor::any_descriptor;

        [[nodiscard]] address_t& destination_1_address() noexcept
        {
            return this->destination_address();
        }

        [[nodiscard]] address_t& destination_2_address() noexcept
        {
            return reinterpret_cast<address_t &>(dsc_.bytes[offsets::destination_2_address]);
        }

    private:
        using any_descriptor::destination_address;
    };

    class crc_descriptor: public any_descriptor
    {
    private:
        struct offsets
        {
            constexpr static auto crc_seed = 40u;
            constexpr static auto crc_seed_address = 48u;
        };

    public:
        using any_descriptor::any_descriptor;

        [[nodiscard]] crc_value_t& crc_seed() noexcept
        {
            return reinterpret_cast<crc_value_t &>(dsc_.bytes[offsets::crc_seed]);
        }

        [[nodiscard]] address_t& crc_seed_address() noexcept
        {
            return reinterpret_cast<address_t &>(dsc_.bytes[offsets::crc_seed_address]);
        }

    private:
        using any_descriptor::destination_address;
    };

    class copy_crc_descriptor: public any_descriptor
    {
    private:
        struct offsets
        {
            constexpr static auto crc_seed = 40u;
            constexpr static auto crc_seed_address = 48u;
        };

    public:
        using any_descriptor::any_descriptor;

        [[nodiscard]] crc_value_t& crc_seed() noexcept
        {
            return reinterpret_cast<crc_value_t &>(dsc_.bytes[offsets::crc_seed]);
        }

        [[nodiscard]] address_t& crc_seed_address() noexcept
        {
            return reinterpret_cast<address_t &>(dsc_.bytes[offsets::crc_seed_address]);
        }
    };

    class dif_check_descriptor: public any_descriptor
    {
    private:
        struct offsets
        {
            constexpr static auto source_dif_flags = 40u;
            constexpr static auto dif_flags = 42u;
            constexpr static auto source_reference_tag_seed = 48u;
            constexpr static auto source_application_tag_mask = 52u;
            constexpr static auto source_application_tag = 54u;
        };

    public:
        using any_descriptor::any_descriptor;

        [[nodiscard]] dif_flags_t& source_dif_flags() noexcept
        {
            return reinterpret_cast<dif_flags_t &>(dsc_.bytes[offsets::source_dif_flags]);
        }

        [[nodiscard]] dif_flags_t& dif_flags() noexcept
        {
            return reinterpret_cast<dif_flags_t &>(dsc_.bytes[offsets::dif_flags]);
        }

        [[nodiscard]] dif_reference_tag_t& source_reference_tag() noexcept
        {
            return reinterpret_cast<dif_reference_tag_t &>(dsc_.bytes[offsets::source_reference_tag_seed]);
        }

        [[nodiscard]] dif_application_tag_t& source_application_tag() noexcept
        {
            return reinterpret_cast<dif_application_tag_t &>(dsc_.bytes[offsets::source_application_tag]);
        }

        [[nodiscard]] dif_application_tag_t& source_application_tag_mask() noexcept
        {
            return reinterpret_cast<dif_application_tag_t &>(dsc_.bytes[offsets::source_application_tag_mask]);
        }

    private:
        using any_descriptor::destination_address;
    };

    class dif_insert_descriptor: public any_descriptor
    {
    private:
        struct offsets
        {
            constexpr static auto destination_dif_flags = 41u;
            constexpr static auto dif_flags = 42u;
            constexpr static auto destination_reference_tag_seed = 56u;
            constexpr static auto destination_application_tag_mask = 60u;
            constexpr static auto destination_application_tag = 62u;
        };

    public:
        using any_descriptor::any_descriptor;

        [[nodiscard]] dif_flags_t& destination_dif_flags() noexcept
        {
            return reinterpret_cast<dif_flags_t &>(dsc_.bytes[offsets::destination_dif_flags]);
        }

        [[nodiscard]] dif_flags_t& dif_flags() noexcept
        {
            return reinterpret_cast<dif_flags_t &>(dsc_.bytes[offsets::dif_flags]);
        }

        [[nodiscard]] dif_reference_tag_t& destination_reference_tag() noexcept
        {
            return reinterpret_cast<dif_reference_tag_t &>(dsc_.bytes[offsets::destination_reference_tag_seed]);
        }

        [[nodiscard]] dif_application_tag_t& destination_application_tag() noexcept
        {
            return reinterpret_cast<dif_application_tag_t &>(dsc_.bytes[offsets::destination_application_tag]);
        }

        [[nodiscard]] dif_application_tag_t& destination_application_tag_mask() noexcept
        {
            return reinterpret_cast<dif_application_tag_t &>(dsc_.bytes[offsets::destination_application_tag_mask]);
        }
    };

    class dif_strip_descriptor: public any_descriptor
    {
    private:
        struct offsets
        {
            constexpr static auto source_dif_flags = 40u;
            constexpr static auto dif_flags = 42u;
            constexpr static auto source_reference_tag_seed = 48u;
            constexpr static auto source_application_tag_mask = 52u;
            constexpr static auto source_application_tag = 54u;
        };

    public:
        using any_descriptor::any_descriptor;

        [[nodiscard]] dif_flags_t& source_dif_flags() noexcept
        {
            return reinterpret_cast<dif_flags_t &>(dsc_.bytes[offsets::source_dif_flags]);
        }

        [[nodiscard]] dif_flags_t& dif_flags() noexcept
        {
            return reinterpret_cast<dif_flags_t &>(dsc_.bytes[offsets::dif_flags]);
        }

        [[nodiscard]] dif_reference_tag_t& source_reference_tag() noexcept
        {
            return reinterpret_cast<dif_reference_tag_t &>(dsc_.bytes[offsets::source_reference_tag_seed]);
        }

        [[nodiscard]] dif_application_tag_t& source_application_tag() noexcept
        {
            return reinterpret_cast<dif_application_tag_t &>(dsc_.bytes[offsets::source_application_tag]);
        }

        [[nodiscard]] dif_application_tag_t& source_application_tag_mask() noexcept
        {
            return reinterpret_cast<dif_application_tag_t &>(dsc_.bytes[offsets::source_application_tag_mask]);
        }
    };

    class dif_update_descriptor: public any_descriptor
    {
    private:
        struct offsets
        {
            constexpr static auto source_dif_flags = 40u;
            constexpr static auto destination_dif_flags = 41u;
            constexpr static auto dif_flags = 42u;
            constexpr static auto source_reference_tag_seed = 48u;
            constexpr static auto source_application_tag_mask = 52u;
            constexpr static auto source_application_tag = 54u;
            constexpr static auto destination_reference_tag_seed = 56u;
            constexpr static auto destination_application_tag_mask = 60u;
            constexpr static auto destination_application_tag = 62u;
        };

    public:
        using any_descriptor::any_descriptor;

        [[nodiscard]] dif_flags_t& source_dif_flags() noexcept
        {
            return reinterpret_cast<dif_flags_t &>(dsc_.bytes[offsets::source_dif_flags]);
        }

        [[nodiscard]] dif_flags_t& destination_dif_flags() noexcept
        {
            return reinterpret_cast<dif_flags_t &>(dsc_.bytes[offsets::destination_dif_flags]);
        }

        [[nodiscard]] dif_flags_t& dif_flags() noexcept
        {
            return reinterpret_cast<dif_flags_t &>(dsc_.bytes[offsets::dif_flags]);
        }

        [[nodiscard]] dif_reference_tag_t& source_reference_tag() noexcept
        {
            return reinterpret_cast<dif_reference_tag_t &>(dsc_.bytes[offsets::source_reference_tag_seed]);
        }

        [[nodiscard]] dif_application_tag_t& source_application_tag() noexcept
        {
            return reinterpret_cast<dif_application_tag_t &>(dsc_.bytes[offsets::source_application_tag]);
        }

        [[nodiscard]] dif_application_tag_t& source_application_tag_mask() noexcept
        {
            return reinterpret_cast<dif_application_tag_t &>(dsc_.bytes[offsets::source_application_tag_mask]);
        }

        [[nodiscard]] dif_reference_tag_t& destination_reference_tag() noexcept
        {
            return reinterpret_cast<dif_reference_tag_t &>(dsc_.bytes[offsets::destination_reference_tag_seed]);
        }

        [[nodiscard]] dif_application_tag_t& destination_application_tag() noexcept
        {
            return reinterpret_cast<dif_application_tag_t &>(dsc_.bytes[offsets::destination_application_tag]);
        }

        [[nodiscard]] dif_application_tag_t& destination_application_tag_mask() noexcept
        {
            return reinterpret_cast<dif_application_tag_t &>(dsc_.bytes[offsets::destination_application_tag_mask]);
        }
    };

    class cache_flush_descriptor: public any_descriptor
    {
    public:
        using any_descriptor::any_descriptor;

    private:
        using any_descriptor::source_address;
    };
}  // namespace dml::ml

#endif  //DML_ML_DESCRIPTOR_VIEWS_HPP
