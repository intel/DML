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
 * @date 05/19/2021
 * @defgroup dmlml DML Middle Layer
 * @brief Middle Layer for Intel(R) Data Mover Library (Intel® DML)
 */

#ifndef DML_ML_DESCRIPTOR_HPP
#define DML_ML_DESCRIPTOR_HPP

#include "types.hpp"

namespace dml::ml
{
    /**
     * @todo
     */
    struct alignas(64u) descriptor
    {
        byte_t bytes[64u]{}; /**< Underlying data array */
    };
}  // namespace dml::ml

#endif  //DML_ML_DESCRIPTOR_HPP
