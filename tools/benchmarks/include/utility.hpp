/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_BENCHMARKS_UTILITY_HPP
#define DML_BENCHMARKS_UTILITY_HPP

#include <cmd_decl.hpp>
#include <details/utility.hpp>

//
// Defines
//
// This macro provides delayed registration for benchmark cases which depend on cmd arguments or file names.
// Cases still can be registered via native Google Benchmark interfaces
#define BENCHMARK_SET_DELAYED(NAME) \
BENCHMARK_SET_DELAYED_INT(BENCHMARK_PRIVATE_NAME(_local_register_##NAME), BENCHMARK_PRIVATE_NAME(_local_register_singleton_##NAME))

namespace bench
{
//
// Custom Allocator Template
//
template <typename ElemT = std::uint8_t>
class custom_allocator_t
{
public:
    using value_type        = ElemT;
    using size_type         = std::size_t;
    using difference_type   = std::ptrdiff_t;

    template <typename OtherElemT>
    struct rebind { using other = custom_allocator_t<OtherElemT>; };

    custom_allocator_t() = default;

    custom_allocator_t(const custom_allocator_t &) = default;

    template <typename OtherElemT>
    constexpr custom_allocator_t(const custom_allocator_t<OtherElemT>&) noexcept {}

    custom_allocator_t& operator=(custom_allocator_t &) = default;

    custom_allocator_t& operator=(custom_allocator_t &&other) = default;

    template <typename ElemOtherT>
    bool operator==(const custom_allocator_t<ElemOtherT> &rhl) const
    {
        return true;
    }

    template <typename ElemOtherT>
    bool operator!=(const custom_allocator_t<ElemOtherT> &rhl) const
    {
        return false;
    }

    [[nodiscard]] ElemT* allocate(std::size_t size)
    {
        if(size > std::numeric_limits<std::size_t>::max() / sizeof(ElemT))
            throw std::bad_array_new_length();

        auto ptr = static_cast<ElemT*>(std::malloc(size*sizeof(ElemT)));
        if(!ptr)
            throw std::bad_alloc();
        return ptr;
    }

    void deallocate(ElemT* ptr, std::size_t) noexcept
    {
        std::free(ptr);
    }
};

//
// Vectors extension
//
template<typename TypeL, typename TypeR, typename AllocatorL, typename AllocatorR>
bool operator==( const std::vector<TypeL, AllocatorL>& lhs,
                 const std::vector<TypeR, AllocatorR>& rhs)
{
    if(lhs.size() != rhs.size())
        return false;
    for(size_t i = 0; i < lhs.size(); ++i)
    {
        if(lhs[i] != rhs[i])
            return false;
    }
    return true;
}

template<typename TypeL, typename TypeR, typename AllocatorL, typename AllocatorR>
bool operator!=( const std::vector<TypeL, AllocatorL>& lhs,
                 const std::vector<TypeR, AllocatorR>& rhs)
{
    return !(lhs==rhs);
}

//
// Common string operations
//
std::string format(const char *format, ...) noexcept;

static inline void trim_left(std::string &str)
{
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char val) { return !std::isspace(val); }));
}

static inline void trim_right(std::string &str)
{
    str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char val) { return !std::isspace(val); }).base(), str.end());
}

static inline void trim(std::string &str)
{
    trim_left(str);
    trim_right(str);
}

template <typename ArgT>
static inline std::string to_string(ArgT arg)
{
    return std::to_string(arg);
}
template <typename ArgT>
static inline std::string to_name(ArgT arg, std::string name)
{
    return std::string("/") + name + ":" + to_string(arg);
}
static inline std::string to_string(api_e api)
{
    switch(api)
    {
    case api_e::cpp:    return "cpp";
    case api_e::c:      return "c";
    case api_e::glibc:  return "glibc";
    default:            return "error";
    }
}
static inline std::string to_name(api_e api)
{
    return std::string("/api:") + to_string(api);
}

