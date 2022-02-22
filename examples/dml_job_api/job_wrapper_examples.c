/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Intel DML Job API examples
 * @date 3/10/2020
 *
 */

#include "malloc.h"

#include "dml/dml.h"
#include "dml_examples.h"

/**
 * @page JOB_API_SNIPPET_PAGE How to work with Job API
 *
 * @anchor JOB_API_INIT_DML_JOB_EXAMPLE
 * Before any usage of Job API the @ref dml_job_t structure should be initialized:
 * @snippet this INIT_DML_JOB_SNIPPET
 *
 * @anchor JOB_API_FINALIZE_DML_JOB_EXAMPLE
 * Resources acquired with @ref dml_init_job should be finalized with @ref dml_finalize_job
 * @snippet this FINALIZE_DML_JOB_SNIPPET
 *
 * @anchor JOB_API_EXECUTE_DML_JOB_EXAMPLE
 * Usage examples of @ref dml_execute_job for each @ref dml_operation_t
 *  - @ref DML_OP_NOP does nothing
 *    - @snippet this DML_NOP_SNIPPET
 *  - @ref DML_OP_MEM_MOVE copies (moves) memory from source_first_ptr to destination_first_ptr.
 *                         The number of moved bytes is given by the source_length parameter.
 *    - @snippet this DML_MOVE_8U_SNIPPET
 *  - @ref DML_OP_BATCH The Batch operation processes multiple jobs at once.
 *    - @snippet this DML_COPY_BATCH_8U_SNIPPET
 *  - @ref DML_OP_DUALCAST copies memory from the source_first_ptr to both destination_first_ptr and destination_second_ptr.
 *    - @snippet this DML_COPY_DUALCAST_8U_SNIPPET
 *  - @ref DML_OP_FILL fills the memory at destination_first_ptr with the bytes from the pattern[8] parameter.
 *    - @snippet this DML_FILL_PATTERN_64U_SNIPPET
 *  - @ref DML_OP_COMPARE compares memory at source_first_ptr with memory at source_second_ptr.
 *    - @snippet this DML_COMPARE_8U_SNIPPET
 *  - @ref DML_OP_COMPARE_PATTERN compares memory at source_first_ptr with the value in the pattern field.
 *    - @snippet this DML_COMPARE_PATTERN_64U_SNIPPET
 *  - @ref DML_OP_COPY_CRC copies memory from source_first_ptr to destination_first_ptr and computes CRC on the data copied.
 *    - @snippet this DML_CRC_COPY_8U_SNIPPET
 *  - @ref DML_OP_CRC computes the CRC on memory at source_first_ptr. The number of bytes used for CRC computation is
 *                    given by the source_length parameter. The crc_ptr field in the dml_job_t is used as a seed for
 *                    CRC calculation, therefore it can be used for continuation.
 *    - @snippet this DML_CRC_SNIPPET
 *  - @ref DML_OP_DELTA_CREATE compares memory at source_first_ptr with memory at source_second_ptr
 *                             and generates a delta record containing the information needed to
 *                             update source_first_ptr to match source_second_ptr.
 *                             The number of bytes compared is given by the source_length parameter.
 *    - @snippet this DML_CREATE_DELTA_8U_SNIPPET
 *  - @ref DML_OP_DELTA_APPLY applies a delta record at source_first_ptr to destination_first_ptr.
 *                            The delta record must be created by the Create Delta Record operation with a result equal
 *                            to 1.
 *    - @snippet this DML_APPLY_DELTA_8U_SNIPPET
 *  - @ref DML_OP_DIF_INSERT copies memory from the source_first_ptr to destination_first_ptr (length bytes) while
 *                           computing the DIF on the source data and inserting the DIF into the output data.
 *    - @snippet this DML_DIF_INSERT_8U_SNIPPET
 *  - @ref DML_OP_DIF_UPDATE copies memory from source_first_ptr to destination_first_ptr (length bytes).
 *    - @snippet this DML_DIF_UPDATE_8U_SNIPPET
 *  - @ref DML_OP_DIF_CHECK computes the Data Integrity Field (DIF) on the source data and compares the computed DIF
 *                          to the DIF contained in the source data (source_first_ptr).
 *    - @snippet this DML_DIF_CHECK_8U_SNIPPET
 *  - @ref DML_OP_DIF_STRIP copies memory from source_first_ptr to destination_first_ptr, removing the DIF.
 *    - @snippet this DML_DIF_STRIP_8U_SNIPPET
 *  - @ref DML_OP_CACHE_FLUSH flushes the processor caches at the destination_first_ptr address. The number of bytes
 *                            flushed is given by the destination_length parameter.
 *    - @snippet this DML_CACHE_FLUSH_8U_SNIPPET
 *
 * @anchor JOB_API_SUBMIT_DML_JOB_EXAMPLE
 * A job can be executed asynchronously using @ref dml_submit_job and @ref dml_wait_job
 * @snippet this dml_submit_job_SNIPPET
 */

