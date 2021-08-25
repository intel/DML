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
 * @brief Contains an implementation of @ref idml_hw_submit_job
 * @date 3/16/2020
 *
 */
#include "hardware_api.h"
#include "own_hardware_definitions.h"


/**
 * @brief Sends prepared descriptor into work queue portal
 *
 * @param[in] descriptor_ptr  pointer to descriptor which will be submit
 * @param[in] portal_ptr      portal of work queue
 *
 * @return @ref DML_STATUS_OK on success and others in case of error.
 */
static inline dml_status_t OWN_FUN(enqueue)(const dsahw_descriptor_t *descriptor_ptr,
                                            const portal_t *portal_ptr)
{
    // Variables
    void *const task_ptr = (void * )descriptor_ptr;
    unsigned char retry;

    // Inline assembler
    #if defined( linux )
    asm volatile("sfence\t\n"
                 ".byte 0xf2, 0x0f, 0x38, 0xf8, 0x02\t\n"
                 "setz %0\t\n"
                 : "=r"(retry): "a" (portal_ptr), "d" (task_ptr));
    #else
    retry = 1; // not supported on Windows yet
    #endif

    return retry;
}


dml_status_t DML_HW_API(submit)(const dsahw_context_t *hw_context_ptr,
                                const dsahw_descriptor_t *descriptor_ptr,
                                dml_operation_flags_t flags)
{
    DML_BAD_ARGUMENT_NULL_POINTER(hw_context_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(descriptor_ptr)

    dsahw_context_t *context_ptr  = (dsahw_context_t *)hw_context_ptr;
    const portal_t *portal_ptr = context_ptr->portal_table.tc_a_portals.portals_ptr;

    if ((DML_FLAG_ADDRESS1_TCB & flags) ||
        (DML_FLAG_ADDRESS2_TCB & flags) ||
        (DML_FLAG_ADDRESS3_TCB & flags))
    {
        DML_BAD_ARGUMENT_RETURN(!context_ptr->portal_table.tc_b_portals.portals_ptr,
                                dml_status_tC_B_NOT_AVAILABLE);
        portal_ptr = context_ptr->portal_table.tc_b_portals.portals_ptr;
    }
    else
    {
        DML_BAD_ARGUMENT_RETURN(!context_ptr->portal_table.tc_a_portals.portals_ptr,
                                dml_status_tC_A_NOT_AVAILABLE);
    }

    // Try to enqueue
    const uint32_t attempts_to_enqueue = 10;
    for (uint32_t attempt = 0u; attempt < attempts_to_enqueue; attempt++)
    {
        dml_status_t status = OWN_FUN_CALL(enqueue)(descriptor_ptr, portal_ptr);

        if (DML_STATUS_OK == status)
        {
            return DML_STATUS_OK;
        }
    }

    return DML_STATUS_WORK_QUEUE_OVERFLOW_ERROR;
}
