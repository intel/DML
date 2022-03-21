/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_CRC_WORKLOAD_BUILDER_HPP
#define DML_TESTING_CRC_WORKLOAD_BUILDER_HPP

#include <utils/crc/workload.hpp>
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

    template <>
    class WorkloadBuilder<CrcOperation>
    {
    public:
        WorkloadBuilder() noexcept:
            transfer_size_(1),
            src_alignment_(1),
            crc_seed_(0),
            reflection_(reflection_e::enable),
            data_reflection_(data_reflection_e::enable)
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

        [[nodiscard]] auto build()
        {
            return Workload<CrcOperation>(MemoryBuilder()
                                              .add_region(RegionConfiguration()
                                                              .set_size(transfer_size_)
                                                              .set_alignment(src_alignment_))
                                              .build(),
                                          crc_seed_,
                                          reflection_ == reflection_e::disable,
                                          data_reflection_ == data_reflection_e::disable);
        }

    private:
        std::uint32_t     transfer_size_;
        std::uint32_t     src_alignment_;
        std::uint32_t     crc_seed_;
        reflection_e      reflection_;
        data_reflection_e data_reflection_;
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
}  // namespace dml::testing

#endif  //DML_TESTING_CRC_WORKLOAD_BUILDER_HPP
