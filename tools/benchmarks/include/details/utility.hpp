/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_BENCHMARKS_DETAILS_UTILITY_HPP
#define DML_BENCHMARKS_DETAILS_UTILITY_HPP

#include "../cmd_decl.hpp"

#ifdef __linux__
#include <time.h>
#endif
#include <types.hpp>
#include <x86intrin.h>
#include <vector>


//
// Defines
//
#define ERROR(M, ...) fprintf(stderr, "[ERROR] " M "\n", ##__VA_ARGS__)
#define ASSERT(C, M, ...) if (!(C)) { ERROR(M, ##__VA_ARGS__); exit(EXIT_FAILURE); }
#define ASSERT_NM(C) if (!(C)) { exit(EXIT_FAILURE); }

#define BENCHMARK_SET_DELAYED_INT(UNIQUE_REG, UNIQUE_SING) \
void UNIQUE_REG(); \
class UNIQUE_SING \
{ \
public: \
UNIQUE_SING() \
    { \
        auto &reg = bench::details::get_registry(); \
        reg.push_back(UNIQUE_REG); \
    } \
}; \
static UNIQUE_SING BENCHMARK_PRIVATE_NAME(_local_register_instance_); \
void UNIQUE_REG()

namespace bench
{
static inline mem_loc_e get_mem_no_cc(mem_loc_e mem);
}

namespace bench::details
{
//
// Registration utils
//

using registry_call_t = void(*)();
using registry_t      = std::vector<registry_call_t>;
registry_t& get_registry();

//
// Registration utils
//

struct accel_info_t
{
    std::uint32_t              total_devices = 0;
    std::vector<std::uint32_t> socket;
};

struct extended_info_t
{
    std::string   host_name;
    std::string   kernel;
    std::uint32_t cpu_model                = 0;
    std::string   cpu_model_name;
    std::uint32_t cpu_stepping             = 0;
    std::uint32_t cpu_microcode            = 0;
    std::uint32_t cpu_logical_cores        = 0;
    std::uint32_t cpu_physical_cores       = 0;
    std::uint32_t cpu_sockets              = 0;
    std::uint32_t cpu_physical_per_socket  = 0;
    std::uint32_t cpu_physical_per_cluster = 0;
    accel_info_t  accelerators;
};

const extended_info_t& get_sys_info();
std::uint32_t get_current_numa() noexcept;
std::uint32_t get_current_numa_accels() noexcept;
void set_affinity_map(const benchmark::State &state) noexcept;

constexpr std::uint64_t submitRetryWaitNs = 0;

inline void retry_sleep()
{
    if constexpr (submitRetryWaitNs == 0)
        return;
    else
    {
#ifdef __linux__
    timespec spec { 0, submitRetryWaitNs };
    nanosleep(&spec, nullptr);
#else
#endif
    }
}

template <typename RangeT>
inline void mem_control(RangeT begin, RangeT end, mem_loc_e mem_loc, bool source_mem) noexcept
{
    std::uint8_t *begin_ptr      = (std::uint8_t*)&(*begin);
    std::uint8_t *end_ptr        = (std::uint8_t*)&(*end);
    auto          mem_loc_no_cc  = get_mem_no_cc(mem_loc);
    auto          always_flushed = (mem_loc_no_cc == mem_loc_e::ram || mem_loc_no_cc == mem_loc_e::pmem);

    if(mem_loc_no_cc == mem_loc_e::none)
        return;

    if(source_mem || always_flushed)
    {
        for (auto line = begin_ptr; line < end_ptr; line += 64u)
            _mm_clflushopt(line);
        _mm_mfence();
    }

    if(!always_flushed)
    {
        for (auto line = begin_ptr; line < end_ptr; line += 64u)
        {
            if(mem_loc_no_cc == mem_loc_e::l1)
                _mm_prefetch(line, _MM_HINT_T0);
            else if(mem_loc_no_cc == mem_loc_e::l2)
                _mm_prefetch(line, _MM_HINT_T1);
            else if(mem_loc_no_cc == mem_loc_e::llc)
            {
                _mm_prefetch(line, _MM_HINT_T2);
                // CLDEMOTE
                //asm volatile(".byte 0x0f, 0x1c, 0x07\t\n" :: "D" (line));
            }
        }
        _mm_mfence();
    }
}
}

#endif //DML_BENCHMARKS_DETAILS_UTILITY_HPP