static inline std::string to_string(path_e path)
{
    switch(path)
    {
    case path_e::cpu:    return "cpu";
    case path_e::dsa:    return "dsa";
    case path_e::auto_:  return "auto";
    default:             return "error";
    }
}
static inline std::string to_name(path_e path)
{
    return std::string("/path:") + to_string(path);
}
static inline std::string to_string(execution_e exec)
{
    switch(exec)
    {
    case execution_e::sync:  return "sync";
    case execution_e::async: return "async";
    default:         return "error";
    }
}
static inline std::string to_name(execution_e exec)
{
    return std::string("/exec:") + to_string(exec);
}
static inline std::string to_string(operation_e op)
{
    switch(op)
    {
    case operation_e::move: return "move";
    default:                return "error";
    }
}
static inline std::string level_to_name(std::int32_t level)
{
    return std::string("/lvl:") + std::to_string(level);
}
static inline std::string to_string(huffman_type_e huffman)
{
    switch(huffman)
    {
    case huffman_type_e::fixed:   return "fixed";
    case huffman_type_e::static_: return "static";
    case huffman_type_e::dynamic: return "dynamic";
    case huffman_type_e::canned:  return "canned";
    default:                      return "error";
    }
}
static inline std::string to_name(huffman_type_e huffman)
{
    return std::string("/huffman:") + to_string(huffman);
}
static inline std::string to_string(mem_loc_e loc)
{
    switch(loc)
    {
    case mem_loc_e::none:    return "def";
    case mem_loc_e::l1:      return "l1";
    case mem_loc_e::l2:      return "l2";
    case mem_loc_e::llc:     return "llc";
    case mem_loc_e::ram:     return "ram";
    case mem_loc_e::pmem:    return "pmem";
    case mem_loc_e::cc_none: return "cc_def";
    case mem_loc_e::cc_l1:   return "cc_l1";
    case mem_loc_e::cc_l2:   return "cc_l2";
    case mem_loc_e::cc_llc:  return "cc_llc";
    case mem_loc_e::cc_ram:  return "cc_ram";
    case mem_loc_e::cc_pmem: return "cc_pmem";
    default:                 return "error";
    }
}

//
// Common cases utils
//
using default_allocator_t = std::allocator<std::uint8_t>;

static inline bool get_mem_cc(mem_loc_e mem)
{
    return ((std::uint32_t)mem & (std::uint32_t)mem_loc_e::cc_flag);
}
static inline mem_loc_e get_mem_no_cc(mem_loc_e mem)
{
    return (mem_loc_e)((std::uint32_t)mem & ~(std::uint32_t)mem_loc_e::cc_flag);
}

template<execution_e exec>
static inline void base_arguments(benchmark::internal::Benchmark *b)
{
    if constexpr (exec == execution_e::async)
    {
        const std::vector<int> threads_vector{1, 2, 4, 8, 16};

        if(!cmd::FLAGS_threads)
        {
            for(auto threads : threads_vector)
            {
                b->Threads(threads);
            }
        }
        else
            b->Threads(cmd::FLAGS_threads);
    }

    b->UseRealTime();
    b->Unit(benchmark::kNanosecond);
}

template <typename LambdaT, typename... ArgsT>
static ::benchmark::internal::Benchmark* register_benchmark_proxy(const std::string name,
                                                                  LambdaT&&   fn,
                                                                  ArgsT&&...  args)
{
    return ::benchmark::RegisterBenchmark(name.c_str(), [fn = std::move(fn), args...](::benchmark::State& st) -> auto { return fn(st, args...); } );
}

struct case_params_t
{
    std::int32_t  numa_id_{cmd::FLAGS_node};
    bool          mem_control_{false};
    mem_loc_e     in_mem_{cmd::get_in_mem()};
    mem_loc_e     out_mem_{cmd::get_out_mem()};
    bool          full_time_{cmd::FLAGS_full_time};
    std::int32_t  queue_size_{cmd::FLAGS_queue_size};
    std::int32_t  batch_size_{cmd::FLAGS_batch_size};
};

