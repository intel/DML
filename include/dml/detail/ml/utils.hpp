/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_DETAIL_ML_UTILS_HPP
#define DML_DETAIL_ML_UTILS_HPP

#include <dml/detail/common/status.hpp>
#include <dml/detail/ml/result.hpp>
#include <memory>

namespace dml::detail::ml::utils
{
   template <typename head_t, typename... tail_t>
   struct structure_from
   {
       head_t                    head;
       structure_from<tail_t...> tail;
   };

   template <typename head_t>
   struct structure_from<head_t>
   {
       head_t head;
   };

   template <std::size_t index, typename requested_t, typename head_t, typename... tail_t>
   [[nodiscard]] inline auto &get(structure_from<head_t, tail_t...> &s) noexcept
   {
       if constexpr (std::is_same_v<head_t, requested_t>)
       {
           if constexpr (index == 0)
           {
               return s.head;
           }
           else
           {
               return get<index - 1, requested_t>(s.tail);
           }
       }
       else
       {
           return get<index, requested_t>(s.tail);
       }
   }

   template <std::size_t alignment, typename data_t>
   [[nodiscard]] auto align(data_t *const pointer) noexcept
   {
       auto align_offset = static_cast<std::size_t>(reinterpret_cast<uintptr_t>(pointer) & (alignment - 1));
       if (align_offset != 0)
       {
           align_offset = alignment - align_offset;
       }

       return reinterpret_cast<data_t *>(reinterpret_cast<byte_t *>(pointer) + align_offset);
   }

}  // namespace dml::detail::ml::utils

#endif  //DML_DETAIL_ML_UTILS_HPP
