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
 * @brief Contains batch service functions
 * @date 9/8/2020
 *
 */
#include "dml/dml.h"
#include "own_dml_api.h"
#include "own_dml_batch.h"


#define OWN_BAD_ARGUMENT_INDEX_OVERFLOW(batch_size, index) DML_BAD_ARGUMENT_RETURN((batch_size / OWN_BATCH_TASK_SIZE) <= task_index, \
                                                                                   DML_STATUS_BATCH_TASK_INDEX_OVERFLOW)

static inline void reset_task(dml_job_t *const job_ptr)
{
    uint8_t *byte_ptr = (uint8_t *)(job_ptr);
    for (uint32_t i = 0u; i < sizeof(dml_job_t); i++)
    {
        byte_ptr[i] = 0u;
    }
}

DML_FUN(dml_status_t, dml_get_batch_size, (const dml_job_t *dml_job_ptr,
                                           uint32_t task_count,
                                           uint32_t *byte_size_ptr))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(byte_size_ptr)
    DML_BAD_ARGUMENT_RETURN(2u > task_count, DML_STATUS_BATCH_SIZE_ERROR)

    // return DML_STATUS_BATCH_SIZE_ERROR

    (*byte_size_ptr) = (OWN_BATCH_TASK_SIZE * task_count);

    return DML_STATUS_OK;
}


DML_FUN(dml_status_t, dml_batch_set_nop_by_index, (dml_job_t * dml_job_ptr,
                                                   uint32_t task_index,
                                                   dml_operation_flags_t flags))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)
    OWN_BAD_ARGUMENT_INDEX_OVERFLOW(dml_job_ptr->destination_length, task_index)

    const  uint32_t task_position       = (OWN_BATCH_TASK_SIZE * task_index);
    dml_job_t *operation_parameters_ptr = &OWN_BATCH_GET_TASK_BY_INDEX(dml_job_ptr, task_position)->dml_job;

    reset_task(operation_parameters_ptr);
    operation_parameters_ptr->operation = DML_OP_NOP;
    operation_parameters_ptr->flags     = flags;

    return DML_STATUS_OK;
}


DML_FUN(dml_status_t, dml_batch_set_mem_move_by_index, (dml_job_t * dml_job_ptr,
                                                        uint32_t task_index,
                                                        uint8_t *source_ptr,
                                                        uint8_t *destination_ptr,
                                                        uint32_t byte_length,
                                                        dml_operation_flags_t flags))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(source_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(destination_ptr)
    OWN_BAD_ARGUMENT_INDEX_OVERFLOW(dml_job_ptr->destination_length, task_index)

    const  uint32_t task_position       = (OWN_BATCH_TASK_SIZE * task_index);
    dml_job_t *operation_parameters_ptr = &OWN_BATCH_GET_TASK_BY_INDEX(dml_job_ptr, task_position)->dml_job;

    reset_task(operation_parameters_ptr);
    operation_parameters_ptr->source_first_ptr      = source_ptr;
    operation_parameters_ptr->source_length         = byte_length;
    operation_parameters_ptr->operation             = DML_OP_MEM_MOVE;
    operation_parameters_ptr->flags                 = flags;
    operation_parameters_ptr->destination_first_ptr = destination_ptr;
    operation_parameters_ptr->destination_length    = byte_length;

    return DML_STATUS_OK;
}


