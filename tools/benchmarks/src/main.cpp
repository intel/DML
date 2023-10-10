/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <benchmark/benchmark.h>
#include <utility.hpp>

#include <dml/dml.h>
#include "../include/cmd_decl.hpp"
#include "hw_dispatcher.hpp"

#if defined( __linux__ )
#include <sys/utsname.h>
#endif
#include <fstream>
#include <memory>
#include <mutex>
#include <stdarg.h>


namespace bench::details
{
//
// Utilities implementations
//
registry_t& get_registry()
{
    static registry_t reg;
    return reg;
}

static bool init_hw()
{
    uint32_t size;

    auto status = dml_get_job_size(DML_PATH_HW, &size);
    if (status != DML_STATUS_OK)
        throw std::runtime_error(format("bench::details::init_hw: dml_get_job_size() failed with status %d", status));

    std::unique_ptr<std::uint8_t[]> job_buffer(new std::uint8_t[size]);

    dml_job_t *job = reinterpret_cast<dml_job_t*>(job_buffer.get());
    status = dml_init_job(DML_PATH_HW, job);
    if (status != DML_STATUS_OK)
        throw std::runtime_error(format("bench::details::init_hw: dml_init_job() failed with status %d", status));

    int data1 = 0;
    int data2 = 0;
    job->source_first_ptr      = (std::uint8_t*)&data1;
    job->source_length         = 4;
    job->destination_first_ptr = (std::uint8_t*)&data2;
    job->destination_length    = 4;
    job->operation             = DML_OP_MEM_MOVE;

    status = dml_submit_job(job);
    if(status != DML_STATUS_OK)
        throw std::runtime_error(format("bench::details::init_hw: dml_submit_job() failed with status %d", status));

    status = dml_wait_job(job, DML_WAIT_MODE_BUSY_POLL);
    if(status != DML_STATUS_OK)
        throw std::runtime_error(format("bench::details::init_hw: dml_wait_job() failed with status %d", status));

    status = dml_finalize_job(job);
    if (status != DML_STATUS_OK)
        throw std::runtime_error(format("bench::details::init_hw: dml_finalize_job() failed with status %d", status));

    return true;
}

static inline std::uint32_t get_num_devices(std::uint32_t numa) noexcept
{
    auto &disp = dml::core::dispatcher::hw_dispatcher::get_instance();
    int counter = 0;
    for(auto &device : disp)
    {
        if(device.numa_id() == numa)
            counter++;
    }
    return counter;
}

std::uint32_t get_current_numa() noexcept
{
    std::uint32_t tsc_aux = 0;
    __rdtscp(&tsc_aux);

    return static_cast<std::uint32_t>(tsc_aux >> 12);
}

std::uint32_t get_current_numa_accels() noexcept
{
    return get_num_devices(get_current_numa());
}

static inline std::int32_t get_cpu_index(const extended_info_t &info, std::int32_t thread_index)
{
    std::uint32_t devices  = info.accelerators.total_devices;
    if(!devices)
        devices = 1; // If no devices avaliable juts pin threads with step 1
    return ((thread_index%devices)*info.cpu_physical_per_cluster+thread_index/devices)%info.cpu_physical_per_socket + info.cpu_physical_per_socket*get_current_numa();
}

void set_affinity_map(const benchmark::State &state)
{
    static thread_local bool is_set{false};
    if(!is_set)
    {
        auto cpu_index = get_cpu_index(get_sys_info(), state.thread_index());

        cpu_set_t cpus;
        CPU_ZERO(&cpus);
        CPU_SET(cpu_index, &cpus);
        pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpus);

        is_set = true;
    }
}

