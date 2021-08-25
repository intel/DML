/*
 * Copyright 2021 Intel Corporation.
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
 * @date 05/20/2021
 * @brief Contains internal utilities
 */

#ifndef DML_DETAIL_UTILS_HPP
#define DML_DETAIL_UTILS_HPP

/**
 * @brief Checks whether two sizes are the same
 */
#define DML_VALIDATE_SIZE_CONSISTENCY(lhs, rhs) if (lhs != rhs) return dml::status_code::inconsistent_size;

#endif  //DML_DETAIL_UTILS_HPP
