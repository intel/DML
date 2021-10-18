/*
 * Copyright 2020-2021 Intel Corporation.
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

/**
 * @date 3/3/2020
 *
 * @defgroup SW_PATH Software Path API
 * @ingroup dml_job_private
 *
 * @defgroup HW_PATH Hardware Path API
 * @ingroup dml_job_private
 *
 */

#include "own_dml_definitions.h"
#include "own_dml_internal_state.h"

#if defined(DML_HW)
    #include "hardware_api.h"
#endif

#ifndef DML_OWN_DML_API_HPP__
#define DML_OWN_DML_API_HPP__


/**
 * @addtogroup SW_PATH
 * @{
 * @brief Software implementation of the Intel(R) Data Streaming Accelerator Features.
 *
 */


OWN_API(uint32_t, sw_get_state_size, ())


/**
 * @brief Calls a library function implemented for CPU that enables a necessary DML feature.
 *
 * @param[in,out] dml_job_ptr  pointer on to job specified by user
 *
 * @return @ref dml_status_t depending on the DML operation in the @ref dml_job_t.operation field
 *
 */
OWN_API(dml_status_t, sw_submit_job, (dml_job_t *const dml_job_ptr))


/** @} */

#endif //DML_OWN_DML_API_HPP__
