/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_ML_MAKE_TASK_HPP
#define DML_ML_MAKE_TASK_HPP

#include <dml/detail/common/types.hpp>
#include <dml/detail/ml/impl/make_descriptor.hpp>
#include <dml/detail/ml/options.hpp>
#include <dml/detail/ml/task.hpp>

namespace dml::detail::ml
{
    template <typename allocator_t>
    static constexpr auto is_stack = std::is_same_v<allocator_t, stack_allocator>;

    template <typename allocator_t = stack_allocator>
    [[nodiscard]] auto make_nop_task(nop_options options, allocator_t allocator = allocator_t()) noexcept(is_stack<allocator_t>)
    {
        return task<allocator_t>(impl::make_nop_descriptor(options), allocator);
    }

    template <typename allocator_t = stack_allocator>
    [[nodiscard]] auto make_drain_task(address_t              readback_address_1,
                                       address_t              readback_address_2,
                                       drain_options          options,
                                       drain_specific_options specific_options,
                                       allocator_t            allocator = allocator_t()) noexcept(is_stack<allocator_t>)
    {
        return task<allocator_t>(impl::make_drain_descriptor(readback_address_1, readback_address_2, options, specific_options), allocator);
    }

    template <typename allocator_t = stack_allocator>
    [[nodiscard]] auto make_mem_move_task(const byte_t    *src,
                                          byte_t          *dst,
                                          transfer_size_t  size,
                                          mem_move_options options,
                                          allocator_t      allocator = allocator_t()) noexcept(is_stack<allocator_t>)
    {
        return task<allocator_t>(impl::make_mem_move_descriptor(src, dst, size, options), allocator);
    }

    template <typename allocator_t = stack_allocator>
    [[nodiscard]] auto make_fill_task(uint64_t        pattern,
                                      byte_t         *dst,
                                      transfer_size_t size,
                                      fill_options    options,
                                      allocator_t     allocator = allocator_t()) noexcept(is_stack<allocator_t>)
    {
        return task<allocator_t>(impl::make_fill_descriptor(pattern, dst, size, options), allocator);
    }

    template <typename allocator_t = stack_allocator>
    [[nodiscard]] auto make_dualcast_task(const byte_t             *src,
                                          byte_t                   *dst1,
                                          byte_t                   *dst2,
                                          transfer_size_t           size,
                                          dualcast_options          options,
                                          dualcast_specific_options specific_options,
                                          allocator_t               allocator = allocator_t()) noexcept(is_stack<allocator_t>)
    {
        return task<allocator_t>(impl::make_dualcast_descriptor(src, dst1, dst2, size, options, specific_options), allocator);
    }

    template <typename allocator_t = stack_allocator>
    [[nodiscard]] auto make_compare_task(const byte_t   *src1,
                                         const byte_t   *src2,
                                         transfer_size_t size,
                                         compare_options options,
                                         compare_result  expected_result,
                                         allocator_t     allocator = allocator_t()) noexcept(is_stack<allocator_t>)
    {
        return task<allocator_t>(impl::make_compare_descriptor(src1, src2, size, options, expected_result), allocator);
    }

    template <typename allocator_t = stack_allocator>
    [[nodiscard]] auto make_compare_pattern_task(uint64_t                pattern,
                                                 const byte_t           *src,
                                                 transfer_size_t         size,
                                                 compare_pattern_options options,
                                                 compare_result          expected_result,
                                                 allocator_t             allocator = allocator_t()) noexcept(is_stack<allocator_t>)
    {
        return task<allocator_t>(impl::make_compare_pattern_descriptor(pattern, src, size, options, expected_result), allocator);
    }

    template <typename allocator_t = stack_allocator>
    [[nodiscard]] auto make_crc_task(const byte_t        *src,
                                     transfer_size_t      size,
                                     crc_value_t          crc_seed,
                                     crc_options          options,
                                     crc_specific_options specific_options,
                                     allocator_t          allocator = allocator_t()) noexcept(is_stack<allocator_t>)
    {
        return task<allocator_t>(impl::make_crc_descriptor(src, size, crc_seed, options, specific_options), allocator);
    }

    template <typename allocator_t = stack_allocator>
    [[nodiscard]] auto make_copy_crc_task(const byte_t             *src,
                                          byte_t                   *dst,
                                          transfer_size_t           size,
                                          crc_value_t               crc_seed,
                                          copy_crc_options          options,
                                          copy_crc_specific_options specific_options,
                                          allocator_t               allocator = allocator_t()) noexcept(is_stack<allocator_t>)
    {
        return task<allocator_t>(impl::make_copy_crc_descriptor(src, dst, size, crc_seed, options, specific_options), allocator);
    }