/* ------ LIBRARY INITIALIZATION ------ */

/** [INIT_DML_JOB_SNIPPET] */
dml_job_t *init_dml_job(const dml_path_t path)
{
    dml_job_t *dml_job_ptr = NULL;
    uint32_t size = 0u;

    dml_status_t status = dml_get_job_size(path, &size);

    if (DML_STATUS_OK != status)
    {
        return NULL;
    }

    dml_job_ptr = (dml_job_t *)malloc(size);

    status = dml_init_job(path, dml_job_ptr);

    if (DML_STATUS_OK != status)
    {
        free(dml_job_ptr);
        return NULL;
    }

    return dml_job_ptr;
}
/** [INIT_DML_JOB_SNIPPET] */

/** [FINALIZE_DML_JOB_SNIPPET] */
dml_status_t finalize_dml_job(dml_job_t *const dml_job_ptr)
{
    return dml_finalize_job(dml_job_ptr);
}
/** [FINALIZE_DML_JOB_SNIPPET] */

/* ------  OPERATION WRAPPERS  ------ */

/** [DML_NOP_SNIPPET] */
dml_status_t dml_nop(dml_job_t *const dml_job_ptr)
{
    dml_job_ptr->operation = DML_OP_NOP;

    return dml_execute_job(dml_job_ptr);
}
/** [DML_NOP_SNIPPET] */

/** [DML_MOVE_8U_SNIPPET] */
dml_status_t dml_move_8u(const uint8_t *const source_ptr,
                         uint8_t *const destination_ptr,
                         const uint32_t length,
                         dml_job_t *const dml_job_ptr)
{
    dml_job_ptr->operation = DML_OP_MEM_MOVE;
    dml_job_ptr->source_first_ptr = (uint8_t *)source_ptr;
    dml_job_ptr->destination_first_ptr = destination_ptr;
    dml_job_ptr->source_length = length;
    dml_job_ptr->destination_length = length;

    return dml_execute_job(dml_job_ptr);
}
/** [DML_MOVE_8U_SNIPPET] */

/** [DML_FILL_PATTERN_64U_SNIPPET] */
dml_status_t dml_fill_pattern_64u(uint8_t *const destination_ptr,
                                  const uint8_t *const pattern_8e_ptr,
                                  const uint32_t destination_length,
                                  dml_job_t *const dml_job_ptr)
{
    dml_job_ptr->operation = DML_OP_FILL;
    dml_job_ptr->destination_first_ptr = destination_ptr;
    dml_job_ptr->destination_length = destination_length;

    for (uint8_t i = 0u; i < PATTERN_SIZE; i++)
    {
        dml_job_ptr->pattern[i] = pattern_8e_ptr[i];
    }

    return dml_execute_job(dml_job_ptr);
}
/** [DML_FILL_PATTERN_64U_SNIPPET] */

