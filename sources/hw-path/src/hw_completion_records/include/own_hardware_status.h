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
 * @file
 * @date 3/27/2020
 *
 * @defgroup HW_PATH_STATUSES Hardware Statuses
 * @ingroup HW_PATH
 * @{
 * @brief Contains a Hardware status descriptions
 */

#ifndef DML_OWN_HARDWARE_ERRORS_H__
#define DML_OWN_HARDWARE_ERRORS_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Represent hardware statuses. For more detail about status take a look at @ref HardwareStatusDetail
 */
typedef enum
{
    HW_STATUS_SUCCESS                       = 0x01u,  /**< Success. */
    HW_STATUS_FALSE_PREDICATE_SUCCESS       = 0x02u,  /**< Success with false   predicate. */
    HW_STATUS_PAGE_FAULT_DURING_PROCESSING  = 0x03u,  /**< Partial completion due to page fault.  */
    HW_STATUS_PAGE_RESPONSE_ERROR           = 0x04u,  /**< Partial completion due to an Invalid Request response to a Page Request. */
    HW_STATUS_BATCH_PROCESSING_ERROR        = 0x05u,  /**< One or more operations in the batch completed with Status not equal to Success. */
    HW_STATUS_BATCH_PAGE_FAULT_ERROR        = 0x06u,  /**< Partial completion due to page fault.  */
    HW_STATUS_OFFSET_ORDER_ERROR            = 0x07u,  /**< Offsets in the delta record were not in increasing order. */
    HW_STATUS_OFFSET_OVERFLOW               = 0x08u,  /**< An offset in the delta record was greater than or equal to the Transfer Size of the descriptor. */
    HW_STATUS_DIF_CONTROL_ERROR             = 0x09u,  /**< DIF error. This value is used for the DIF Check, DIF Strip, and DIF Update operations. */
    HW_STATUS_OPERATION_ERROR               = 0x10u,  /**< Unsupported operation code. */
    HW_STATUS_FLAG_ERROR                    = 0x11u,  /**< Invalid flags. Any flag contains an unsupported or reserved value. */
    HW_STATUS_NON_ZERO_RESERVED_FIELD_ERROR = 0x12u,  /**< Non-zero reserved field (other than a flag). */
    HW_STATUS_INVALID_TRANSFER_SIZE_ERROR   = 0x13u,  /**< Invalid Transfer Size. */
    HW_STATUS_DESCRIPTOR_COUNT_ERROR        = 0x14u,  /**< Descriptor Count out of range. */
    HW_STATUS_DELTA_SIZE_ERROR              = 0x15u,  /**< Maximum Delta Record Size or Delta Record Size out of range. */
    HW_STATUS_BUFFERS_OVERLAP               = 0x16u,  /**< Overlapping buffers. */
    HW_STATUS_DUALCAST_MISALIGN_ERROR       = 0x17u,  /**< Bits 11:0 of the two destination buffers differ in Memory Copy with Dualcast. */
    HW_STATUS_DESCRIPTOR_LIST_ALIGN_ERROR   = 0x18u,  /**< Misaligned Descriptor List Address. */
    HW_STATUS_INVALID_INTERRUPT_HANDLE      = 0x19u,  /**< Invalid Completion Interrupt Handle. */
    HW_STATUS_PAGE_FAULT_ON_TRANSLATION     = 0x1au,  /**< A page fault occurred while translating a Completion Record Address and either */
    HW_STATUS_COMPLETION_RECORD_ALIGN_ERROR = 0x1bu,  /**< Completion Record Address is not 32-byte aligned. */
    HW_STATUS_MISALIGN_ADDRESS_ERROR        = 0x1cu,  /**< Misaligned address */
    HW_STATUS_PRIVILEGE_ERROR               = 0x1du,  /**< Priv is 1 and the Privileged Mode Enable field of the PCI Express PASID capability is 0. */
    HW_STATUS_TRAFFIC_CLASS_ERROR           = 0x1eu,  /**< Incorrect Traffic Class configuration */
    HW_STATUS_READBACK_TRANSLATION_ERROR    = 0x1fu,  /**< A page fault occurred while translating a Readback Address */
    HW_STATUS_OPERATION_READBACK_TIMEOUT    = 0x20u,  /**< The operation failed due to a hardware error other than a completion timeout or unsuccessful */
    HW_STATUS_HARDWARE_TIMEOUT              = 0x21u,  /**< Hardware error (completion timeout or unsuccessful completion status) */
    HW_STATUS_ADDRESS_TRANSLATION_ERROR     = 0x22u,  /**< An error occurred during address translation */
} hw_status_t;



/* ======================== STATUS CODES COMMENTS ======================== */

/**
 * @name Detail flag description
 * @anchor HardwareStatusDetail
 * @{
 * @brief This table extend descriptions for some hardware statuses
 *
 * | STATUS VALUE |             STATUS NAME                |                                     Details                                                              |
 * | -----------: | :------------------------------------: | :------------------------------------------------------------------------------------------------------- |
 * |     0x03     | HW_STATUS_PAGE_FAULT_DURING_PROCESSING | Partial completion due to page fault, when the Block on Fault flag in the descriptor is 0.               |
 * |     ----     |               -------                  |                                         ----                                                             |
 * |     0x06     |    HW_STATUS_BATCH_PAGE_FAULT_ERROR    | Partial completion of batch due to page fault while translating the Descriptor List Address              |
 * |              |                                        | in a Batch descriptor and either:                                                                        |
 * |              |                                        |   - Page Request Services are disabled; or                                                               |
 * |              |                                        |   - An Invalid Request response was received for the Page Request for the Descriptor List Address        |
 * |     ----     |               -------                  |                                         ----                                                             |
 * |     0x09     |      HW_STATUS_DIF_CONTROL_ERROR       | This value is used for the DIF Check, DIF Strip, and DIF Update operations.                              |
 * |     ----     |               -------                  |                                         ----                                                             |
 * |     0x14     |    HW_STATUS_DESCRIPTOR_COUNT_ERROR    | Descriptor Count out of range (less than 2 or greater than the maximum batch size for the WQ)            |
 * |     ----     |               -------                  |                                         ----                                                             |
 * |     0x1a     |  HW_STATUS_PAGE_FAULT_ON_TRANSLATION   | A page fault occurred while translating a Completion Record Address and either:                          |
 * |              |                                        |    - Page Request Services are disabled; or                                                              |
 * |              |                                        |    - An Invalid Request response was received for the Page Request for the completion record.            |
 * |     ----     |               -------                  |                                         ----                                                             |
 * |     0x1c     |    HW_STATUS_MISALIGN_ADDRESS_ERROR    | Misaligned address:                                                                                      |
 * |              |                                        |    - In a Create Delta Record or Apply Delta Record operation:                                           |
 * |              |                                        |           Source1 Address, Source2 Address,                                                              |
 * |              |                                        |           Destination Address, or Transfer Size is not 8-byte aligned.                                   |
 * |              |                                        |    - In a CRC Generation or Copy with CRC Generation operation: CRC Seed Address is not 4-byte aligned.  |
 * |     ----     |               -------                  |                                         ----                                                             |
 * |     0x1e     |    HW_STATUS_TRAFFIC_CLASS_ERROR       | Incorrect Traffic Class configuration:                                                                   |
 * |              |                                        |    - A TC selected by the descriptor is not enabled in the TC/VC Map of any VC Resource сontrol register.|
 * |              |                                        |    - A TC selected by the descriptor is enabled in the TC/VC Map of a VC Resource Control register       |
 * |              |                                        |      in which VC Enable is 0.                                                                            |
 * |     ----     |               -------                  |                                         ----                                                             |
 * |     ----     |               -------                  | A page fault occurred while translating a Readback Address in a Drain descriptor and either:             |
 * |              |                                        |    - Page Request Services are disabled; or                                                              |
 * |              |                                        |    - An Invalid Request response was received for the Page Request for the Drain Readback Address.       |                                                      |
 * |     ----     |               -------                  |                                         ----                                                             |
 * |     0x20     |  HW_STATUS_OPERATION_READBACK_TIMEOUT  | The operation failed due to a hardware error other than a completion timeout or unsuccessful             |
 * |              |                                        | completion status for destination readback. Details of the hardware error are reported via PCIe          |
 * |              |                                        | Advanced Error Reporting (AER), if enabled.                                                              |                                         |
 * |     ----     |               -------                  |                                         ----                                                             |
 * |     0x21     |       HW_STATUS_HARDWARE_TIMEOUT       | Hardware error (completion timeout or unsuccessful completion status) on a destination                   |
 * |              |                                        | readback operation. Error details are reported via PCIe Advanced Error Reporting (AER), if               |
 * |              |                                        | enabled.                                                                                                 |
 * |     ----     |               -------                  |                                         ----                                                             |
 * |     0x22     |  HW_STATUS_ADDRESS_TRANSLATION_ERROR   | An error occurred during address translation:                                                            |
 * |              |                                        |    - An UR or CA response to an ATS translation request.                                                 |
 * |              |                                        |    - A Response Failure response to a Page Request.                                                      |
 * |              |                                        | The error is also recorded in SWERROR and in some cases, also via PCIe Advanced Error Reporting (AER),   |
 * |              |                                        | if enabled.                                                                                              |                                                                                                   |
 * |     ----     |               -------                  |                                         ----                                                             |
 * |  0x0a - 0x0f |                UNUSED                  |                                         ----                                                             |
 * |  0x23 - 0x3f |                UNUSED                  |                                         ----                                                             |
 *
 */

/** @} */

#ifdef __cplusplus
}
#endif

#endif //DML_OWN_HARDWARE_ERRORS_H__

/** @} */
