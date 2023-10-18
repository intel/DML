/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <vector>
#include <cstdint>
#include <stddef.h>

#include "dml/dml.hpp"

static inline int mem_move_fuzz(const uint8_t *data, size_t size){
  if (0==size) {
    return 0;
  }
  {
    std::vector<uint8_t> src(data, data+size);
    std::vector<uint8_t> dst(size, 0xaa);
    auto result = dml::execute<dml::software>(dml::mem_move, dml::make_view(src), dml::make_view(dst));
    if(result.status != dml::status_code::ok ){
      return 0;
    }
  }

  return 0;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size){
  return mem_move_fuzz(data, size);
}
