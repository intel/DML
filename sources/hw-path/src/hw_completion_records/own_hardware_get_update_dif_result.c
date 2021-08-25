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
 * @brief Contains an implementation of the @ref dsa_get_update_dif_result function
 * @date 06/23/2020
 *
 */
#include <assert.h>
#include "own_hardware_status.h"
#include "own_completion_record_defines.h"
#include "hardware_completion_records_api.h"


/**
 * @brief Contains tag values
 */
DML_HW_BYTE_PACKED_STRUCTURE_BEGIN
{
    uint32_t reference_tag;     /**< Reference tag value    */
    uint16_t application_mask;  /**< Application mask value */
    uint16_t application_tag;   /**< Application tag value  */
} dif_tag_values_t;
DML_HW_BYTE_PACKED_STRUCTURE_END


/**
 * @brief Represents a Completion Record of the @ref DML_OP_DIF_UPDATE operation
 */
DML_HW_BYTE_PACKED_STRUCTURE_BEGIN
{
    own_general_result_field_t general_result_field; /**< General Result field    */
    uint8_t                    *fault_address_ptr;   /**< Address of Page Fault   */
    dif_tag_values_t           source_tags;          /**< Last values of DIF tags in the source */
    dif_tag_values_t           destination_tags;     /**< Last values of DIF tags in the destination */
} own_update_dif_completion_record_t;
DML_HW_BYTE_PACKED_STRUCTURE_END


/*
 * Check that completion record has a correct size
 */
static_assert(sizeof(own_update_dif_completion_record_t) == DSA_HW_COMPLETION_RECORD_SIZE,
              "Descriptor size is not correct");


dsahw_status_t DML_HW_API(get_update_dif_result)(const dsahw_completion_record_t *completion_record_ptr,
                                                 dml_dif_config_t *dif_config_ptr,
                                                 dml_meta_result_t *result_ptr,
                                                 uint32_t *elements_processed_ptr)
{
    own_update_dif_completion_record_t *hw_result_ptr = (own_update_dif_completion_record_t *) completion_record_ptr;

    hw_status_t hw_status = hw_result_ptr->general_result_field.operation_status;
    *result_ptr           = hw_result_ptr->general_result_field.operation_result;

    dif_config_ptr->source_reference_tag_seed   = hw_result_ptr->source_tags.reference_tag;
    dif_config_ptr->source_application_tag_mask = hw_result_ptr->source_tags.application_mask;
    dif_config_ptr->source_application_tag_seed = hw_result_ptr->source_tags.application_tag;

    dif_config_ptr->destination_reference_tag_seed   = hw_result_ptr->destination_tags.reference_tag;
    dif_config_ptr->destination_application_tag_mask = hw_result_ptr->destination_tags.application_mask;
    dif_config_ptr->destination_application_tag_seed = hw_result_ptr->destination_tags.application_tag;

    switch (hw_status)
    {
        case HW_STATUS_SUCCESS:

            return DML_STATUS_OK;

        case HW_STATUS_DIF_CONTROL_ERROR:

            (*elements_processed_ptr) = hw_result_ptr->general_result_field.elements_processed;

            return DML_STATUS_DIF_CHECK_ERROR;

        case HW_STATUS_PAGE_FAULT_DURING_PROCESSING:

            (*elements_processed_ptr) = hw_result_ptr->general_result_field.elements_processed;

            return DML_STATUS_PAGE_FAULT_ERROR;

        case HW_STATUS_BUFFERS_OVERLAP:

            return DML_STATUS_OVERLAPPING_BUFFER_ERROR;

        default:

            return DML_STATUS_INTERNAL_ERROR;
    }
}