/** [DML_COMPARE_8U_SNIPPET] */
dml_status_t dml_compare_8u(const uint8_t *const source_first_ptr,
                            const uint8_t *const source_second_ptr,
                            const uint32_t source_length,
                            dml_job_t *const dml_job_ptr)
{
    dml_job_ptr->operation = DML_OP_COMPARE;
    dml_job_ptr->source_first_ptr = (uint8_t *)source_first_ptr;
    dml_job_ptr->source_second_ptr = (uint8_t *)source_second_ptr;
    dml_job_ptr->source_length = source_length;

    return dml_execute_job(dml_job_ptr);
}
/** [DML_COMPARE_8U_SNIPPET] */

/** [DML_COMPARE_PATTERN_64U_SNIPPET] */
dml_status_t dml_compare_pattern_64u(const uint8_t *const source_ptr,
                                     const uint8_t *const pattern_8e_ptr,
                                     const uint32_t source_length,
                                     dml_job_t *const dml_job_ptr)
{
    dml_job_ptr->operation = DML_OP_COMPARE_PATTERN;
    dml_job_ptr->source_first_ptr = (uint8_t *)source_ptr;
    dml_job_ptr->source_length = source_length;

    for (uint8_t i = 0u; i < PATTERN_SIZE; i++)
    {
        dml_job_ptr->pattern[i] = pattern_8e_ptr[i];
    }

    return dml_execute_job(dml_job_ptr);
}
/** [DML_COMPARE_PATTERN_64U_SNIPPET] */

/** [DML_CRC_SNIPPET] */
dml_status_t dml_crc(uint8_t *const source_ptr,
                     const uint32_t length,
                     uint32_t *const crc_ptr,
                     dml_job_t *const dml_job_ptr)
{
    dml_job_ptr->operation = DML_OP_CRC;
    dml_job_ptr->source_first_ptr = source_ptr;
    dml_job_ptr->source_length = length;
    dml_job_ptr->crc_checksum_ptr = crc_ptr;

    return dml_execute_job(dml_job_ptr);
}
/** [DML_CRC_SNIPPET] */

/** [DML_CRC_COPY_8U_SNIPPET] */
dml_status_t dml_crc_copy_8u(const uint8_t *const source_ptr,
                             uint8_t *const destination_ptr,
                             const uint32_t length,
                             uint32_t *const crc_ptr,
                             dml_job_t *const dml_job_ptr)
{
    dml_job_ptr->operation = DML_OP_COPY_CRC;
    dml_job_ptr->source_first_ptr = (uint8_t *)source_ptr;
    dml_job_ptr->destination_first_ptr = destination_ptr;
    dml_job_ptr->source_length = length;
    dml_job_ptr->destination_length = length;
    dml_job_ptr->crc_checksum_ptr = crc_ptr;

    return dml_execute_job(dml_job_ptr);
}
/** [DML_CRC_COPY_8U_SNIPPET] */

/** [DML_COPY_DUALCAST_8U_SNIPPET] */
dml_status_t dml_copy_dualcast_8u(const uint8_t *const source_ptr,
                                  uint8_t *const destination_first_ptr,
                                  uint8_t *const destination_second_ptr,
                                  const uint32_t length,
                                  dml_job_t *const dml_job_ptr)
{
    dml_job_ptr->operation = DML_OP_DUALCAST;
    dml_job_ptr->source_first_ptr = (uint8_t *)source_ptr;
    dml_job_ptr->destination_first_ptr = destination_first_ptr;
    dml_job_ptr->destination_second_ptr = destination_second_ptr;
    dml_job_ptr->source_length = length;

    return dml_execute_job(dml_job_ptr);
}
/** [DML_COPY_DUALCAST_8U_SNIPPET] */

/** [DML_CREATE_DELTA_8U_SNIPPET] */
dml_status_t dml_create_delta_8u(const uint8_t *const source_first_ptr,
                                 const uint8_t *const source_second_ptr,
                                 uint8_t *const delta_ptr,
                                 const uint32_t max_delta_size,
                                 const uint32_t length,
                                 uint32_t *const delta_size_ptr,
                                 dml_job_t *const dml_job_ptr)
{
    dml_status_t status;
    dml_job_ptr->operation = DML_OP_DELTA_CREATE;
    dml_job_ptr->source_first_ptr = (uint8_t *)source_first_ptr;
    dml_job_ptr->source_second_ptr = (uint8_t *)source_second_ptr;
    dml_job_ptr->destination_first_ptr = delta_ptr;
    dml_job_ptr->source_length = length;
    dml_job_ptr->destination_length = max_delta_size;

    status = dml_execute_job(dml_job_ptr);

    *delta_size_ptr = dml_job_ptr->destination_length;

    return status;
}
/** [DML_CREATE_DELTA_8U_SNIPPET] */

