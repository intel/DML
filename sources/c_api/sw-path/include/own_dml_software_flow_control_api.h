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
 * @date 3/11/2020
 *
 * @defgroup SW_PATH_CONTROL Flow Control API
 * @ingroup SW_PATH
 * @{
 * @brief Contains declaration of common Intel® Data Mover Library (Intel® DML) Features to control the flow.
 *
 */

#ifndef DML_OWN_DML_SOFTWARE_FLOW_CONTROL_API_H__
#define DML_OWN_DML_SOFTWARE_FLOW_CONTROL_API_H__

/**
 * @brief Enables the dml feature that is provided by @ref DML_FLAG_CHECK_RESULT flag.
 *
 * @details If the operation is successful and the @ref DML_FLAG_CHECK_RESULT is set,
 * the function gets a final result in accordance with the table below.
 *
 * @param [in] dml_job_ptr   pointer to user specified @ref dml_job_t
 * @param [in,out] result    successful status received from the operation
 *
 * @note With this feature, you can continue or stop execution of the batch (with the @ref DML_FLAG_FENCE flag)
 * based on the result of the comparison.
 * @note Expected result can be set in the @ref dml_job_t.expected_result field,
 * which can have the following values:
 *       -  0: Expected that data is matching (equal)
 *       -  1: Expected that data is not matching (not equal)
 *
 * @warning Function expects that result is equal to @ref DML_STATUS_OK or @ref DML_STATUS_FALSE_PREDICATE_OK
 *
 * | DML_FLAG_CHECK_RESULT |   Expected result value  | Comparison result | Returned Status              |
 * |-----------------------|--------------------------|-------------------|------------------------------|
 * |           0           |            X             |       equal       | Success                      |
 * |           1           |          equal           |       equal       | Success                      |
 * |           1           |          equal           |     not equal     | Success with false predicate |
 * |           1           |        not equal         |       equal       | Success with false predicate |
 * |           1           |        not equal         |     not equal     | Success                      |
 *
 *
 * @return
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_FALSE_PREDICATE_OK
 *
 */
OWN_API_INLINE(dml_status_t, sw_check_result, (dml_job_t *const dml_job_ptr, const dmlc_status_t result))

#endif //DML_OWN_DML_SOFTWARE_FLOW_CONTROL_API_H__

/** @} */
