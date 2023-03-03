/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <dml_test_utils/data_storage.hpp>

namespace dml::testing
{
    data_storage::data_storage(const uint32_t size, const uint32_t alignment):
        base(size, allocator_t{get_aligned_memory_resource(alignment)})
    {
    }
}  // namespace dml::testing
