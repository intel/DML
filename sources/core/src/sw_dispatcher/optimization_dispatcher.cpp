/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include "optimization_dispatcher.hpp"

#include <tuple>

#include "dml_cpuid.h"
#include "dml_kernels.h"

namespace dml::core::dispatch
{
    static auto gs_mem_move          = dml_ref_mem_move;
    static auto gs_fill_u64          = dml_ref_fill_u64;
    static auto gs_compare           = dml_ref_compare;
    static auto gs_compare_pattern   = dml_ref_compare_pattern;
    static auto gs_create_delta      = dml_ref_create_delta;
    static auto gs_apply_delta       = dml_ref_apply_delta;
    static auto gs_dualcast          = dml_ref_dualcast;
    static auto gs_crc_u32           = dml_ref_crc_32u;
    static auto gs_crc_reflected_u32 = dml_ref_crc_reflected_u32;
    static auto gs_cache_flush       = dml_clflush;
    static auto gs_cache_write_back  = dml_clwb_unsupported;

    class dispatcher
    {
    public:
        dispatcher() noexcept
        {

            auto registers = dml_core_cpuid(DML_CPUID_EXTENSIONS);

            if ((registers.ebx & DML_AVX512_MASK) == DML_AVX512_MASK)
            {
                gs_mem_move        = dml_avx512_mem_move;
                gs_fill_u64        = dml_avx512_fill_u64;
                gs_compare         = dml_avx512_compare;
                gs_compare_pattern = dml_avx512_compare_pattern;
                gs_crc_u32         = dml_avx512_crc_u32;
                gs_crc_reflected_u32 = dml_avx512_crc_reflected_u32;
            }

            if ((registers.ebx & DML_CLFLUSHOPT) == DML_CLFLUSHOPT)
            {
                gs_cache_flush = dml_clflushopt;
            }

            if ((registers.ebx & DML_CLWB) == DML_CLWB)
            {
                gs_cache_write_back = dml_clwb;
            }
        }
    };

    [[maybe_unused]] static auto gs_dispatcher = dispatcher();

    void mem_move(const uint8_t* src, uint8_t* dst, uint32_t transfer_size) noexcept
    {
        gs_mem_move(src, dst, transfer_size);
    }

    void fill(uint64_t pattern, uint8_t* dst, uint32_t transfer_size) noexcept
    {
        gs_fill_u64(pattern, dst, transfer_size);
    }

    std::tuple<uint32_t, uint8_t> compare(const uint8_t* src1, const uint8_t* src2, uint32_t transfer_size) noexcept
    {
        uint8_t result   = 0;
        auto    mismatch = gs_compare(src1, src2, transfer_size, &result);

        return { mismatch, result };
    }

    std::tuple<uint32_t, uint8_t> compare_pattern(uint64_t pattern, const uint8_t* src, uint32_t transfer_size) noexcept
    {
        uint8_t result   = 0;
        auto    mismatch = gs_compare_pattern(pattern, src, transfer_size, &result);

        return { mismatch, result };
    }

    std::tuple<uint32_t, uint8_t> create_delta(const uint8_t* src1,
                                               const uint8_t* src2,
                                               uint32_t       transfer_size,
                                               uint8_t*       delta_record,
                                               uint32_t       delta_max_size) noexcept
    {
        uint8_t result            = 0;
        auto    delta_record_size = gs_create_delta(src1, src2, transfer_size, delta_record, delta_max_size, &result);

        return { delta_record_size, result };
    }

    void apply_delta(const uint8_t* delta_record, uint8_t* dst, uint32_t transfer_size) noexcept
    {
        gs_apply_delta(delta_record, dst, transfer_size);
    }

    void dualcast(const uint8_t* src, uint8_t* dst1, uint8_t* dst2, uint32_t transfer_size) noexcept
    {
        gs_dualcast(src, dst1, dst2, transfer_size);
    }

    uint32_t crc(const uint8_t* src, uint32_t transfer_size, uint32_t crc_seed, uint32_t polynomial) noexcept
    {
        return gs_crc_u32(src, transfer_size, crc_seed, polynomial);
    }

    uint32_t crc_reflected(const uint8_t* src, uint32_t transfer_size, uint32_t crc_seed, uint32_t polynomial) noexcept
    {
        return gs_crc_reflected_u32(src, transfer_size, crc_seed, polynomial);
    }

    void cache_flush(uint8_t* dst, uint32_t transfer_size) noexcept
    {
        gs_cache_flush(dst, transfer_size);
    }

    void cache_write_back(uint8_t* dst, uint32_t transfer_size) noexcept
    {
        gs_cache_write_back(dst, transfer_size);
    }
}  // namespace dml::core::dispatch
