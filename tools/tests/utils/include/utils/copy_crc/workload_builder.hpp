/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_COPY_CRC_WORKLOAD_BUILDER_HPP
#define DML_TESTING_COPY_CRC_WORKLOAD_BUILDER_HPP

#include <ostream>
#include <stdexcept>
#include <utils/copy_crc/workload.hpp>
#include <utils/memory.hpp>
#include <utils/operation.hpp>

namespace dml::testing
{
    enum class reflection_e
    {
        enable,
        disable
    };

    static inline std::ostream& operator<<(std::ostream& ostream, reflection_e reflection);

    enum class data_reflection_e
    {
        enable,
        disable
    };

    static inline std::ostream& operator<<(std::ostream&     ostream,
                                           data_reflection_e data_reflection);

    enum class block_on_fault_e
    {
        block,
        dont_block
    };

    static inline std::ostream& operator<<(std::ostream& ostream, block_on_fault_e block_on_fault);

    template <>
    class WorkloadBuilder<CopyCrcOperation>
    {
    public:
        WorkloadBuilder() noexcept:
            transfer_size_(1),
            src_alignment_(1),
            dst_alignment_(1),
            crc_seed_(0),
            reflection_(reflection_e::enable),
            data_reflection_(data_reflection_e::enable),
            block_on_fault_(block_on_fault_e::dont_block)
        {
        }

        auto& set_transfer_size(std::uint32_t size) noexcept
        {
            transfer_size_ = size;

            return *this;
        }

        auto& set_src_alignment(std::uint32_t size) noexcept
        {
            src_alignment_ = size;

            return *this;
        }

        auto& set_dst_alignment(std::uint32_t size) noexcept
        {
            dst_alignment_ = size;

            return *this;
        }

        auto& set_crc_seed(std::uint32_t seed) noexcept
        {
            crc_seed_ = seed;

            return *this;
        }

        auto& set_reflection(reflection_e reflection) noexcept
        {
            reflection_ = reflection;

            return *this;
        }

        auto& set_data_reflection(data_reflection_e data_reflection) noexcept
        {
            data_reflection_ = data_reflection;

            return *this;
        }

        auto& set_block_on_fault(block_on_fault_e block_on_fault) noexcept
        {
            block_on_fault_ = block_on_fault;

            return *this;
        }

        [[nodiscard]] auto build()
        {
            return Workload<CopyCrcOperation>(MemoryBuilder()
                                                  .add_region(RegionConfiguration()
                                                                  .set_size(transfer_size_)
                                                                  .set_alignment(src_alignment_))
                                                  .add_region(RegionConfiguration()
                                                                  .set_size(transfer_size_)
                                                                  .set_alignment(dst_alignment_))
                                                  .build(),
                                              crc_seed_,
                                              reflection_ == reflection_e::disable,
                                              data_reflection_ == data_reflection_e::disable,
                                              block_on_fault_ == block_on_fault_e::block);
        }

    private:
        std::uint32_t     transfer_size_;
        std::uint32_t     src_alignment_;
        std::uint32_t     dst_alignment_;
        std::uint32_t     crc_seed_;
        reflection_e      reflection_;
        data_reflection_e data_reflection_;
        block_on_fault_e  block_on_fault_;
    };

    static inline std::ostream& operator<<(std::ostream& ostream, reflection_e reflection)
    {
        if (reflection == reflection_e::enable)
        {
            return ostream << "reflection_enabled";
        }
        else if (reflection == reflection_e::disable)
        {
            return ostream << "reflection_disabled";
        }
        else
        {
            throw std::logic_error("Unexpected enumeration");
        }
    }

    static inline std::ostream& operator<<(std::ostream& ostream, data_reflection_e data_reflection)
    {
        if (data_reflection == data_reflection_e::enable)
        {
            return ostream << "data_reflection_enabled";
        }
        else if (data_reflection == data_reflection_e::disable)
        {
            return ostream << "data_reflection_disabled";
        }
        else
        {
            throw std::logic_error("Unexpected enumeration");
        }
    }

    static inline std::ostream& operator<<(std::ostream& ostream, block_on_fault_e block_on_fault)
    {
        if (block_on_fault == block_on_fault_e::block)
        {
            return ostream << "block_on_fault";
        }
        else if (block_on_fault == block_on_fault_e::dont_block)
        {
            return ostream << "dont_block_on_fault";
        }
        else
        {
            throw std::logic_error("Unexpected enumeration");
        }
    }
}  // namespace dml::testing

#endif  //DML_TESTING_COPY_CRC_WORKLOAD_BUILDER_HPP