template <typename CaseT, typename... ArgsT>
static ::benchmark::internal::Benchmark* register_benchmarks_common(const std::string   &case_name,
                                                                    const std::string   &case_name_ext,
                                                                    CaseT&&              case_functor,
                                                                    const case_params_t &case_common_params,
                                                                    const data_t        &data,
                                                                    ArgsT&&...           case_args)
{
    auto case_common_params_loc = case_common_params;

    std::string api_name   = to_name(case_functor.api_v);
    std::string exec_name  = to_name(case_functor.exec_v);
    std::string path_name  = to_name(case_functor.path_v);
    std::string in_mem     = to_name(case_common_params_loc.in_mem_, "in_mem");
    std::string out_mem    = to_name(case_common_params_loc.out_mem_, "out_mem");
    std::string timer_name;
    if(case_common_params_loc.full_time_)
        timer_name = "/timer:full";
    else
        timer_name = "/timer:proc";
    std::string qsize_name;
    if(case_common_params_loc.queue_size_ == 0)
    {
        if(case_functor.exec_v == execution_e::async)
            case_common_params_loc.queue_size_ = 128;
        else
            case_common_params_loc.queue_size_ = 1;
    }
    qsize_name = to_name(case_common_params_loc.queue_size_, "qsize");
    std::string bsize_name = to_name(case_common_params_loc.batch_size_, "bsize");
    std::string data_name  = (data.name.size()) ? std::string("/data:") + data.name : std::string{};
    std::string test_name  = case_name + api_name + path_name + exec_name + qsize_name + bsize_name + in_mem + out_mem + timer_name + data_name + case_name_ext;
    if(case_functor.exec_v == execution_e::async){
        return register_benchmark_proxy(test_name, std::forward<CaseT>(case_functor), case_common_params_loc, data, std::forward<ArgsT>(case_args)...)->Apply(base_arguments<execution_e::async>);
    }
    else{
        return register_benchmark_proxy(test_name, std::forward<CaseT>(case_functor), case_common_params_loc, data, std::forward<ArgsT>(case_args)...)->Apply(base_arguments<execution_e::sync>);
    }
}


template <typename ElemT, typename AllocatorT, typename SourceAllocatorT>
static void setup_buffers(const case_params_t &params, std::vector<std::vector<ElemT, AllocatorT>> &batch_vector, const std::vector<ElemT, SourceAllocatorT> &source_pattern)
{
    batch_vector.resize((params.batch_size_ > 0)?params.batch_size_:1);
    for(auto &batch : batch_vector)
    {
        batch.resize(source_pattern.size());
        for(size_t i = 0; i < batch.size(); ++i)
            batch[i] = source_pattern[i];
    }
}

template <typename ElemT, typename AllocatorT>
static void setup_buffers(const case_params_t &params, std::vector<std::vector<ElemT, AllocatorT>> &batch_vector, size_t vector_size)
{
    batch_vector.resize((params.batch_size_ > 0)?params.batch_size_:1);
    for(auto &batch : batch_vector)
        batch.resize(vector_size);
}


static inline void base_counters(benchmark::State &state, statistics_t &stat, ratio_type_e type = ratio_type_e::none)
{
    auto bytes_processed = static_cast<double>(stat.data_read);
    if(type == ratio_type_e::to_source)
    {
        state.counters["Ratio"]      = benchmark::Counter(static_cast<double>(stat.data_read) / static_cast<double>(stat.data_written),
                                                          benchmark::Counter::kAvgThreads);
    }
    else if(type == ratio_type_e::to_result)
    {
        state.counters["Ratio"]      = benchmark::Counter(static_cast<double>(stat.data_written) / static_cast<double>(stat.data_read),
                                                          benchmark::Counter::kAvgThreads);
        bytes_processed = static_cast<double>(stat.data_written);
    }
    state.counters["Throughput"] = benchmark::Counter(bytes_processed,
                                                      benchmark::Counter::kIsIterationInvariantRate|benchmark::Counter::kAvgThreads,
                                                      benchmark::Counter::kIs1000);

    state.counters["Latency/Op"] = benchmark::Counter(stat.operations_per_thread,
                                                      benchmark::Counter::kIsIterationInvariantRate|benchmark::Counter::kAvgThreads|benchmark::Counter::kInvert,
                                                      benchmark::Counter::kIs1000);

    state.counters["Latency"]    = benchmark::Counter(1,
                                                      benchmark::Counter::kIsIterationInvariantRate |benchmark::Counter::kAvgThreads|benchmark::Counter::kInvert,
                                                      benchmark::Counter::kIs1000);
}
}

#endif //DML_BENCHMARKS_UTILITY_HPP
