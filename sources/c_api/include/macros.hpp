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

#ifndef DML_MACROS_HPP
#define DML_MACROS_HPP

#include "dml/dmldefs.h"

#define CHECK_NULL(p) \
    if (!(p))         \
    return DML_STATUS_NULL_POINTER_ERROR

#define CHECK_PATH(p)                                                              \
    if (DML_PATH_AUTO != (p) && DML_PATH_SW != (p) && DML_PATH_HW != (p)) \
    return DML_STATUS_NULL_POINTER_ERROR

#endif // DML_MACROS_HPP
