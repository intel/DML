/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <benchmark/benchmark.h>

#include <ops/ops.hpp>
#include <data_providers.hpp>
#include <utility.hpp>
#include <measure.hpp>

using namespace bench;

template <execution_e exec, api_e api, path_e path>
class move_t
{
public:
    static constexpr auto exec_v = exec;
    static constexpr auto api_v  = api;
    static constexpr auto path_v = path;

    void operator()(benchmark::State &state, const case_params_t &common_params, const data_t &data, bool copy_mode) const
    {
        try
        {
            // Prepare compression
            ops::move_params_t params(data.buffer, copy_mode);
            std::vector<ops::move_t<api, path, custom_allocator_t<>, custom_allocator_t<>>> operations;

            // Measuring loop
            auto stat = measure<exec, path>(state, common_params, operations, params);

            // Validation
            for (auto &operation : operations)
            {
                for(auto &result: operation.get_result().data_)
                {
                    if(data.buffer != result)
                        throw std::runtime_error("move_t::operator(): Verification failed");
                }
            }

            // Set counters
            base_counters(state, stat);
        }
        catch(std::runtime_error &err) { state.SkipWithError(err.what()); }
        catch(...)                     { state.SkipWithError("move_t::operator(): unknown exception"); }
    }
};

template <path_e path>
static inline void cases_set(case_params_t &common_params, data_t &data)
{
    if(path != path_e::cpu && cmd::FLAGS_no_hw)
        return;
    if(path == path_e::cpu && get_mem_cc(common_params.out_mem_))
        return;

    for(std::uint32_t copy = 0; copy <= 1; ++copy)
    {
        common_params.full_time_ = false;
        if(path == path_e::cpu)
        {
            register_benchmarks_common((copy)?"move":"copy", to_name(data.buffer.size(), "size"), move_t<execution_e::sync,  api_e::glibc, path_e::cpu>{}, common_params, data, copy);
            register_benchmarks_common((copy)?"move":"copy", to_name(data.buffer.size(), "size"), move_t<execution_e::async,  api_e::glibc, path_e::cpu>{}, common_params, data, copy);
        }

        for(std::uint32_t full_time = 0; full_time <= 1; ++full_time)
        {
            common_params.full_time_ = full_time;
            register_benchmarks_common((copy)?"move":"copy", to_name(data.buffer.size(), "size"), move_t<execution_e::sync,  api_e::c,     path>{}, common_params, data, copy);
            register_benchmarks_common((copy)?"move":"copy", to_name(data.buffer.size(), "size"), move_t<execution_e::async, api_e::c,     path>{}, common_params, data, copy);
            // Using if statement on get_mem_cc while cpp api cache control disabling is not supported 
            if(get_mem_cc(common_params.out_mem_)){register_benchmarks_common((copy)?"move":"copy", to_name(data.buffer.size(), "size"), move_t<execution_e::sync,  api_e::cpp,   path>{}, common_params, data, copy);}
            if(get_mem_cc(common_params.out_mem_)){register_benchmarks_common((copy)?"move":"copy", to_name(data.buffer.size(), "size"), move_t<execution_e::async, api_e::cpp,   path>{}, common_params, data, copy);}
        }
    }
}

struct task_set
{
    size_t       size{0};
    std::int32_t queue_size{0};
    std::int32_t batch_size{0};
};

BENCHMARK_SET_DELAYED(move)
{
    std::vector<size_t>         sizes        = (cmd::get_block_size() >= 0) ? std::vector<size_t>{(size_t)cmd::get_block_size()} : std::vector<size_t>{1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216};
    std::vector<std::int32_t>   queue_sizes  = (cmd::FLAGS_queue_size > 0)  ? std::vector<std::int32_t>{cmd::FLAGS_queue_size} : std::vector<std::int32_t>{16, 32, 64, 128};
    std::vector<std::int32_t>   batch_sizes  = (cmd::FLAGS_batch_size >= 0) ? std::vector<std::int32_t>{cmd::FLAGS_batch_size} : std::vector<std::int32_t>{0, 4, 8, 16};
    // std::vector<mem_loc_e>      mem_locs_in  = std::vector<mem_loc_e>{mem_loc_e::none, mem_loc_e::l1, mem_loc_e::l2, mem_loc_e::llc, mem_loc_e::ram};
    // std::vector<mem_loc_e>      mem_locs_out = std::vector<mem_loc_e>{mem_loc_e::none, mem_loc_e::l1, mem_loc_e::l2, mem_loc_e::llc, mem_loc_e::ram, mem_loc_e::cc_none, mem_loc_e::cc_l1, mem_loc_e::cc_l2, mem_loc_e::cc_llc, mem_loc_e::cc_ram};

    std::vector<mem_loc_e>      mem_locs_in  = std::vector<mem_loc_e>{cmd::get_in_mem()};
    std::vector<mem_loc_e>      mem_locs_out = std::vector<mem_loc_e>{cmd::get_out_mem()};

    for(auto &size : sizes)
    {
        auto data = data::gen_data(data::gen_params_t{size, data::fill_params_t{}});

        for(auto queue_size : queue_sizes)
        {
            for(auto batch_size : batch_sizes)
            {
                for(auto mem_loc_in : mem_locs_in)
                {
                    for(auto mem_loc_out : mem_locs_out)
                    {
                        case_params_t common_params{};
                        common_params.in_mem_     = mem_loc_in;
                        common_params.out_mem_    = mem_loc_out;
                        common_params.queue_size_ = queue_size;
                        common_params.batch_size_ = batch_size;

                        cases_set<path_e::dsa>(common_params, data);
                        cases_set<path_e::cpu>(common_params, data);
                    }
                }
            }
        }
    }
}