DML_FUN(dml_status_t, dml_batch_set_dualcast_by_index, (dml_job_t * dml_job_ptr,
                                                        uint32_t task_index,
                                                        uint8_t *source_ptr,
                                                        uint8_t *destination_first_ptr,
                                                        uint8_t *destination_second_ptr,
                                                        uint32_t byte_length,
                                                        dml_operation_flags_t flags))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(source_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(destination_first_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(destination_second_ptr)
    OWN_BAD_ARGUMENT_INDEX_OVERFLOW(dml_job_ptr->destination_length, task_index)

    const uint32_t task_position        = (OWN_BATCH_TASK_SIZE * task_index);
    dml_job_t *operation_parameters_ptr = &OWN_BATCH_GET_TASK_BY_INDEX(dml_job_ptr, task_position)->dml_job;

    reset_task(operation_parameters_ptr);
    operation_parameters_ptr->source_first_ptr       = source_ptr;
    operation_parameters_ptr->source_length          = byte_length;
    operation_parameters_ptr->operation              = DML_OP_DUALCAST;
    operation_parameters_ptr->flags                  = flags;
    operation_parameters_ptr->destination_first_ptr  = destination_first_ptr;
    operation_parameters_ptr->destination_second_ptr = destination_second_ptr;

    return DML_STATUS_OK;
}


DML_FUN(dml_status_t, dml_batch_set_compare_by_index, (dml_job_t * dml_job_ptr,
                                                       uint32_t task_index,
                                                       uint8_t *source_first_ptr,
                                                       uint8_t *source_second_ptr,
                                                       uint32_t byte_length,
                                                       dml_meta_result_t expected_result,
                                                       dml_operation_flags_t flags))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(source_first_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(source_second_ptr)
    OWN_BAD_ARGUMENT_INDEX_OVERFLOW(dml_job_ptr->destination_length, task_index)

    const uint32_t task_position        = (OWN_BATCH_TASK_SIZE * task_index);
    dml_job_t *operation_parameters_ptr = &OWN_BATCH_GET_TASK_BY_INDEX(dml_job_ptr, task_position)->dml_job;

    reset_task(operation_parameters_ptr);
    operation_parameters_ptr->source_first_ptr  = source_first_ptr;
    operation_parameters_ptr->source_second_ptr = source_second_ptr;
    operation_parameters_ptr->source_length     = byte_length;
    operation_parameters_ptr->expected_result   = expected_result;
    operation_parameters_ptr->operation         = DML_OP_COMPARE;
    operation_parameters_ptr->flags             = flags;

    return DML_STATUS_OK;
}


DML_FUN(dml_status_t, dml_batch_set_compare_pattern_by_index, (dml_job_t * dml_job_ptr,
                                                               uint32_t task_index,
                                                               uint8_t *source_ptr,
                                                               uint8_t *pattern_ptr,
                                                               uint32_t byte_length,
                                                               dml_meta_result_t expected_result,
                                                               dml_operation_flags_t flags))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(source_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(pattern_ptr)
    OWN_BAD_ARGUMENT_INDEX_OVERFLOW(dml_job_ptr->destination_length, task_index)

    const uint32_t task_position        = (OWN_BATCH_TASK_SIZE * task_index);
    dml_job_t *operation_parameters_ptr = &OWN_BATCH_GET_TASK_BY_INDEX(dml_job_ptr, task_position)->dml_job;

    reset_task(operation_parameters_ptr);
    operation_parameters_ptr->source_first_ptr = source_ptr;
    operation_parameters_ptr->source_length    = byte_length;
    operation_parameters_ptr->expected_result  = expected_result;
    operation_parameters_ptr->operation        = DML_OP_COMPARE_PATTERN;
    operation_parameters_ptr->flags            = flags;

    // Fill the pattern
    for (uint32_t i = 0u; i < sizeof(operation_parameters_ptr->pattern); ++i)
    {
        operation_parameters_ptr->pattern[i] = pattern_ptr[i];
    }

    return DML_STATUS_OK;
}


DML_FUN(dml_status_t, dml_batch_set_crc_by_index, (dml_job_t * dml_job_ptr,
                                                   uint32_t task_index,
                                                   uint8_t *source_ptr,
                                                   uint32_t byte_length,
                                                   uint32_t *crc_seed_ptr,
                                                   dml_operation_flags_t flags))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(source_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(crc_seed_ptr)
    OWN_BAD_ARGUMENT_INDEX_OVERFLOW(dml_job_ptr->destination_length, task_index)

    const uint32_t task_position        = (OWN_BATCH_TASK_SIZE * task_index);
    dml_job_t *operation_parameters_ptr = &OWN_BATCH_GET_TASK_BY_INDEX(dml_job_ptr, task_position)->dml_job;

    reset_task(operation_parameters_ptr);
    operation_parameters_ptr->source_first_ptr = source_ptr;
    operation_parameters_ptr->source_length    = byte_length;
    operation_parameters_ptr->crc_checksum_ptr = crc_seed_ptr;
    operation_parameters_ptr->operation        = DML_OP_CRC;
    operation_parameters_ptr->flags            = flags;

    return DML_STATUS_OK;
}


DML_FUN(dml_status_t, dml_batch_set_copy_crc_by_index, (dml_job_t * dml_job_ptr,
                                                        uint32_t task_index,
                                                        uint8_t *source_ptr,
                                                        uint32_t byte_length,
                                                        uint32_t *crc_seed_ptr,
                                                        uint8_t *destination_ptr,
                                                        dml_operation_flags_t flags))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(source_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(crc_seed_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(destination_ptr)
    OWN_BAD_ARGUMENT_INDEX_OVERFLOW(dml_job_ptr->destination_length, task_index)

    const uint32_t task_position        = (OWN_BATCH_TASK_SIZE * task_index);
    dml_job_t *operation_parameters_ptr = &OWN_BATCH_GET_TASK_BY_INDEX(dml_job_ptr, task_position)->dml_job;

    reset_task(operation_parameters_ptr);
    operation_parameters_ptr->source_first_ptr      = source_ptr;
    operation_parameters_ptr->source_length         = byte_length;
    operation_parameters_ptr->crc_checksum_ptr      = crc_seed_ptr;
    operation_parameters_ptr->operation             = DML_OP_COPY_CRC;
    operation_parameters_ptr->destination_first_ptr = destination_ptr;
    operation_parameters_ptr->flags                 = flags;

    return DML_STATUS_OK;
}


DML_FUN(dml_status_t, dml_batch_set_fill_by_index, (dml_job_t * dml_job_ptr,
                                                    uint32_t task_index,
                                                    const uint8_t *pattern_ptr,
                                                    uint8_t *destination_ptr,
                                                    uint32_t byte_length,
                                                    dml_operation_flags_t flags))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(pattern_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(destination_ptr)
    OWN_BAD_ARGUMENT_INDEX_OVERFLOW(dml_job_ptr->destination_length, task_index)

    const uint32_t task_position        = (OWN_BATCH_TASK_SIZE * task_index);
    dml_job_t *operation_parameters_ptr = &OWN_BATCH_GET_TASK_BY_INDEX(dml_job_ptr, task_position)->dml_job;

    reset_task(operation_parameters_ptr);
    operation_parameters_ptr->destination_first_ptr = destination_ptr;
    operation_parameters_ptr->destination_length    = byte_length;
    operation_parameters_ptr->operation             = DML_OP_FILL;
    operation_parameters_ptr->flags                 = flags;

    // Fill the pattern
    for (uint32_t i = 0u; i < sizeof(operation_parameters_ptr->pattern); ++i)
    {
        operation_parameters_ptr->pattern[i] = pattern_ptr[i];
    }

    return DML_STATUS_OK;
}


DML_FUN(dml_status_t, dml_batch_set_cache_flush_by_index, (dml_job_t * dml_job_ptr,
                                                           uint32_t task_index,
                                                           uint8_t *destination_ptr,
                                                           uint32_t byte_length,
                                                           dml_operation_flags_t flags))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(destination_ptr)
    OWN_BAD_ARGUMENT_INDEX_OVERFLOW(dml_job_ptr->destination_length, task_index)

    const uint32_t task_position        = (OWN_BATCH_TASK_SIZE * task_index);
    dml_job_t *operation_parameters_ptr = &OWN_BATCH_GET_TASK_BY_INDEX(dml_job_ptr, task_position)->dml_job;

    reset_task(operation_parameters_ptr);
    operation_parameters_ptr->destination_first_ptr = destination_ptr;
    operation_parameters_ptr->destination_length    = byte_length;
    operation_parameters_ptr->operation             = DML_OP_CACHE_FLUSH;
    operation_parameters_ptr->flags                 = flags;

    return DML_STATUS_OK;
}


DML_FUN(dml_status_t, dml_batch_set_delta_create_by_index, (dml_job_t * dml_job_ptr,
                                                            uint32_t task_index,
                                                            uint8_t *source_ptr,
                                                            uint8_t *reference_ptr,
                                                            uint32_t compare_length,
                                                            uint8_t *delta_record_ptr,
                                                            uint32_t delta_record_length,
                                                            dml_meta_result_t expected_result,
                                                            dml_operation_flags_t flags))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(source_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(reference_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(delta_record_ptr)
    OWN_BAD_ARGUMENT_INDEX_OVERFLOW(dml_job_ptr->destination_length, task_index)

    const uint32_t task_position        = (OWN_BATCH_TASK_SIZE * task_index);
    dml_job_t *operation_parameters_ptr = &OWN_BATCH_GET_TASK_BY_INDEX(dml_job_ptr, task_position)->dml_job;

    reset_task(operation_parameters_ptr);
    operation_parameters_ptr->source_first_ptr      = source_ptr;
    operation_parameters_ptr->source_second_ptr     = reference_ptr;
    operation_parameters_ptr->source_length         = compare_length;
    operation_parameters_ptr->expected_result       = expected_result;
    operation_parameters_ptr->operation             = DML_OP_DELTA_CREATE;
    operation_parameters_ptr->destination_first_ptr = delta_record_ptr;
    operation_parameters_ptr->destination_length    = delta_record_length;
    operation_parameters_ptr->flags                 = flags;

    return DML_STATUS_OK;
}


DML_FUN(dml_status_t, dml_batch_set_delta_apply_by_index, (dml_job_t * dml_job_ptr,
                                                           uint32_t task_index,
                                                           uint8_t *delta_record_ptr,
                                                           uint32_t delta_record_length,
                                                           uint8_t *destination_ptr,
                                                           uint32_t destination_length,
                                                           dml_operation_flags_t flags))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(delta_record_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(destination_ptr)
    OWN_BAD_ARGUMENT_INDEX_OVERFLOW(dml_job_ptr->destination_length, task_index)

    const uint32_t task_position        = (OWN_BATCH_TASK_SIZE * task_index);
    dml_job_t *operation_parameters_ptr = &OWN_BATCH_GET_TASK_BY_INDEX(dml_job_ptr, task_position)->dml_job;

    reset_task(operation_parameters_ptr);
    operation_parameters_ptr->source_first_ptr      = delta_record_ptr;
    operation_parameters_ptr->source_length         = delta_record_length;
    operation_parameters_ptr->operation             = DML_OP_DELTA_APPLY;
    operation_parameters_ptr->destination_first_ptr = destination_ptr;
    operation_parameters_ptr->destination_length    = destination_length;
    operation_parameters_ptr->flags                 = flags;

    return DML_STATUS_OK;
}


DML_FUN(dml_status_t, dml_batch_set_dif_check_by_index, (dml_job_t * dml_job_ptr,
                                                         uint32_t task_index,
                                                         uint8_t *source_ptr,
                                                         uint32_t source_length,
                                                         const dml_dif_config_t *dif_config_ptr,
                                                         dml_operation_flags_t flags))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(source_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(dif_config_ptr)
    OWN_BAD_ARGUMENT_INDEX_OVERFLOW(dml_job_ptr->destination_length, task_index)

    const uint32_t task_position        = (OWN_BATCH_TASK_SIZE * task_index);
    dml_job_t *operation_parameters_ptr = &OWN_BATCH_GET_TASK_BY_INDEX(dml_job_ptr, task_position)->dml_job;

    reset_task(operation_parameters_ptr);
    operation_parameters_ptr->source_first_ptr = source_ptr;
    operation_parameters_ptr->source_length    = source_length;
    operation_parameters_ptr->operation        = DML_OP_DIF_CHECK;
    operation_parameters_ptr->dif_config       = (*dif_config_ptr);
    operation_parameters_ptr->flags            = flags;

    return DML_STATUS_OK;
}


DML_FUN(dml_status_t, dml_batch_set_dif_update_by_index, (dml_job_t * dml_job_ptr,
                                                          uint32_t task_index,
                                                          uint8_t *source_ptr,
                                                          uint32_t source_length,
                                                          const dml_dif_config_t *dif_config_ptr,
                                                          uint8_t *destination_ptr,
                                                          uint32_t destination_length,
                                                          dml_operation_flags_t flags))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(source_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(dif_config_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(destination_ptr)
    OWN_BAD_ARGUMENT_INDEX_OVERFLOW(dml_job_ptr->destination_length, task_index)

    const uint32_t task_position        = (OWN_BATCH_TASK_SIZE * task_index);
    dml_job_t *operation_parameters_ptr = &OWN_BATCH_GET_TASK_BY_INDEX(dml_job_ptr, task_position)->dml_job;

    reset_task(operation_parameters_ptr);
    operation_parameters_ptr->source_first_ptr      = source_ptr;
    operation_parameters_ptr->source_length         = source_length;
    operation_parameters_ptr->operation             = DML_OP_DIF_UPDATE;
    operation_parameters_ptr->dif_config            = (*dif_config_ptr);
    operation_parameters_ptr->destination_first_ptr = destination_ptr;
    operation_parameters_ptr->destination_length    = destination_length;
    operation_parameters_ptr->flags                 = flags;

    return DML_STATUS_OK;
}


DML_FUN(dml_status_t, dml_batch_set_dif_insert_by_index, (dml_job_t * dml_job_ptr,
                                                          uint32_t task_index,
                                                          uint8_t *source_ptr,
                                                          uint32_t source_length,
                                                          const dml_dif_config_t *dif_config_ptr,
                                                          uint8_t *destination_ptr,
                                                          uint32_t destination_length,
                                                          dml_operation_flags_t flags))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(source_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(dif_config_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(destination_ptr)
    OWN_BAD_ARGUMENT_INDEX_OVERFLOW(dml_job_ptr->destination_length, task_index)

    const uint32_t task_position        = (OWN_BATCH_TASK_SIZE * task_index);
    dml_job_t *operation_parameters_ptr = &OWN_BATCH_GET_TASK_BY_INDEX(dml_job_ptr, task_position)->dml_job;

    reset_task(operation_parameters_ptr);
    operation_parameters_ptr->source_first_ptr      = source_ptr;
    operation_parameters_ptr->source_length         = source_length;
    operation_parameters_ptr->operation             = DML_OP_DIF_INSERT;
    operation_parameters_ptr->dif_config            = (*dif_config_ptr);
    operation_parameters_ptr->destination_first_ptr = destination_ptr;
    operation_parameters_ptr->destination_length    = destination_length;
    operation_parameters_ptr->flags                 = flags;

    return DML_STATUS_OK;
}


DML_FUN(dml_status_t, dml_batch_set_dif_strip_by_index, (dml_job_t * dml_job_ptr,
                                                         uint32_t task_index,
                                                         uint8_t *source_ptr,
                                                         uint32_t source_length,
                                                         const dml_dif_config_t *dif_config_ptr,
                                                         uint8_t *destination_ptr,
                                                         uint32_t destination_length,
                                                         dml_operation_flags_t flags))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(source_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(dif_config_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(destination_ptr)
    OWN_BAD_ARGUMENT_INDEX_OVERFLOW(dml_job_ptr->destination_length, task_index)

    const uint32_t task_position        = (OWN_BATCH_TASK_SIZE * task_index);
    dml_job_t *operation_parameters_ptr = &OWN_BATCH_GET_TASK_BY_INDEX(dml_job_ptr, task_position)->dml_job;

    reset_task(operation_parameters_ptr);
    operation_parameters_ptr->source_first_ptr      = source_ptr;
    operation_parameters_ptr->source_length         = source_length;
    operation_parameters_ptr->operation             = DML_OP_DIF_STRIP;
    operation_parameters_ptr->dif_config            = (*dif_config_ptr);
    operation_parameters_ptr->destination_first_ptr = destination_ptr;
    operation_parameters_ptr->destination_length    = destination_length;
    operation_parameters_ptr->flags                 = flags;

    return DML_STATUS_OK;
}


DML_FUN(dml_status_t, dml_batch_get_result, (const dml_job_t *dml_job_ptr,
                                             uint32_t task_index,
                                             dml_meta_result_t *result_ptr))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(result_ptr)
    OWN_BAD_ARGUMENT_INDEX_OVERFLOW(dml_job_ptr->destination_length, task_index)

    const uint32_t task_position              = (OWN_BATCH_TASK_SIZE * task_index);
    const dml_job_t *operation_parameters_ptr = &OWN_BATCH_GET_TASK_BY_INDEX(dml_job_ptr, task_position)->dml_job;

    (*result_ptr) = operation_parameters_ptr->result;

    return DML_STATUS_OK;
}


DML_FUN(dml_status_t, dml_batch_get_status, (const dml_job_t *dml_job_ptr,
                                             uint32_t task_index,
                                             dml_status_t *status_ptr))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(status_ptr)
    OWN_BAD_ARGUMENT_INDEX_OVERFLOW(dml_job_ptr->destination_length, task_index)

    const uint32_t task_position = (OWN_BATCH_TASK_SIZE * task_index);

    (*status_ptr) = OWN_BATCH_GET_TASK_BY_INDEX(dml_job_ptr, task_position)->status;

    return DML_STATUS_OK;
}
