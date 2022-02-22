/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_ML_SW_PATH_LEGACY_DIF_H
#define DML_ML_SW_PATH_LEGACY_DIF_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* job_t;

int dml_legacy_dif_check(job_t dml_job_ptr);

int dml_legacy_dif_insert(job_t dml_job_ptr);

int dml_legacy_dif_strip(job_t dml_job_ptr);

int dml_legacy_dif_update(job_t dml_job_ptr);

#ifdef __cplusplus
}
#endif

#endif  // DML_ML_SW_PATH_LEGACY_DIF_H
