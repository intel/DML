/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <vector>
#include <cstdint>
#include <stddef.h>

#include "dml/dml.hpp"

static inline int crc32_fuzz(const uint8_t *data, size_t size){
  if (size <= 4) { // first 4 bytes needed for crc_seed
    return 0;
  }
  {
    const uint32_t crc_start_seed = *reinterpret_cast<const uint32_t*>(data);
    uint32_t crc_seed = crc_start_seed;
    std::vector<uint8_t> src(data+4, data+size);
    auto result = dml::execute<dml::software>(dml::crc, dml::make_view(src), crc_seed);
    if(result.status != dml::status_code::ok ){
      return 0;
    }
  }

  return 0;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size){
  return crc32_fuzz(data, size);
}
