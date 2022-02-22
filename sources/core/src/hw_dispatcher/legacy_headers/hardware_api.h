/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @date 3/16/2020
 *
 * @defgroup HW_PATH_API General Hardware API
 * @ingroup HW_PATH
 * @{
 * @brief Contains general hardware function declarations
 */
#include "hardware_completion_records_api.h"
#include "hardware_definitions.h"
#include "hardware_descriptors_api.h"
#include "hardware_limits.h"

#ifndef DML_OWN_HW_API_H__
#define DML_OWN_HW_API_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes and returns hardware context
 *
 * @param[in,out] hw_context_ptr  pointer to pointer to assign the hardware context
 *
 * @return Follow statuses:
 *      - @todo Add statuses
 */
dml_status_t DML_HW_API(get_context)(dsahw_context_t **hw_context_ptr);

/**
 * @brief Calls an operation, which is implemented with DSA hardware
 *
 * @param[in] hw_state_ptr   pointer to memory allocated for hardware state
 * @param[in] descriptor_ptr pointer to the descriptor for the execution
 * @param[in] flags          specific operation flags
 *
 * @return @ref dml_status_t in according to specified DSA operation in @ref dml_job_t
 *
 */
dml_status_t DML_HW_API(submit)(const dsahw_context_t *hw_state_ptr, const dsahw_descriptor_t *descriptor_ptr, dml_operation_flags_t flags);

/**
 * @brief Closes connection with hardware
 *
 * @param[in] hw_context_ptr    pointer to pointer to hardware context
 *
 * @return The one of the follow statuses:
 *      - @ref DML_STATUS_OK;
 *
 */
dml_status_t DML_HW_API(finalize)(dsahw_context_t *hw_context_ptr);

/**
 * @brief Returns value of OverlappingCopySupport from GENCAP
 *
 * @note Returns false in case of an error occurred (hw init fail)
 *
 * @return True if Overlapping copy is supported, false otherwise.
 *
 */
int DML_HW_API(get_overlapping_copy_support)(dsahw_context_t *hw_context_ptr);

#ifdef __cplusplus
}
#endif

#endif  //DML_OWN_HW_API_H__

/** @} */