/** [DML_APPLY_DELTA_8U_SNIPPET] */
dml_status_t dml_apply_delta_8u(const uint8_t *const delta_ptr,
                                uint8_t *const destination_ptr,
                                const uint32_t delta_size,
                                const uint32_t destination_length,
                                dml_job_t *const dml_job_ptr)
{
    dml_job_ptr->operation = DML_OP_DELTA_APPLY;
    dml_job_ptr->source_first_ptr = (uint8_t *)delta_ptr;
    dml_job_ptr->source_length = delta_size;
    dml_job_ptr->destination_first_ptr = destination_ptr;
    dml_job_ptr->destination_length = destination_length;

    return dml_execute_job(dml_job_ptr);
}
/** [DML_APPLY_DELTA_8U_SNIPPET] */

/** [DML_DIF_CHECK_8U_SNIPPET] */
dml_status_t dml_dif_check_8u(const uint8_t *const source_ptr,
                              const uint32_t length,
                              dml_job_t *const dml_job_ptr)
{
    // Init general job parameters
    dml_job_ptr->operation = DML_OP_DIF_CHECK;
    dml_job_ptr->source_first_ptr = (uint8_t *)source_ptr;
    dml_job_ptr->source_length = length;

    dml_job_ptr->dif_config.block_size = DML_DIF_BLOCK_SIZE_4096;

    dml_job_ptr->dif_config.source_application_tag_mask = 0u;
    dml_job_ptr->dif_config.source_application_tag_seed = 0x0100;
    dml_job_ptr->dif_config.source_reference_tag_seed = 0u;

    dml_job_ptr->dif_config.flags = DML_DIF_FLAG_INVERT_CRC_SEED
                                    | DML_DIF_FLAG_INVERT_CRC_RESULT
                                    | DML_DIF_FLAG_SRC_INC_APP_TAG
                                    | DML_DIF_FLAG_SRC_FIX_REF_TAG;

    return dml_execute_job(dml_job_ptr);
}
/** [DML_DIF_CHECK_8U_SNIPPET] */

/** [DML_DIF_UPDATE_8U_SNIPPET] */
dml_status_t dml_dif_update_8u(const uint8_t *const source_ptr,
                               uint8_t *const destination_ptr,
                               const uint32_t length,
                               dml_job_t *const dml_job_ptr)
{
    // Init general job parameters
    dml_job_ptr->operation = DML_OP_DIF_UPDATE;
    dml_job_ptr->source_first_ptr = (uint8_t *)source_ptr;
    dml_job_ptr->destination_first_ptr = destination_ptr;
    dml_job_ptr->source_length = length;
    dml_job_ptr->destination_length = length;

    /*
     * Init operation parameters to update only application tag with disabled check
     *
     * Data blocks will be redistributed between applications
     * Application 1 used a fixed application tag 0x0100 and incremented reference tag
     * Application 2 will use an application tag 0x02xx where low byte will be incremented to separate 256 blocks
     * Application 2 will use a fixed reference tag.
     */
    dml_job_ptr->dif_config.block_size = DML_DIF_BLOCK_SIZE_4096;

    dml_job_ptr->dif_config.source_application_tag_mask = 0u;
    dml_job_ptr->dif_config.source_application_tag_seed = 0x0100;
    dml_job_ptr->dif_config.source_reference_tag_seed = 0u;

    dml_job_ptr->dif_config.destination_application_tag_mask = 0xFF00u;
    dml_job_ptr->dif_config.destination_application_tag_seed = 0x0200u;
    dml_job_ptr->dif_config.destination_reference_tag_seed = 0u;

    dml_job_ptr->dif_config.flags = DML_DIF_FLAG_INVERT_CRC_SEED
                                    | DML_DIF_FLAG_INVERT_CRC_RESULT
                                    | DML_DIF_FLAG_SRC_GUARD_CHECK_DISABLE
                                    | DML_DIF_FLAG_SRC_REF_TAG_CHECK_DISABLE
                                    | DML_DIF_FLAG_DST_INC_APP_TAG
                                    | DML_DIF_FLAG_DST_FIX_REF_TAG;

    return dml_execute_job(dml_job_ptr);
}
/** [DML_DIF_UPDATE_8U_SNIPPET] */