const extended_info_t& get_sys_info()
{
    static extended_info_t info;
    static bool is_setup{false};
    static std::mutex guard;

    guard.lock();
    if(!is_setup)
    {
#if defined( __linux__ )
        utsname uname_buf;
        uname(&uname_buf);
        info.host_name = uname_buf.nodename;
        info.kernel    = uname_buf.release;

        std::ifstream info_file("/proc/cpuinfo");
        if(!info_file.is_open()){
            guard.unlock();
            throw std::runtime_error("bench::details::get_sys_info: failed to open /proc/cpuinfo");
        }
        std::string line;
        while (std::getline(info_file, line))
        {
            if (line.empty())
                continue;
            auto del_index = line.find(':');
            if(del_index == std::string::npos)
                continue;
            auto key = line.substr(0, del_index);
            auto val = line.substr(del_index+1);
            trim(key);
            trim(val);

            // Start of descriptor
            if(key == "processor")
                info.cpu_logical_cores++;
            else if(key == "physical id")
                info.cpu_sockets = std::max(info.cpu_sockets, (std::uint32_t)atoi(val.c_str())+1);
            else if(!info.cpu_physical_per_socket && key == "cpu cores")
                info.cpu_physical_per_socket = std::max(info.cpu_physical_per_socket, (std::uint32_t)atoi(val.c_str()));
            else if(!info.cpu_model_name.size() && key == "model name")
                info.cpu_model_name = val;
            else if(!info.cpu_model && key == "model")
                info.cpu_model = atoi(val.c_str());
            else if(!info.cpu_microcode && key == "microcode")
                info.cpu_microcode = strtol(val.c_str(), NULL, 16);
            else if(!info.cpu_stepping && key == "stepping")
                info.cpu_stepping = atoi(val.c_str());
        }

        constexpr std::uint32_t clusters_per_socket = 4; // How to get this dynamically?
        info.cpu_physical_cores       = info.cpu_physical_per_socket*info.cpu_sockets;
        info.cpu_physical_per_cluster = info.cpu_physical_per_socket/clusters_per_socket;

        for(std::uint32_t i = 0; i < info.cpu_sockets; ++i)
        {
            auto devices = get_num_devices(i);
            info.accelerators.total_devices += devices;
            info.accelerators.socket.push_back(devices);
        }

        printf("== Host:   %s\n", info.host_name.c_str());
        printf("== Kernel: %s\n", info.kernel.c_str());
        printf("== CPU:    %s (%d)\n", info.cpu_model_name.c_str(), info.cpu_model);
        printf("  --> Microcode: 0x%x\n", info.cpu_microcode);
        printf("  --> Stepping:  %d\n", info.cpu_stepping);
        printf("  --> Logical:   %d\n", info.cpu_logical_cores);
        printf("  --> Physical:  %d\n", info.cpu_physical_cores);
        printf("  --> Socket:    %d\n", info.cpu_physical_per_socket);
        printf("  --> Cluster:   %d\n", info.cpu_physical_per_cluster);
        printf("== Accelerators: %d\n", info.accelerators.total_devices);
        for(std::uint32_t i = 0; i < info.accelerators.socket.size(); ++i)
        {
            printf("  --> NUMA %d: %d\n", i, info.accelerators.socket[i]);
        }
        printf("== Affinity Map [device_index: thread_index(cpu_index)...]:\n");
        printf("  --> ");
        for(std::uint32_t i = 0; i < info.cpu_physical_per_cluster; ++i)
        {
            auto cpu = get_cpu_index(info, i);
            printf("%d(%d) ", i, cpu);
        }
        printf("\n");
#endif
        is_setup = true;
    }
    guard.unlock();

    return info;
}
}

//
// GBench command line extension
//
namespace bench::cmd
{
BM_DEFINE_string(block_size, "-1");
BM_DEFINE_int32(queue_size, 0);
BM_DEFINE_int32(batch_size, 0);
BM_DEFINE_int32(threads, 0);
BM_DEFINE_int32(node, -1);
BM_DEFINE_string(dataset, "");
BM_DEFINE_string(in_mem, "ram");
BM_DEFINE_string(out_mem, "cc_def");
BM_DEFINE_bool(full_time, false);
BM_DEFINE_bool(no_hw, false);

static void print_help()
{
    fprintf(stdout,
            "Common arguments:\n"
            "benchmark [--dataset=<path>]            - path to generic dataset\n"
            "          [--block_size=<size>]         - size of data block for testing\n"
            "          [--queue_size=<size>]         - number of operations to submit for each iteration. Each operation processes block_size*batch_size of data\n"
            "          [--batch_size=<size>]         - size of batch in operation. 0 - not batched operation. Each task in batch processes block_size of data\n"
            "                                          Total transfer size per iteration will be: block_size*queue_size*batch_size\n"
            "          [--threads=<num>]             - number of threads for asynchronous measurements.\n"
            "                                          For accelerator this means number of concurrent submission threads\n"
            "                                          For CPU this means number of parallel executions\n"
            "                                          Each thread works on queue_size/threads operations\n"
            "          [--node=<num>]                - force specific numa node for the task\n"
            "          [--in_mem=<location>]         - input memory location: def, l1, l2, llc, ram (default).\n"
            "          [--out_mem=<location>]        - output memory location: def, l1, l2, llc, ram and same with cc_ (cache control) prefix. Default: cc_def\n"
            "          [--full_time]                 - measure library specific task initialization and destruction\n"
            "          [--no_hw]                     - run only software implementations\n"

            "\nDefault benchmark arguments:\n");
}

static void parse_local(int* argc, char** argv)
{
    for(int i = 1; argc && i < *argc; ++i)
    {
        if(benchmark::ParseStringFlag(argv[i],  "dataset",      &FLAGS_dataset) ||
           benchmark::ParseStringFlag(argv[i],  "block_size",   &FLAGS_block_size) ||
           benchmark::ParseInt32Flag(argv[i],   "threads",      &FLAGS_threads) ||
           benchmark::ParseInt32Flag(argv[i],   "node",         &FLAGS_node) ||
           benchmark::ParseBoolFlag(argv[i],    "full_time",    &FLAGS_full_time) ||
           benchmark::ParseInt32Flag(argv[i],   "queue_size",   &FLAGS_queue_size) ||
           benchmark::ParseInt32Flag(argv[i],   "batch_size",   &FLAGS_batch_size) ||
           benchmark::ParseBoolFlag(argv[i],    "no_hw",        &FLAGS_no_hw) ||
           benchmark::ParseStringFlag(argv[i],  "in_mem",       &FLAGS_in_mem) ||
           benchmark::ParseStringFlag(argv[i],  "out_mem",      &FLAGS_out_mem))
        {
            for(int j = i; j != *argc - 1; ++j)
                argv[j] = argv[j + 1];

            --(*argc);
            --i;
        }
        else if (benchmark::IsFlag(argv[i], "help"))
            print_help();
    }
}

std::int32_t get_block_size()
{
    static std::int32_t block_size = -1;
    if(block_size < 0)
    {
        auto str = FLAGS_block_size;
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);

        std::int32_t mult = 1;
        if((str.size() > 2 && str.find("KB", str.size()-2) == str.size()-2) || (str.size() > 1 && str.find("K", str.size()-1) == str.size()-1))
            mult = 1024;
        else if((str.size() > 2 && str.find("MB", str.size()-2) == str.size()-2) || (str.size() > 1 && str.find("M", str.size()-1) == str.size()-1))
            mult = 1024*1024;

        block_size = std::atoi(str.c_str());
        if(block_size == 0 && str != "0")
            throw std::runtime_error("bench::cmd::get_block_size: invalid block size format");
        block_size *= mult;
    }
    return block_size;
}

