/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_DETAIL_ML_VIEW_HPP
#define DML_DETAIL_ML_VIEW_HPP

#include <dml/detail/ml/buffer.hpp>
#include <dml/detail/ml/task.hpp>
#include <dml/detail/ml/impl/core_interconnect.hpp>

namespace dml::detail::ml
{
   template <typename buffer_t>
   class buffer_view
   {
       using element_type   = typename buffer_t::element_type;
       using allocator_type = typename buffer_t::allocator_type;

   public:
       explicit buffer_view(buffer_t& some_buffer) noexcept: element_(nullptr)
       {
           if constexpr (std::is_same_v<allocator_type, stack_allocator>)
           {
               element_ = &some_buffer.element_;
           }
           else
           {
               element_ = some_buffer.aligned_memory_;
           }
       }

       template <size_t index, typename requested_t>
       [[nodiscard]] auto& get() noexcept
       {
           return utils::get<index, requested_t>(*element_);
       }

   private:
       element_type* element_;
   };

   template <typename allocator_t, typename... elements_t>
   auto make_view(buffer<allocator_t, elements_t...>& some_buffer) noexcept
   {
       return buffer_view(some_buffer);
   }

   class task_view
   {
   public:
       // TODO: Remove this constructor and use the next one only
       template <typename task_t>
       explicit task_view(task_t& some_task) noexcept:
           descriptor_(make_view(some_task.buffer_).template get<0, descriptor>()),
           completion_record_(make_view(some_task.buffer_).template get<0, completion_record>())
       {
       }

       task_view(descriptor& dsc, completion_record& record) noexcept: descriptor_(dsc), completion_record_(record)
       {
       }

       // TODO: Probably nice to add `operator tuple<dsc&, record&>` to task itself
       // TODO: Task class is strange, as it just container for buffer, but buffer is to be used for arrays of task
       template <typename task_t>
       task_view& operator=(task_t&& some_task) noexcept
       {
           descriptor_        = make_view(some_task.buffer_).template get<0, descriptor>();
           completion_record_ = make_view(some_task.buffer_).template get<0, completion_record>();

           impl::rebind(descriptor_, completion_record_);

           return *this;
       }

       [[nodiscard]] auto& get_descriptor() noexcept
       {
           return descriptor_;
       }

       [[nodiscard]] auto& get_completion_record() noexcept
       {
           return completion_record_;
       }

   private:
       descriptor&        descriptor_;
       completion_record& completion_record_;
   };

   template <typename allocator_t>
   auto make_view(task<allocator_t>& some_task) noexcept
   {
       return task_view(some_task);
   }

   [[nodiscard]] static inline auto make_view(descriptor& dsc, completion_record& record) noexcept
   {
       return task_view(dsc, record);
   }

}  // namespace dml::detail::ml

#endif  //DML_DETAIL_ML_VIEW_HPP
