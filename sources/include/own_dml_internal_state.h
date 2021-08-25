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
 * @date 3/13/2020
 *
 * @addtogroup dml_job_private_definitions
 * @{
 * @brief Contains a description of internal DML structure
 *
 */
#if defined(DML_HW)
    #include "hardware_limits.h"
    #include "hardware_definitions.h"
#endif
#include "own_dml_definitions.h"

#ifndef DML_OWN_DML_INTERNAL_DATA_DEFINITIONS_H__
#define DML_OWN_DML_INTERNAL_DATA_DEFINITIONS_H__

/* #################  DML INTERNAL STRUCTURE COMMON DEFINITIONS  ################# */

/**
 * @brief Gets @ref own_dml_state_t from @ref  dml_job_t.internal_data_ptr
 */
#define OWN_GET_JOB_STATE_PTR(dml_job_ptr) ((own_dml_state_t *) (dml_job_ptr)->internal_data_ptr)


#if defined(DML_HW)
    #define OWN_HW_INTERNAL_BUFFERS_SIZE  (DML_HW_BATCH_DESCRIPTORS_SIZE   + \
                                        DML_HW_BATCH_RESULTS_SIZE       + \
                                        DSA_HW_MAX_DESCRIPTOR_ALIGNMENT + \
                                        DSA_HW_MAX_RESULT_ALIGNMENT) /**< The size of the internal HW buffers */
#endif


/**
 * @brief Contains Information about batch state
 */
typedef struct
{
    own_dml_structure_id_t guard;         /**< Structure guard               */
    uint32_t size;                        /**< Current batch size            */
    uint32_t max_size;                    /**< Maximal batch size            */
    own_dml_buffer_t descriptors;         /**< Buffer for batch tasks        */
    own_dml_buffer_t completion_records;  /**< Buffer for completion records */
} own_dml_batch_info_t;


#if defined(DML_HW)
/**
 * @brief Hardware operation type
 */
typedef struct
{
    dsahw_descriptor_t        *descriptor_ptr; /**< Pointer to hardware descriptor */
    dsahw_completion_record_t *result_ptr;     /**< Pointer to hardware completion record */
} own_dml_hw_operation_t;

/**
 * @brief Hardware batch buffer type
 */
typedef struct
{
    dsahw_descriptor_t        *descriptors_ptr; /**< Pointer to sequence of hardware descriptors */
    dsahw_completion_record_t *results_ptr;     /**< Pointer to sequence of hardware completion records */
} own_dml_hw_batch_buffer_t;
#endif


/**
 * @brief Contain specific information, which are used to @ref dml_job_t execution.
 */
typedef struct
{
    dml_path_t                active_path;      /**< Execution path, for which @ref dml_job_t was inited with @ref dml_init_job */
    own_dml_batch_info_t      batch_info;       /**< Information about Batch for current job                                    */
#if defined(DML_HW)
    own_dml_hw_operation_t    hw_operation;     /**< Contains descriptor and completion record for an operation execution       */
    own_dml_hw_batch_buffer_t hw_batch_buffers; /**< Contains descriptors and completion records for the batch internal buffers */
#endif
    uint8_t                   *sw_state_ptr;    /**< Specific information about @ref dml_job_t to execute with software path    */
    uint8_t                   *hw_state_ptr;    /**< Specific information about @ref dml_job_t to execute with hardware path    */
} own_dml_state_t;



/* ################# DML INTERNAL STRUCTURE SOFTWARE DEFINITIONS ################# */

#define OWN_SOFTWARE_BATCH_SIZE 100 /**< Temporary limitation */

/**
 * @brief Contains specific information about Software Path
 */
typedef struct
{
    own_dml_structure_id_t guard; /**< Structure guard */
    uint8_t placeholder;          /**< Temporary placeholder */
} own_sw_state_t;


#endif //DML_OWN_DML_INTERNAL_DATA_DEFINITIONS_H__

/** @} */
