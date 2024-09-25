/*******************************************************************************
 * Copyright (C) 2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_MIDDLE_LAYER_DISPATCHER_DESCRIPTOR_UTILS_HPP_
#define DML_MIDDLE_LAYER_DISPATCHER_DESCRIPTOR_UTILS_HPP_

#include <cstdint>

#include "legacy_headers/hardware_definitions.h"

namespace dml::core::util
{
/**
 * @brief Get operation from descriptor
 *
 * @param[in] descriptor_ptr @ref hw_descriptor
 * @return operation field from descriptor
 */
static inline uint8_t descriptor_get_operation(const dsahw_descriptor_t *desc_ptr) {
    const uint8_t OP_CODE_BYTE_OFFSET = 7;
    return (uint8_t)(desc_ptr->bytes[OP_CODE_BYTE_OFFSET]);
}
}

#endif  //DML_MIDDLE_LAYER_DISPATCHER_DESCRIPTOR_UTILS_HPP_
