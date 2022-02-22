/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_CORE_OWN_KERNELS_DEFS_H
#define DML_CORE_OWN_KERNELS_DEFS_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void dml_ref_mem_move(const uint8_t *src, uint8_t *dst, uint32_t transfer_size);

void dml_avx512_mem_move(const uint8_t *src, uint8_t *dst, uint32_t transfer_size);

void dml_ref_fill_u64(uint64_t pattern, uint8_t *dst, uint32_t transfer_size);

void dml_avx512_fill_u64(uint64_t pattern, uint8_t *dst, uint32_t transfer_size);

uint32_t dml_ref_compare(const uint8_t *src1, const uint8_t *src2, uint32_t transfer_size, uint8_t *result);

uint32_t dml_avx512_compare(const uint8_t *src1, const uint8_t *src2, uint32_t transfer_size, uint8_t *result);

uint32_t dml_ref_compare_pattern(uint64_t pattern, const uint8_t *src, uint32_t transfer_size, uint8_t *result);

uint32_t dml_avx512_compare_pattern(uint64_t pattern, const uint8_t *src, uint32_t transfer_size, uint8_t *result);

uint32_t dml_ref_create_delta(const uint8_t *src1,
                              const uint8_t *src2,
                              uint32_t       transfer_size,
                              uint8_t       *delta_record,
                              uint32_t       max_delta_record_size,
                              uint8_t       *result);

void dml_ref_apply_delta(const uint8_t *delta_record, uint8_t *dst, uint32_t delta_record_size);

void dml_ref_dualcast(const uint8_t *src, uint8_t *dst1, uint8_t *dst2, uint32_t transfer_size);

uint32_t dml_ref_crc_32u(const uint8_t *src, uint32_t transfer_size, uint32_t crc_value, uint32_t polynomial);

uint32_t dml_avx512_crc_u32(const uint8_t *src, uint32_t transfer_size, uint32_t crc_value, uint32_t polynomial);

uint32_t dml_ref_crc_reflected_u32(const uint8_t *src, uint32_t transfer_size, uint32_t crc_value, uint32_t polynomial);

uint32_t dml_avx512_crc_reflected_u32(const uint8_t* src, uint32_t transfer_size, uint32_t crc_value, uint32_t polynomial);

void dml_clflushopt(uint8_t *dst, uint32_t transfer_size);

void dml_clflush(uint8_t *dst, uint32_t transfer_size);

void dml_clwb(uint8_t *dst, uint32_t transfer_size);

void dml_clwb_unsupported(uint8_t *dst, uint32_t transfer_size);

#ifdef __cplusplus
}
#endif

#endif  // DML_CORE_OWN_KERNELS_DEFS_H
