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
 * @brief Contains a TODO
 * @date 3/19/2020
 *
 */

#include "own_dml_internal_state.h"
#include "own_dml_align.h"

OWN_FUN(uint32_t, sw_get_state_size, ())
{
    return DML_ALIGNED_SIZE(sizeof(own_sw_state_t), DML_DEFAULT_ALIGNMENT);
}