/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_MACROS_HPP
#define DML_MACROS_HPP

#include "dml/dmldefs.h"

#define CHECK_NULL(p) \
    if (!(p))         \
    return DML_STATUS_NULL_POINTER_ERROR

#define CHECK_PATH(p)                                                     \
    if (DML_PATH_AUTO != (p) && DML_PATH_SW != (p) && DML_PATH_HW != (p)) \
    return DML_STATUS_PATH_ERROR

#endif  // DML_MACROS_HPP
