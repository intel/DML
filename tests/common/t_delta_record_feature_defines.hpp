/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Common definitions for delta record feature testing
 * @date 2/25/2020
 *
 */

#ifndef DML_T_DELTA_RECORD_FEATURE_DEFINES_HPP__
#define DML_T_DELTA_RECORD_FEATURE_DEFINES_HPP__

#include "t_common.hpp"
#include "t_random_parameters.hpp"
#include "t_range.hpp"

// Delta Record Feature types
typedef   uint16_t offset_t;
typedef   uint64_t pattern_t;

// Delta Record's Note format
constexpr uint32_t DELTA_DATA_FIELD_SIZE   = sizeof(pattern_t);
constexpr uint32_t DELTA_OFFSET_FIELD_SIZE = sizeof(uint16_t);
constexpr uint32_t DELTA_NOTE_BYTE_SIZE    = DELTA_DATA_FIELD_SIZE + DELTA_OFFSET_FIELD_SIZE;

// Delta Record Feature limitations
constexpr uint32_t MAX_MEMORY_REGION_SIZE  = 0x7FFF8u;

// Bad argument tests vector sizes
constexpr uint32_t DELTA_CHUNK_COUNT       = 100u;
constexpr uint32_t TEST_ARRAY_SIZE         = DELTA_CHUNK_COUNT * DELTA_DATA_FIELD_SIZE;
constexpr uint32_t TEST_DELTA_RECORD_SIZE  = DELTA_CHUNK_COUNT * (DELTA_DATA_FIELD_SIZE + DELTA_OFFSET_FIELD_SIZE);

// Algorithm tests vector size
constexpr dml::test::range_t<uint32_t> REGIONS_COUNT = {TEST_MIN_MEMORY_BLOCK_SIZE / DELTA_DATA_FIELD_SIZE,
                                                        TEST_MAX_MEMORY_BLOCK_SIZE / DELTA_DATA_FIELD_SIZE};

#endif //DML_T_DELTA_RECORD_FEATURE_DEFINES_HPP__
