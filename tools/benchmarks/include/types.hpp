/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_BENCHMARKS_TYPES_HPP
#define DML_BENCHMARKS_TYPES_HPP

#include <cstdint>

namespace bench
{
struct data_t
{
    std::string               name;
    std::vector<std::uint8_t> buffer;
};

using dataset_t = std::vector<data_t>;
using blocks_t  = std::vector<data_t>;

struct canned_table_t
{
    std::vector<std::uint8_t> com_table_l1;
    std::vector<std::uint8_t> com_table_l3;
    std::vector<std::uint8_t> dec_table_l1;
    std::vector<std::uint8_t> dec_table_l3;

    std::vector<std::uint8_t> com_table_l1_ho;
    std::vector<std::uint8_t> com_table_l3_ho;
    std::vector<std::uint8_t> dec_table_l1_ho;
    std::vector<std::uint8_t> dec_table_l3_ho;
};

struct dictionary_t
{
    std::vector<std::uint8_t> dict;
};

struct statistics_t
{
    std::uint32_t   queue_size{0};
    std::uint32_t   operations{0};
    std::uint32_t   operations_per_thread{0};
    std::uint64_t   completed_operations{0};
    std::uint64_t   data_read{0};
    std::uint64_t   data_written{0};
};

enum class api_e
{
    cpp,
    c,
    glibc
};

enum class path_e
{
    cpu,
    dsa,
    auto_
};

enum class execution_e
{
    sync,
    async
};

enum class batch_e
{
    yes,
    no
};

enum class operation_e
{
    move
};

enum class ratio_type_e
{
    none,
    to_result,
    to_source
};

enum class huffman_type_e
{
    fixed,
    static_,
    dynamic,
    canned
};

enum class task_status_e
{
    retired,
    in_progress,
    completed
};

enum class mem_loc_e: std::uint32_t
{
    cc_flag = 0xF0,
    none    = 0x00,
    l1      = 0x01,
    l2      = 0x02,
    llc     = 0x04,
    ram     = 0x08,
    pmem    = 0x0F,
    cc_none = none|cc_flag,
    cc_l1   = l1|cc_flag,
    cc_l2   = l2|cc_flag,
    cc_llc  = llc|cc_flag,
    cc_ram  = ram|cc_flag,
    cc_pmem = pmem|cc_flag
};

enum class mem_loc_mask_e: std::uint32_t
{
    src1 = 0x01,
    src2 = 0x02,
    dst1 = 0x04,
    dst2 = 0x08,
    src  = src1|src2,
    dst  = dst1|dst2,
    all  = src|dst
};

static inline std::uint32_t operator& (mem_loc_mask_e lha, mem_loc_mask_e rha)
{
    return static_cast<std::uint32_t>(lha)&static_cast<std::uint32_t>(rha);
}

static inline std::uint32_t operator& (mem_loc_mask_e lha, std::uint32_t rha)
{
    return static_cast<std::uint32_t>(lha)&rha;
}

static inline std::uint32_t operator| (mem_loc_mask_e lha, mem_loc_mask_e rha)
{
    return static_cast<std::uint32_t>(lha)|static_cast<std::uint32_t>(rha);
}

static inline std::uint32_t operator| (mem_loc_mask_e lha, std::uint32_t rha)
{
    return static_cast<std::uint32_t>(lha)|rha;
}
}

#endif //DML_BENCHMARKS_TYPES_HPP
