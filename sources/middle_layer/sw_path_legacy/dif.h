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

#ifndef DML_ML_SW_PATH_LEGACY_DIF_H
#define DML_ML_SW_PATH_LEGACY_DIF_H

#include <dml/dmldefs.h>

#ifdef __cplusplus
extern "C" {
#endif

dml_status_t dml_legacy_dif_check(dml_job_t* dml_job_ptr);

dml_status_t dml_legacy_dif_insert(dml_job_t* dml_job_ptr);

dml_status_t dml_legacy_dif_strip(dml_job_t* dml_job_ptr);

dml_status_t dml_legacy_dif_update(dml_job_t* dml_job_ptr);

#ifdef __cplusplus
}
#endif
#endif  // DML_ML_SW_PATH_LEGACY_DIF_H