/** [DML_DIF_INSERT_8U_SNIPPET] */
dml_status_t dml_dif_insert_8u(const uint8_t *const source_ptr,
                               uint8_t *const destination_ptr,
                               const uint32_t source_length,
                               const uint32_t destination_size,
                               dml_job_t *const dml_job_ptr)
{
    // Init general job parameters
    dml_job_ptr->operation = DML_OP_DIF_INSERT;
    dml_job_ptr->source_first_ptr = (uint8_t *)source_ptr;
    dml_job_ptr->destination_first_ptr = destination_ptr;
    dml_job_ptr->source_length = source_length;
    dml_job_ptr->destination_length = destination_size;

    dml_job_ptr->dif_config.block_size = DML_DIF_BLOCK_SIZE_4096;

    dml_job_ptr->dif_config.destination_application_tag_mask = 0x0u;
    dml_job_ptr->dif_config.destination_application_tag_seed = 0x0100;
    dml_job_ptr->dif_config.destination_reference_tag_seed = 0u;

    dml_job_ptr->dif_config.flags = DML_DIF_FLAG_INVERT_CRC_SEED
                                    | DML_DIF_FLAG_INVERT_CRC_RESULT
                                    | DML_DIF_FLAG_DST_INC_APP_TAG
                                    | DML_DIF_FLAG_DST_FIX_REF_TAG;

    return dml_execute_job(dml_job_ptr);
}
/** [DML_DIF_INSERT_8U_SNIPPET] */

/** [DML_DIF_STRIP_8U_SNIPPET] */
dml_status_t dml_dif_strip_8u(const uint8_t *const source_ptr,
                              uint8_t *const destination_ptr,
                              const uint32_t source_length,
                              const uint32_t destination_size,
                              dml_job_t *const dml_job_ptr)
{
    // Init general job parameters
    dml_job_ptr->operation = DML_OP_DIF_STRIP;
    dml_job_ptr->source_first_ptr = (uint8_t *)source_ptr;
    dml_job_ptr->destination_first_ptr = destination_ptr;
    dml_job_ptr->source_length = source_length;
    dml_job_ptr->destination_length = destination_size;

    dml_job_ptr->dif_config.block_size = DML_DIF_BLOCK_SIZE_4096;

    dml_job_ptr->dif_config.source_application_tag_mask = 0x0u;
    dml_job_ptr->dif_config.source_application_tag_seed = 0x0100;
    dml_job_ptr->dif_config.source_reference_tag_seed = 0u;

    dml_job_ptr->dif_config.flags = DML_DIF_FLAG_INVERT_CRC_SEED
                                    | DML_DIF_FLAG_INVERT_CRC_RESULT
                                    | DML_DIF_FLAG_SRC_INC_APP_TAG
                                    | DML_DIF_FLAG_SRC_FIX_REF_TAG;

    return dml_execute_job(dml_job_ptr);
}
/** [DML_DIF_STRIP_8U_SNIPPET] */

