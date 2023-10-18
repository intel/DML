/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <vector>
#include <cstdint>
#include <stddef.h>

#include "dml/dml.hpp"

static inline int fill_fuzz(const uint8_t *data, size_t size){
  if (size <= 8) { // first 8 bytes needed for fill pattern
    return 0;
  }
  {
    std::vector<uint8_t> dst(data + 8, data + size);
    const uint64_t starting_pattern = *reinterpret_cast<const uint64_t*>(data);
    uint64_t pattern = starting_pattern;
    auto result = dml::execute<dml::software>(dml::fill, pattern , dml::make_view(dst));
    if(result.status != dml::status_code::ok ){
      return 0;
    }
  }
  return 0;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size){
  return fill_fuzz(data, size);
}
