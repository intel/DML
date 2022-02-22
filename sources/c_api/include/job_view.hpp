/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_JOB_VIEW_HPP
#define DML_JOB_VIEW_HPP

#include <dml/dml.h>

#include <memory>

#include "state.hpp"

namespace dml
{
    constexpr auto get_job_size() noexcept
    {
        constexpr auto alignment  = 64u;
        constexpr auto job_size   = sizeof(dml_job_t);
        constexpr auto state_size = sizeof(dml::state);

        // Enough size for job, state, and to align state to default boundary
        return job_size + state_size + alignment;
    }

    class job_view
    {
    public:
        explicit job_view(dml_job_t* job_ptr) noexcept: job_ptr_(job_ptr)
        {
        }

        [[nodiscard]] auto numa_id() const noexcept
        {
            return job_ptr_->numa_id;
        }

        [[nodiscard]] auto& state() const noexcept
        {
            return *std::launder(reinterpret_cast<dml::state*>(job_ptr_->internal_data_ptr));
        }

        [[nodiscard]] auto operation() const noexcept
        {
            return job_ptr_->operation;
        }

        [[nodiscard]] auto source_first() const noexcept
        {
            return job_ptr_->source_first_ptr;
        }

        [[nodiscard]] auto source_second() const noexcept
        {
            return job_ptr_->source_second_ptr;
        }

        [[nodiscard]] auto destination_first() const noexcept
        {
            return job_ptr_->destination_first_ptr;
        }

        [[nodiscard]] auto destination_second() const noexcept
        {
            return job_ptr_->destination_second_ptr;
        }

        [[nodiscard]] auto crc_ptr() const noexcept
        {
            return job_ptr_->crc_checksum_ptr;
        }

        [[nodiscard]] auto source_length() const noexcept
        {
            return job_ptr_->source_length;
        }

        [[nodiscard]] auto destination_length() const noexcept
        {
            return job_ptr_->destination_length;
        }

        [[nodiscard]] auto pattern() const noexcept
        {
            return *reinterpret_cast<uint64_t*>(job_ptr_->pattern);
        }

        [[nodiscard]] auto expected_result() const noexcept
        {
            return static_cast<uint8_t>(job_ptr_->expected_result);
        }

        [[nodiscard]] auto flags() const noexcept
        {
            // Zeroing-out everything except the first 16 bits
            return static_cast<uint16_t>(job_ptr_->flags & 0xFFFF);
        }

        [[nodiscard]] auto specific_flags() const noexcept
        {
            // Shifting the 3rd byte to the right and then
            // Zeroing-out everything except this byte
            return static_cast<uint8_t>((job_ptr_->flags >> 16) & 0xFF);
        }

        [[nodiscard]] auto dif_src_flags() const noexcept
        {
            return static_cast<uint8_t>(job_ptr_->dif_config.flags & 0xFF);
        }

        [[nodiscard]] auto dif_dst_flags() const noexcept
        {
            return static_cast<uint8_t>((job_ptr_->dif_config.flags >> 8) & 0xFF);
        }

        [[nodiscard]] auto dif_flags() const noexcept
        {
            return static_cast<uint8_t>(((job_ptr_->dif_config.flags >> 16) & 0xFF) | job_ptr_->dif_config.block_size);
        }

        [[nodiscard]] auto src_ref_tag() const noexcept
        {
            return job_ptr_->dif_config.source_reference_tag_seed;
        }

        [[nodiscard]] auto src_app_tag() const noexcept
        {
            return job_ptr_->dif_config.source_application_tag_seed;
        }

        [[nodiscard]] auto src_app_tag_mask() const noexcept
        {
            return job_ptr_->dif_config.source_application_tag_mask;
        }

        [[nodiscard]] auto dst_ref_tag() const noexcept
        {
            return job_ptr_->dif_config.destination_reference_tag_seed;
        }

        [[nodiscard]] auto dst_app_tag() const noexcept
        {
            return job_ptr_->dif_config.destination_application_tag_seed;
        }

        [[nodiscard]] auto dst_app_tag_mask() const noexcept
        {
            return job_ptr_->dif_config.destination_application_tag_mask;
        }

        [[nodiscard]] auto dif_block_size() const noexcept
        {
            constexpr uint32_t dif_block_sizes[4] = { 512u, 520u, 4096u, 4104u };

            return dif_block_sizes[job_ptr_->dif_config.block_size];
        }

        auto set_offset(uint32_t offset) noexcept
        {
            job_ptr_->offset = offset;
        }

        auto set_result(uint8_t result) noexcept
        {
            job_ptr_->result = result;
        }

        auto set_crc(uint32_t value) noexcept
        {
            *job_ptr_->crc_checksum_ptr = value;
        }

        auto set_destination_length(uint32_t value) noexcept
        {
            job_ptr_->destination_length = value;
        }

    private:
        dml_job_t* const job_ptr_;
    };
}  // namespace dml

#endif  // DML_JOB_VIEW_HPP