/** [DML_COPY_BATCH_8U_SNIPPET] */
dml_status_t dml_copy_batch_8u(uint8_t *source_first_ptr,
                               uint8_t *destination_first_ptr,
                               uint32_t first_length,
                               uint8_t *source_second_ptr,
                               uint8_t *destination_second_ptr,
                               uint32_t second_length,
                               dml_job_t *const dml_job_ptr)
{
    dml_status_t status = DML_STATUS_OK;
    uint32_t operations_count    = 4u;
    uint32_t batch_buffer_length = 0u;

    status = dml_get_batch_size(dml_job_ptr, operations_count, &batch_buffer_length);

    if (DML_STATUS_OK != status)
    {
        return status;
    }

    uint8_t * batch_buffer_ptr = (uint8_t *) malloc(batch_buffer_length);

    dml_job_ptr->operation             = DML_OP_BATCH;
    dml_job_ptr->destination_first_ptr = batch_buffer_ptr;
    dml_job_ptr->destination_length    = batch_buffer_length;

    status = dml_batch_set_mem_move_by_index(dml_job_ptr,
                                             0u,
                                             source_first_ptr,
                                             destination_first_ptr,
                                             first_length,
                                             0u);
    if (DML_STATUS_OK != status)
    {
        free(batch_buffer_ptr);
        return status;
    }

    status = dml_batch_set_mem_move_by_index(dml_job_ptr,
                                             1u,
                                             source_second_ptr,
                                             destination_second_ptr,
                                             second_length,
                                             0u);
    if (DML_STATUS_OK != status)
    {
        free(batch_buffer_ptr);
        return status;
    }

    status = dml_batch_set_mem_move_by_index(dml_job_ptr,
                                             2u,
                                             source_second_ptr,
                                             destination_second_ptr,
                                             second_length,
                                             0u);
    if (DML_STATUS_OK != status)
    {
        free(batch_buffer_ptr);
        return status;
    }

    status = dml_batch_set_mem_move_by_index(dml_job_ptr,
                                             3u,
                                             source_second_ptr,
                                             destination_second_ptr,
                                             second_length,
                                             0u);
    if (DML_STATUS_OK != status)
    {
        free(batch_buffer_ptr);
        return status;
    }

    status = dml_execute_job(dml_job_ptr);

    free(batch_buffer_ptr);

    return status;
}

/** [DML_COPY_BATCH_8U_SNIPPET] */

/** [DML_CACHE_FLUSH_8U_SNIPPET] */
dml_status_t dml_cache_flush_8u(uint8_t *const destination_ptr,
                                const uint32_t length,
                                dml_job_t *const dml_job_ptr)
{
    dml_job_ptr->operation = DML_OP_CACHE_FLUSH;
    dml_job_ptr->destination_first_ptr = destination_ptr;
    dml_job_ptr->destination_length = length;

    return dml_execute_job(dml_job_ptr);
}
/** [DML_CACHE_FLUSH_8U_SNIPPET] */

/** [dml_submit_job_SNIPPET] */
dml_status_t dml_submit(const uint8_t *const source_first_ptr,
                        const uint8_t *const source_second_ptr,
                        const uint32_t source_first_length,
                        const uint32_t source_second_length,
                        uint32_t *const crc_first_ptr,
                        uint32_t *const crc_second_ptr,
                        dml_job_t *const dml_first_job_ptr,
                        dml_job_t *const dml_second_job_ptr)
{
    dml_first_job_ptr->operation = DML_OP_CRC;
    dml_first_job_ptr->source_first_ptr = (uint8_t *)source_first_ptr;
    dml_first_job_ptr->source_length = source_first_length;
    dml_first_job_ptr->crc_checksum_ptr = crc_first_ptr;

    dml_second_job_ptr->operation = DML_OP_CRC;
    dml_second_job_ptr->source_first_ptr = (uint8_t *)source_second_ptr;
    dml_second_job_ptr->source_length = source_second_length;
    dml_second_job_ptr->crc_checksum_ptr = crc_second_ptr;

    dml_status_t status = dml_submit_job(dml_first_job_ptr);
    if (DML_STATUS_OK != status)
    {
        return status;
    }

    status = dml_submit_job(dml_second_job_ptr);
    if (DML_STATUS_OK != status)
    {
        return status;
    }

    status = dml_wait_job(dml_first_job_ptr);
    if (DML_STATUS_OK != status)
    {
        return status;
    }

    return dml_wait_job(dml_second_job_ptr);
}
/** [dml_submit_job_SNIPPET] */
