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
 * @brief Contains an implementation of @ref dsa_get_operation_result
 * @date 3/26/2020
 *
 */

/**
 * @brief Extracts operation results from @ref own_dml_completion_record_t
 * and fills the @ref dml_job_t structure in accordance with specified job operation
 *
 * @param[in]   completion_record_ptr    pointer to the completion record with the operation result
 * @param[out]  dml_job_ptr              pointer to the job specified by user
 *
 * @return Result of executed operation:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR;
 *      - @ref DML_STATUS_JOB_CORRUPTED;
 *      - or other status depending on the DML operation in the @ref dml_job_t.operation field .
 */
OWN_FUN_INLINE(dml_status_t, hw_get_operation_result, (const dsahw_completion_record_t *completion_record_ptr,
                                                       dsahw_completion_record_t *batch_results_ptr,
                                                       dml_job_t *dml_job_ptr))
{
    switch (dml_job_ptr->operation)
    {
        case DML_OP_MEM_MOVE:

            return dsa_get_mem_move_result(completion_record_ptr,
                                           &dml_job_ptr->result,
                                           &dml_job_ptr->offset);

        case DML_OP_FILL:

            return dsa_get_fill_result(completion_record_ptr, &dml_job_ptr->offset);

        case DML_OP_NOP:

            return dsa_get_nop_result(completion_record_ptr);
            
        case DML_OP_BATCH:
        {
            uint32_t results_count      = dml_job_ptr->destination_length / OWN_BATCH_TASK_SIZE;
            own_batch_task_t *tasks_ptr = (own_batch_task_t *)&dml_job_ptr->destination_first_ptr[0];

            for (uint32_t i = 0u; i < results_count; ++i)
            {
                tasks_ptr[i].status = idml_hw_get_operation_result(&batch_results_ptr[i],
                                                                   NULL,
                                                                   &tasks_ptr[i].dml_job);
            }
            
            return dsa_get_batch_result(completion_record_ptr,
                                        &dml_job_ptr->offset);
        }

        case DML_OP_DRAIN:

            return dsa_get_drain_result(completion_record_ptr);

        case DML_OP_COMPARE:
        case DML_OP_COMPARE_PATTERN:

            return dsa_get_compare_result(completion_record_ptr,
                                          &dml_job_ptr->result,
                                          &dml_job_ptr->offset);

        case DML_OP_DELTA_CREATE:

            return dsa_get_delta_create_result(completion_record_ptr,
                                               dml_job_ptr->destination_first_ptr,
                                               &dml_job_ptr->destination_length,
                                               &dml_job_ptr->result,
                                               &dml_job_ptr->offset);

        case DML_OP_DELTA_APPLY:

            return dsa_get_delta_apply_result(completion_record_ptr, &dml_job_ptr->offset);

        case DML_OP_DUALCAST:

            return dsa_get_dualcast_result(completion_record_ptr, &dml_job_ptr->offset);

        case DML_OP_CRC:

            return dsa_get_crc_result(completion_record_ptr,
                                      dml_job_ptr->crc_checksum_ptr,
                                      &dml_job_ptr->offset);

        case DML_OP_COPY_CRC:

            return dsa_get_crc_copy_result(completion_record_ptr,
                                           dml_job_ptr->crc_checksum_ptr,
                                           &dml_job_ptr->offset);

        case DML_OP_CACHE_FLUSH:

            return dsa_get_cache_flush_result(completion_record_ptr, &dml_job_ptr->offset);

        case DML_OP_DIF_CHECK:

            return dsa_get_check_dif_result(completion_record_ptr,
                                            &dml_job_ptr->result,
                                            &dml_job_ptr->dif_config,
                                            &dml_job_ptr->offset);

        case DML_OP_DIF_INSERT:

            return dsa_get_insert_dif_result(completion_record_ptr,
                                             &dml_job_ptr->dif_config,
                                             &dml_job_ptr->offset);

        case DML_OP_DIF_STRIP:

            return dsa_get_strip_dif_result(completion_record_ptr,
                                            &dml_job_ptr->dif_config,
                                            &dml_job_ptr->offset);

        case DML_OP_DIF_UPDATE:

            return dsa_get_update_dif_result(completion_record_ptr,
                                             &dml_job_ptr->dif_config,
                                             &dml_job_ptr->result,
                                             &dml_job_ptr->offset);

        default:

            return DML_STATUS_JOB_OPERATION_ERROR;
    }
}
