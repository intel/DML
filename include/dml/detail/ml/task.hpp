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

#ifndef DML_DETAIL_ML_TASK_HPP
#define DML_DETAIL_ML_TASK_HPP

#include <dml/detail/ml/allocator.hpp>
#include <dml/detail/ml/buffer.hpp>
#include <dml/detail/ml/impl/core_interconnect.hpp>
#include <dml/detail/ml/result.hpp>

namespace dml::detail::ml
{
   class task_view;

   template <typename allocator_t>
   class task
   {
       using buffer_type = buffer<allocator_t, descriptor, completion_record>;

       friend task_view;

       static constexpr bool is_stack = std::is_same_v<allocator_t, stack_allocator>;

   public:
       explicit task(allocator_t allocator = allocator_t()) noexcept(is_stack): buffer_(allocator)
       {
       }

       explicit task(const descriptor& dsc, allocator_t allocator) noexcept(is_stack): buffer_(allocator)
       {
           make_view(buffer_).template get<0, descriptor>() = dsc;

           rebind_completion_record();
       }

       task(const task& other) = delete;

       task(task&& other) noexcept: buffer_(std::move(other.buffer_))
       {
           if constexpr (is_stack)
           {
               rebind_completion_record();
           }
       }

       task& operator=(const task& other) = delete;

       task& operator=(task&& other) noexcept
       {
           if (this != &other)
           {
               buffer_ = std::move(other.buffer_);
               if constexpr (is_stack)
               {
                   rebind_completion_record();
               }
           }

           return *this;
       }

   private:
       void rebind_completion_record() noexcept
       {
           impl::rebind(make_view(buffer_).template get<0, descriptor>(), make_view(buffer_).template get<0, completion_record>());
       }

   private:
       buffer_type buffer_;
   };
}  // namespace dml::detail::ml

#endif  //DML_DETAIL_ML_TASK_HPP