    template <typename allocator_t = stack_allocator>
    [[nodiscard]] auto make_create_delta_task(const byte_t        *src1,
                                              const byte_t        *src2,
                                              transfer_size_t      size,
                                              byte_t              *delta_record,
                                              transfer_size_t      delta_max_size,
                                              create_delta_options options,
                                              create_delta_result  expected_result,
                                              allocator_t          allocator = allocator_t()) noexcept(is_stack<allocator_t>)
    {
        return task<allocator_t>(
            impl::make_create_delta_descriptor(src1, src2, size, delta_record, delta_max_size, options, expected_result),
            allocator);
    }

    template <typename allocator_t = stack_allocator>
    [[nodiscard]] auto make_apply_delta_task(const byte_t       *delta_record,
                                             transfer_size_t     delta_size,
                                             byte_t             *dst,
                                             transfer_size_t     size,
                                             apply_delta_options options,
                                             allocator_t         allocator = allocator_t()) noexcept(is_stack<allocator_t>)
    {
        return task<allocator_t>(impl::make_apply_delta_descriptor(delta_record, delta_size, dst, size, options), allocator);
    }

    template <typename allocator_t = stack_allocator>
    [[nodiscard]] auto make_cache_flush_task(byte_t             *dst,
                                             transfer_size_t     size,
                                             cache_flush_options options,
                                             allocator_t         allocator = allocator_t()) noexcept(is_stack<allocator_t>)
    {
        return task<allocator_t>(impl::make_cache_flush_descriptor(dst, size, options), allocator);
    }

    template <typename allocator_t = stack_allocator>
    [[nodiscard]] auto make_dif_check_task(const byte_t        *src,
                                           transfer_size_t      transfer_size,
                                           dif_parameters       src_parameters,
                                           dif_check_options    options,
                                           dif_specific_options specific_options,
                                           dif_source_options   source_options,
                                           allocator_t          allocator = allocator_t()) noexcept(is_stack<allocator_t>)
    {
        return task<allocator_t>(
            impl::make_dif_check_descriptor(src, transfer_size, src_parameters, options, specific_options, source_options),
            allocator);
    }

    template <typename allocator_t = stack_allocator>
    [[nodiscard]] auto make_dif_insert_task(const byte_t           *src,
                                            byte_t                 *dst,
                                            transfer_size_t         transfer_size,
                                            dif_parameters          dst_parameters,
                                            dif_insert_options      options,
                                            dif_specific_options    specific_options,
                                            dif_destination_options destination_options,
                                            allocator_t             allocator = allocator_t()) noexcept(is_stack<allocator_t>)
    {
        return task<allocator_t>(
            impl::make_dif_insert_descriptor(src, dst, transfer_size, dst_parameters, options, specific_options, destination_options),
            allocator);
    }

    template <typename allocator_t = stack_allocator>
    [[nodiscard]] auto make_dif_strip_task(const byte_t        *src,
                                           byte_t              *dst,
                                           transfer_size_t      transfer_size,
                                           dif_parameters       src_parameters,
                                           dif_strip_options    options,
                                           dif_specific_options specific_options,
                                           dif_source_options   source_options,
                                           allocator_t          allocator = allocator_t()) noexcept(is_stack<allocator_t>)
    {
        return task<allocator_t>(
            impl::make_dif_strip_descriptor(src, dst, transfer_size, src_parameters, options, specific_options, source_options),
            allocator);
    }

    template <typename allocator_t = stack_allocator>
    [[nodiscard]] auto make_dif_update_task(const byte_t           *src,
                                            byte_t                 *dst,
                                            transfer_size_t         transfer_size,
                                            dif_parameters          src_parameters,
                                            dif_parameters          dst_parameters,
                                            dif_update_options      options,
                                            dif_specific_options    specific_options,
                                            dif_source_options      source_options,
                                            dif_destination_options destination_options,
                                            allocator_t             allocator = allocator_t()) noexcept(is_stack<allocator_t>)
    {
        return task<allocator_t>(impl::make_dif_update_descriptor(src,
                                                                  dst,
                                                                  transfer_size,
                                                                  src_parameters,
                                                                  dst_parameters,
                                                                  options,
                                                                  specific_options,
                                                                  source_options,
                                                                  destination_options),
                                 allocator);
    }

    template <typename allocator_t = stack_allocator>
    [[nodiscard]] auto make_batch_task(const descriptor *src,
                                       transfer_size_t   length,
                                       batch_options     options,
                                       allocator_t       allocator = allocator_t()) noexcept(is_stack<allocator_t>)
    {
        return task<allocator_t>(impl::make_batch_descriptor(src, length, options), allocator);
    }
}  // namespace dml::detail::ml

#endif  //DML_ML_MAKE_TASK_HPP
