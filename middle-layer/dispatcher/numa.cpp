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

#include <array>
#include <fstream>
#include <unordered_map>

#if defined(linux)
    #include <x86intrin.h>
#endif

#include "numa.hpp"

namespace dml::ml::util {

static constexpr auto get_nodes_configuration() -> std::array<std::pair<const char *, int32_t>, 10> {
    return {
            {
                    {R"(/sys/bus/node/devices/node0/cpulist)", 0},
                    {R"(/sys/bus/node/devices/node1/cpulist)", 1},
                    {R"(/sys/bus/node/devices/node2/cpulist)", 2},
                    {R"(/sys/bus/node/devices/node3/cpulist)", 3},
                    {R"(/sys/bus/node/devices/node4/cpulist)", 4},
                    {R"(/sys/bus/node/devices/node5/cpulist)", 5},
                    {R"(/sys/bus/node/devices/node6/cpulist)", 6},
                    {R"(/sys/bus/node/devices/node7/cpulist)", 7},
                    {R"(/sys/bus/node/devices/node8/cpulist)", 8},
                    {R"(/sys/bus/node/devices/node9/cpulist)", 9}
            }
    };
}

static inline void get_region(std::ifstream &stream,
                              uint32_t &begin,
                              uint32_t &end,
                              bool read_end_splitter = false) {
    char splitter = '0';

    stream >> begin;
    stream >> splitter;
    stream >> end;

    if (read_end_splitter) {
        stream >> splitter;
    }
}

static inline void update_regions(std::unordered_map<uint32_t, int32_t> &regions,
                                  uint32_t begin,
                                  uint32_t end,
                                  int32_t value) {
    for (uint32_t i = begin; i <= end; i++) {
        regions[i] = value;
    }
}

class numa_configuration {
public:
    numa_configuration(const numa_configuration &other) = delete;

    auto operator=(const numa_configuration &other) -> numa_configuration & = delete;

    numa_configuration(numa_configuration &&other) = delete;

    auto operator=(numa_configuration &&other) -> numa_configuration & = delete;

    static auto get_instance() noexcept -> numa_configuration & {
        static numa_configuration inst{};

        return inst;
    }

    auto operator[](uint32_t cpu_id) noexcept -> int32_t {
        if (mapping_.find(cpu_id) != mapping_.end()) {
            return mapping_[cpu_id];
        } else {
            return 0;
        }
    }

private:
    numa_configuration() noexcept {
        constexpr auto node_lists = get_nodes_configuration();

        for (auto path : node_lists) {
            std::ifstream file(path.first);

            if (!file.is_open()) {
                continue;
            }

            uint32_t begin = 0;
            uint32_t end   = 0;

            get_region(file, begin, end, true);
            update_regions(mapping_, begin, end, path.second);

            get_region(file, begin, end);
            update_regions(mapping_, begin, end, path.second);
        }
    }

    std::unordered_map<uint32_t, int32_t> mapping_;
};

auto get_cpu_id() -> uint32_t {
    uint32_t cpu_id = -1;

#if defined(linux)
    __rdtscp(&cpu_id);
#endif

    return cpu_id;
}

int32_t get_numa_id() noexcept {
#if defined(linux)
    static auto &numa_config = numa_configuration::get_instance();

    static thread_local auto cpu_id  = get_cpu_id();
    static thread_local auto numa_id = numa_config[cpu_id];

    return numa_id;
#else
    // Not supported in Windows yet
    return -1;
#endif
}

}
