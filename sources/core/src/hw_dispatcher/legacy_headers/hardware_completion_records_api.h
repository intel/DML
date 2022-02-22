/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @date 3/16/2020
 *
 * @defgroup HW_PATH_COMPLETION_RECORDS_API Completion records readers API
 * @ingroup HW_PATH
 * @{
 * @brief Contains functions that are used to read Intel(R) Data Streaming Accelerator Completion record
 */
#include "hardware_definitions.h"

#ifndef DML_HW_COMPLETION_RECORDS_API_H__
#define DML_HW_COMPLETION_RECORDS_API_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Extracts operation results from @ref dsahw_completion_record_t
 * and fills the @ref dml_job_t structure in accordance with the @ref DML_OP_MEM_MOVE operation
 *
 * @param[in]  completion_record_ptr  pointer to the completion record with the operation result
 * @param[out] result_ptr             pointer to the result of the operation
 * @param[out] elements_processed_ptr pointer to the total processed elements
 *
 * @return Result of executed operation:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR;
 *      - @ref DML_STATUS_JOB_CORRUPTED;
 *      - @ref DML_STATUS_PAGE_FAULT_ERROR;
 *      - @ref DML_STATUS_OVERLAPPING_BUFFER_ERROR;
 *
 */
dsahw_status_t DML_HW_API(get_mem_move_result)(const dsahw_completion_record_t *completion_record_ptr,
                                               dml_meta_result_t               *result_ptr,
                                               uint32_t                        *elements_processed_ptr);

/**
 * @brief Extracts operation results from @ref dsahw_completion_record_t
 * and fills the @ref dml_job_t structure in accordance with the @ref DML_OP_FILL operation
 *
 * @param[in]  completion_record_ptr  pointer to the completion record with the operation result
 * @param[out] elements_processed_ptr pointer to the total processed elements
 *
 * @return Result of executed operation:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR;
 *      - @ref DML_STATUS_PAGE_FAULT_ERROR;
 *
 */
dsahw_status_t DML_HW_API(get_fill_result)(const dsahw_completion_record_t *completion_record_ptr, uint32_t *elements_processed_ptr);

/**
 * @brief Extracts operation results from @ref dsahw_completion_record_t
 * and fills the @ref dml_job_t structure in accordance with the compare operations
 * (@ref DML_OP_COMPARE and @ref DML_OP_COMPARE_PATTERN)
 *
 * @param[in]  completion_record_ptr  pointer to the completion record with the operation result
 * @param[out] result_ptr             pointer to the result of the operation
 * @param[out] elements_processed_ptr pointer to the total processed elements
 *
 * @return Result of executed operation:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_FALSE_PREDICATE_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR;
 *      - @ref DML_STATUS_PAGE_FAULT_ERROR;
 *
 */
dsahw_status_t DML_HW_API(get_compare_result)(const dsahw_completion_record_t *completion_record_ptr,
                                              dml_meta_result_t               *result_ptr,
                                              uint32_t                        *elements_processed_ptr);

/**
 * @brief Extracts operation results from @ref dsahw_completion_record_t
 * and fills the @ref dml_job_t structure in accordance with the compare operations
 * DML_OP_DELTA_CREATE
 *
 * @param[in]  completion_record_ptr   pointer to the completion record with the operation result
 * @param[out] destination_ptr         pointer to the destination memory buffer
 * @param[out] delta_record_length_ptr pointer to the created delta record length
 * @param[out] result_ptr              pointer to the result of the operation
 * @param[out] elements_processed_ptr  pointer to the total processed elements
 *
 * @return Result of executed operation:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR;
 *      - @ref DML_STATUS_DELTA_ALIGN_ERROR;
 *      - @ref DML_STATUS_DELTA_RECORD_SIZE_ERROR;
 *      - @ref DML_STATUS_DELTA_INPUT_SIZE_ERROR;
 *      - @ref DML_STATUS_DELTA_OFFSET_ERROR.
 *
 */
dsahw_status_t DML_HW_API(get_delta_create_result)(const dsahw_completion_record_t *completion_record_ptr,
                                                   const uint8_t                   *destination_ptr,
                                                   uint32_t                        *delta_record_length_ptr,
                                                   dml_meta_result_t               *result_ptr,
                                                   uint32_t                        *elements_processed_ptr);

/**
 * @brief Extracts operation results from @ref dsahw_completion_record_t
 * and fills the @ref dml_job_t structure in accordance with the compare operations
 * DML_OP_DELTA_APPLY
 *
 * @param[in]  completion_record_ptr  pointer to the completion record with the operation result
 * @param[out] elements_processed_ptr pointer to the total processed elements
 *
 * @return Result of executed operation:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR;
 *      - @ref DML_STATUS_DELTA_ALIGN_ERROR;
 *      - @ref DML_STATUS_DELTA_RECORD_SIZE_ERROR;
 *      - @ref DML_STATUS_OVERLAPPING_BUFFER_ERROR.
 *
 */
dsahw_status_t DML_HW_API(get_delta_apply_result)(const dsahw_completion_record_t *completion_record_ptr, uint32_t *elements_processed_ptr);

/**
 * @brief Extracts operation results from @ref dsahw_completion_record_t
 * and fills the @ref dml_job_t structure in accordance with the @ref DML_OP_DUALCAST operation
 *
 * @param[in]  completion_record_ptr  pointer to the completion record with the operation result
 * @param[out] elements_processed_ptr pointer to the total processed elements
 *
 * @return Result of executed operation:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR;
 *      - @ref DML_STATUS_PAGE_FAULT_ERROR;
 *      - @ref DML_STATUS_DUALCAST_ALIGN_ERROR;
 *      - @ref DML_STATUS_OVERLAPPING_BUFFER_ERROR
 *
 */
dsahw_status_t DML_HW_API(get_dualcast_result)(const dsahw_completion_record_t *completion_record_ptr, uint32_t *elements_processed_ptr);

/**
 * @brief Extracts operation results from @ref dsahw_completion_record_t
 * and fills the @ref dml_job_t structure in accordance with the @ref DML_OP_CRC operation
 *
 * @param[in]  completion_record_ptr  pointer to the completion record with the operation result
 * @param[out] crc_result_ptr         pointer to the result of the operation
 * @param[out] elements_processed_ptr pointer to the total processed elements
 *
 * @return Result of executed operation:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR;
 *      - @ref DML_STATUS_PAGE_FAULT_ERROR;
 *        @todo: Add alignment error
 *
 */
dsahw_status_t DML_HW_API(get_crc_result)(const dsahw_completion_record_t *completion_record_ptr,
                                          uint32_t                        *crc_result_ptr,
                                          uint32_t                        *elements_processed_ptr);

/**
 * @brief Extracts operation results from @ref dsahw_completion_record_t
 * and fills the @ref dml_job_t structure in accordance with the @ref DML_OP_COPY_CRC operation
 *
 * @param[in]  completion_record_ptr  pointer to the completion record with the operation result
 * @param[out] crc_result_ptr         pointer to the result of the operation
 * @param[out] elements_processed_ptr pointer to the total processed elements
 *
 * @return Result of executed operation:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR;
 *      - @ref DML_STATUS_PAGE_FAULT_ERROR;
 *      - @ref DML_STATUS_OVERLAPPING_BUFFER_ERROR;
 *        @todo: Add alignment error
 *
 */
dsahw_status_t DML_HW_API(get_crc_copy_result)(const dsahw_completion_record_t *completion_record_ptr,
                                               uint32_t                        *crc_result_ptr,
                                               uint32_t                        *elements_processed_ptr);

/**
 * @brief Extracts operation results from @ref dsahw_completion_record_t
 * and fills the @ref dml_job_t structure in accordance with the @ref DML_OP_CACHE_FLUSH operation
 *
 * @param[in]  completion_record_ptr  pointer to the completion record with the operation result
 * @param[out] elements_processed_ptr pointer to the total processed elements
 *
 * @return Result of executed operation:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR;
 *      - @ref DML_STATUS_PAGE_FAULT_ERROR;
 *
 */
dsahw_status_t DML_HW_API(get_cache_flush_result)(const dsahw_completion_record_t *completion_record_ptr, uint32_t *elements_processed_ptr);

/**
 * @brief Extracts operation results from @ref dsahw_completion_record_t
 * and fills the @ref dml_job_t structure in accordance with the @ref DML_OP_DIF_CHECK operation
 *
 * @param[in]  completion_record_ptr  pointer to the completion record with the operation result
 * @param[out] result_ptr             pointer to the result of the operation
 * @param[out] dif_config_ptr         pointer to the dif config to update
 * @param[out] elements_processed_ptr pointer to the total processed elements
 *
 * @return Result of executed operation:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR;
 *      - @ref DML_STATUS_PAGE_FAULT_ERROR;
 *        @ref DML_STATUS_DIF_CHECK_ERROR.
 *
 */
dsahw_status_t DML_HW_API(get_check_dif_result)(const dsahw_completion_record_t *completion_record_ptr,
                                                dml_meta_result_t               *result_ptr,
                                                dml_dif_config_t                *dif_config_ptr,
                                                uint32_t                        *elements_processed_ptr);

/**
 * @brief Extracts operation results from @ref dsahw_completion_record_t
 * and fills the @ref dml_job_t structure in accordance with the @ref DML_OP_DIF_INSERT operation
 *
 * @param[in]  completion_record_ptr  pointer to the completion record with the operation result
 * @param[out] dif_config_ptr         pointer to the dif config to update
 * @param[out] elements_processed_ptr pointer to the total processed elements
 *
 * @return Result of executed operation:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR;
 *      - @ref DML_STATUS_PAGE_FAULT_ERROR;
 *      - @ref DML_STATUS_OVERLAPPING_BUFFER_ERROR;
 *      - @ref DML_STATUS_JOB_LENGTH_ERROR;
 *
 */
dsahw_status_t DML_HW_API(get_insert_dif_result)(const dsahw_completion_record_t *completion_record_ptr,
                                                 dml_dif_config_t                *dif_config_ptr,
                                                 uint32_t                        *elements_processed_ptr);

/**
 * @brief Extracts operation results from @ref dsahw_completion_record_t
 * and fills the @ref dml_job_t structure in accordance with the @ref DML_OP_DIF_STRIP operation
 *
 * @param[in]  completion_record_ptr  pointer to the completion record with the operation result
 * @param[out] dif_config_ptr         pointer to the dif config to update
 * @param[out] elements_processed_ptr pointer to the total processed elements
 *
 * @return Result of executed operation:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR;
 *      - @ref DML_STATUS_PAGE_FAULT_ERROR;
 *      - @ref DML_STATUS_OVERLAPPING_BUFFER_ERROR;
 *      - @ref DML_STATUS_JOB_LENGTH_ERROR;
 *
 */
dsahw_status_t DML_HW_API(get_strip_dif_result)(const dsahw_completion_record_t *completion_record_ptr,
                                                dml_dif_config_t                *dif_config_ptr,
                                                uint32_t                        *elements_processed_ptr);

/**
 * @brief Extracts operation results from @ref dsahw_completion_record_t
 * and fills the @ref dml_job_t structure in accordance with the @ref DML_OP_DIF_UPDATE operation
 *
 * @param[in]  completion_record_ptr  pointer to the completion record with the operation result
 * @param[out] dif_config_ptr         pointer to the dif config to update
 * @param[out] result_ptr             pointer to the result of the operation
 * @param[out] elements_processed_ptr pointer to the total processed elements
 *
 * @return Result of executed operation:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR;
 *      - @ref DML_STATUS_PAGE_FAULT_ERROR;
 *      - @ref DML_STATUS_OVERLAPPING_BUFFER_ERROR;
 *      - @ref DML_STATUS_JOB_LENGTH_ERROR;
 *
 */
dsahw_status_t DML_HW_API(get_update_dif_result)(const dsahw_completion_record_t *completion_record_ptr,
                                                 dml_dif_config_t                *dif_config_ptr,
                                                 dml_meta_result_t               *result_ptr,
                                                 uint32_t                        *elements_processed_ptr);

/**
 * @brief Extracts operation results from @ref dsahw_completion_record_t
 * and fills the @ref dml_job_t structure in accordance with the @ref DML_OP_NOP operation
 *
 * @param[in] completion_record_ptr pointer to the completion record with the operation result
 *
 * @return Result of executed operation:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR;
 *
 */
dsahw_status_t DML_HW_API(get_nop_result)(const dsahw_completion_record_t *completion_record_ptr);

/**
 * @brief Extracts operation results from @ref dsahw_completion_record_t
 * and fills the @ref dml_job_t structure in accordance with the @ref DML_OP_BATCH operation
 *
 * @param[in]  completion_record_ptr     pointer to the completion record with the operation result
 * @param[out] descriptors_processed_ptr number of processed tasks
 *
 * @return Result of executed operation:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_BATCH_SIZE_ERROR;
 *      - @ref DML_STATUS_BATCH_ERROR;
 *
 */
dsahw_status_t DML_HW_API(get_batch_result)(const dsahw_completion_record_t *completion_record_ptr, uint32_t *descriptors_processed_ptr);

/**
 * @brief Extracts operation results from @ref dsahw_completion_record_t
 * and fills the @ref dml_job_t structure in accordance with the @ref DML_OP_DRAIN operation
 *
 * @param[in]  completion_record_ptr     pointer to the completion record with the operation result
 *
 * @return Result of executed operation:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_DRAIN_PAGE_FAULT_ERROR;
 *
 */
dsahw_status_t DML_HW_API(get_drain_result)(const dsahw_completion_record_t *completion_record_ptr);

#ifdef __cplusplus
}
#endif

#endif  //DML_HW_COMPLETION_RECORDS_API_H__

/** @} */
