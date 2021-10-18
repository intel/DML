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
 * @brief Contains an implementation of @ref dsa_init_descriptor function
 * @date 3/16/2020
 *
 */

/**
 * @brief Fills a descriptor for a specified in the @ref dml_job_t.operation field operation
 *
 * @param[in]  dml_job_ptr     pointer to the job specified by user
 * @param[in]  result_ptr      pointer to the memory that will contain operation result after descriptor execution
 * @param[out] descriptor_ptr  pointer to @ref dsa_descriptor_t
 *
 * @return The following statuses:
 * @todo add return statuses
 *
 */
OWN_FUN_INLINE(dml_status_t, hw_init_descriptor, (dml_job_t *dml_job_ptr,
                                                  dsahw_completion_record_t *result_ptr,
                                                  own_dml_hw_batch_buffer_t *batch_bundle_ptr,
                                                  dsahw_descriptor_t *descriptor_ptr))
{
    switch (dml_job_ptr->operation)
    {
        case DML_OP_MEM_MOVE:

            return dsa_init_mem_move_descriptor(descriptor_ptr,
                                                  dml_job_ptr->source_first_ptr,
                                                  dml_job_ptr->source_length,
                                                  dml_job_ptr->destination_first_ptr,
                                                  dml_job_ptr->flags,
                                                  result_ptr);

        case DML_OP_FILL:

            return dsa_init_fill_descriptor(descriptor_ptr,
                                              (uint64_t *)(&dml_job_ptr->pattern[0]),
                                              dml_job_ptr->destination_first_ptr,
                                              dml_job_ptr->destination_length,
                                              dml_job_ptr->flags,
                                              result_ptr);

        case DML_OP_NOP:

            return dsa_init_nop_descriptor(descriptor_ptr, dml_job_ptr->flags, result_ptr);

        case DML_OP_BATCH:
        {
            uint32_t descriptors_count                   = dml_job_ptr->destination_length / OWN_BATCH_TASK_SIZE;
            dsahw_descriptor_t *batch_descriptors_ptr    = &batch_bundle_ptr->descriptors_ptr[0];
            dsahw_completion_record_t *batch_results_ptr = &batch_bundle_ptr->results_ptr[0];
            own_batch_task_t *tasks_ptr                  = (own_batch_task_t *)&dml_job_ptr->destination_first_ptr[0];

            for (uint32_t i = 0u; i < descriptors_count; ++i)
            {
                if ((DML_OP_BATCH == tasks_ptr[i].dml_job.operation) ||
                    (DML_OP_DRAIN == tasks_ptr[i].dml_job.operation))
                {
                    return DML_STATUS_JOB_OPERATION_ERROR;
                }

                dml_status_t status = idml_hw_init_descriptor(&tasks_ptr[i].dml_job,
                                                              &batch_results_ptr[i], 
                                                              NULL,
                                                              &batch_descriptors_ptr[i]);
                if (DML_STATUS_OK != status)
                {
                    return status;
                }
            }

            return dsa_init_batch_descriptor(descriptor_ptr,
                                             &batch_descriptors_ptr[0],
                                             descriptors_count,
                                             dml_job_ptr->flags,
                                             result_ptr);
        }            

        case DML_OP_DRAIN:

            return dsa_init_drain_descriptor(descriptor_ptr,
                                             (uint64_t *)dml_job_ptr->destination_first_ptr,
                                             (uint64_t *)dml_job_ptr->destination_second_ptr,
                                             dml_job_ptr->flags,
                                             result_ptr);

        case DML_OP_COMPARE:

            return dsa_init_compare_descriptor(descriptor_ptr,
                                               dml_job_ptr->source_first_ptr,
                                               dml_job_ptr->source_second_ptr,
                                               dml_job_ptr->source_length,
                                               dml_job_ptr->expected_result,
                                               dml_job_ptr->flags,
                                               result_ptr);

        case DML_OP_COMPARE_PATTERN:

            return dsa_init_compare_pattern_descriptor(descriptor_ptr,
                                                       dml_job_ptr->source_first_ptr,
                                                       dml_job_ptr->source_length,
                                                       (uint64_t *)&dml_job_ptr->pattern[0],
                                                       dml_job_ptr->expected_result,
                                                       dml_job_ptr->flags,
                                                       result_ptr);

        case DML_OP_DELTA_CREATE:

            return dsa_init_delta_create_descriptor(descriptor_ptr,
                                                    dml_job_ptr->source_first_ptr,
                                                    dml_job_ptr->source_second_ptr,
                                                    dml_job_ptr->source_length,
                                                    dml_job_ptr->destination_first_ptr,
                                                    dml_job_ptr->destination_length,
                                                    dml_job_ptr->expected_result,
                                                    dml_job_ptr->flags,
                                                    result_ptr);

        case DML_OP_DELTA_APPLY:

            return dsa_init_delta_apply_descriptor(descriptor_ptr,
                                                   dml_job_ptr->source_first_ptr,
                                                   dml_job_ptr->source_length,
                                                   dml_job_ptr->destination_first_ptr,
                                                   dml_job_ptr->destination_length,
                                                   dml_job_ptr->flags,
                                                   result_ptr);

        case DML_OP_DUALCAST:

            return dsa_init_dualcast_descriptor(descriptor_ptr,
                                                dml_job_ptr->source_first_ptr,
                                                dml_job_ptr->source_length,
                                                dml_job_ptr->destination_first_ptr,
                                                dml_job_ptr->destination_second_ptr,
                                                dml_job_ptr->flags,
                                                result_ptr);

        case DML_OP_CRC:

            return dsa_init_crc_descriptor(descriptor_ptr,
                                           dml_job_ptr->source_first_ptr,
                                           dml_job_ptr->source_length,
                                           dml_job_ptr->crc_checksum_ptr,
                                           dml_job_ptr->flags,
                                           result_ptr);

        case DML_OP_COPY_CRC:

            return dsa_init_crc_copy_descriptor(descriptor_ptr,
                                                dml_job_ptr->source_first_ptr,
                                                dml_job_ptr->source_length,
                                                dml_job_ptr->crc_checksum_ptr,
                                                dml_job_ptr->destination_first_ptr,
                                                dml_job_ptr->flags,
                                                result_ptr);

        case DML_OP_CACHE_FLUSH:

            return dsa_init_cache_flush_descriptor(descriptor_ptr,
                                                   dml_job_ptr->destination_first_ptr,
                                                   dml_job_ptr->destination_length,
                                                   dml_job_ptr->flags,
                                                   result_ptr);

        case DML_OP_DIF_CHECK:

            return dsa_init_check_dif_descriptor(descriptor_ptr,
                                                 dml_job_ptr->source_first_ptr,
                                                 dml_job_ptr->source_length,
                                                 &dml_job_ptr->dif_config,
                                                 dml_job_ptr->flags,
                                                 result_ptr);

        case DML_OP_DIF_INSERT:

            return dsa_init_insert_dif_descriptor(descriptor_ptr,
                                                  dml_job_ptr->source_first_ptr,
                                                  dml_job_ptr->source_length,
                                                  &dml_job_ptr->dif_config,
                                                  dml_job_ptr->destination_first_ptr,
                                                  dml_job_ptr->flags,
                                                  result_ptr);

        case DML_OP_DIF_STRIP:

            return dsa_init_strip_dif_descriptor(descriptor_ptr,
                                                 dml_job_ptr->source_first_ptr,
                                                 dml_job_ptr->source_length,
                                                 &dml_job_ptr->dif_config,
                                                 dml_job_ptr->destination_first_ptr,
                                                 dml_job_ptr->flags,
                                                 result_ptr);

        case DML_OP_DIF_UPDATE:

            return dsa_init_update_dif_descriptor(descriptor_ptr,
                                                  dml_job_ptr->source_first_ptr,
                                                  dml_job_ptr->source_length,
                                                  &dml_job_ptr->dif_config,
                                                  dml_job_ptr->destination_first_ptr,
                                                  dml_job_ptr->flags,
                                                  result_ptr);

        default:

            return DML_STATUS_JOB_OPERATION_ERROR;
    }
}