mem_loc_e get_in_mem()
{
    static mem_loc_e mem = (mem_loc_e)-1;
    if((std::int32_t)mem < 0)
    {
        auto str = FLAGS_in_mem;
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        if(str == "none")
            mem = mem_loc_e::none;
        else if(str == "l1")
            mem = mem_loc_e::l1;
        else if(str == "l2")
            mem = mem_loc_e::l2;
        else if(str == "llc")
            mem = mem_loc_e::llc;
        else if(str == "ram")
            mem = mem_loc_e::ram;
        else if(str == "pmem")
            mem = mem_loc_e::pmem;
        else
            throw std::runtime_error("bench::cmd::get_in_mem: invalid input memory location");
    }
    return mem;
}

mem_loc_e get_out_mem()
{
    static mem_loc_e mem = (mem_loc_e)-1;
    if((std::int32_t)mem < 0)
    {
        auto str = FLAGS_out_mem;
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        if(str == "def")
            mem = mem_loc_e::none;
        else if(str == "l1")
            mem = mem_loc_e::l1;
        else if(str == "l2")
            mem = mem_loc_e::l2;
        else if(str == "llc")
            mem = mem_loc_e::llc;
        else if(str == "ram")
            mem = mem_loc_e::ram;
        else if(str == "pmem")
            mem = mem_loc_e::pmem;
        else if(str == "cc_def")
            mem = mem_loc_e::cc_none;
        else if(str == "cc_l1")
            mem = mem_loc_e::cc_l1;
        else if(str == "cc_l2")
            mem = mem_loc_e::cc_l2;
        else if(str == "cc_llc")
            mem = mem_loc_e::cc_llc;
        else if(str == "cc_ram")
            mem = mem_loc_e::cc_ram;
        else if(str == "cc_pmem")
            mem = mem_loc_e::cc_pmem;
        else
            throw std::runtime_error("bench::cmd::get_out_mem: invalid output memory location");
    }
    return mem;
}
}

namespace bench
{
std::string format(const char *format, ...) noexcept
{
    std::string out;
    size_t      size;

    va_list argptr1, argptr2;
    va_start(argptr1, format);
    va_copy(argptr2, argptr1);
    size = vsnprintf(NULL, 0, format, argptr1);
    va_end(argptr1);

    out.resize(size+1);
    vsnprintf(out.data(), out.size(), format, argptr2);
    va_end(argptr2);
    out.resize(out.size()-1);

    return out;
}
}

//
// Main
//

int main(int argc, char** argv)
{
    bench::cmd::parse_local(&argc, argv);
    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    bench::details::get_sys_info();

    if(!bench::cmd::FLAGS_no_hw)
        bench::details::init_hw();

    auto &registry = bench::details::get_registry();
    for(auto &reg : registry)
        reg();
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();
    return 0;
}
