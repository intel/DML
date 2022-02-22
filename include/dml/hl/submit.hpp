/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @date 05/20/2021
 * @brief Contains @ref submit definition
 */

#ifndef DML_SUBMIT_HPP
#define DML_SUBMIT_HPP

#include <dml/detail/ml/make_task.hpp>
#include <dml/hl/detail/handler.hpp>
#include <dml/hl/detail/submit.hpp>
#include <dml/hl/detail/utils.hpp>
#include <dml/hl/execution_interface.hpp>
#include <dml/hl/operations.hpp>
#include <dml/hl/sequence.hpp>

namespace dml
{
    /**
     * @ingroup dmlhl_submit
     * @{
     */

    /**
     * @brief Submits Batch operation on a specified execution path
     *
     * Submit function used to start an operation asynchronously
     * Asynchronous execution is controlled by @ref execution_interface
     *
     * See @ref batch_operation for algorithm details
     *
     * @tparam execution_path        Type of @ref dmlhl_aux_path
     * @tparam sequence_allocator_t  Type of @ref sequence allocator
     * @tparam execution_interface_t Type of @ref execution_interface
     * @param operation              Instance of @ref batch_operation
     * @param seq                    Instance of @ref sequence filled with operations
     * @param executor               Instance of @ref execution_interface
     * @param numa_id                Custom numa id for submission
     *
     * Usage (software execution path):
     * @code
     * auto handler = dml::submit<dml::software>(dml::batch, sequence);
     * // Some code...
     * auto result = handler.get();
     * @endcode
     * Usage (hardware execution path):
     * @code
     * auto handler = dml::submit<dml::hardware>(dml::batch, sequence);
     * // Some code...
     * auto result = handler.get();
     * @endcode
     *
     * @note To use custom @ref execution_interface pass its instance as the last argument to this function
     *
     * @return @ref handler for @ref batch_operation
     */
    template <typename execution_path,
              typename sequence_allocator_t,
              typename execution_interface_t = default_execution_interface<execution_path>>
    inline auto submit(batch_operation                       operation,
                       const sequence<sequence_allocator_t> &seq,
                       const execution_interface_t          &executor = execution_interface_t(),
                       std::uint32_t numa_id = std::numeric_limits<std::uint32_t>::max())
        -> handler<batch_operation, typename execution_interface_t::allocator_type>
    {
        return detail::submit<execution_path, batch_operation>(
            numa_id,
            executor,
            [&]
            {
                return detail::ml::make_batch_task(seq.data(), seq.length(), operation.get_options(), executor.get_allocator());
            });
    }

    /**
     * @brief Submits Memory Move operation on a specified execution path
     *
     * Submit function used to start an operation asynchronously
     * Asynchronous execution is controlled by @ref execution_interface
     *
     * See @ref mem_move_operation for algorithm details
     *
     * @tparam execution_path        Type of @ref dmlhl_aux_path
     * @tparam execution_interface_t Type of @ref execution_interface
     * @param operation              Instance of @ref mem_move_operation
     * @param src_view               @ref data_view to the source memory region
     * @param dst_view               @ref data_view to the destination memory region
     * @param executor               Instance of @ref execution_interface
     * @param numa_id                Custom numa id for submission
     *
     * Usage (software execution path):
     * @code
     * auto handler = dml::submit<dml::software>(dml::mem_move, dml::make_view(src), dml::make_view(dst));
     * // Some code...
     * auto result = handler.get();
     * @endcode
     * Usage (hardware execution path):
     * @code
     * auto handler = dml::submit<dml::hardware>(dml::mem_move, dml::make_view(src), dml::make_view(dst));
     * // Some code...
     * auto result = handler.get();
     * @endcode
     *
     * @note To use custom @ref execution_interface pass its instance as the last argument to this function
     *
     * @return @ref handler for @ref mem_move_operation
     */
    template <typename execution_path, typename execution_interface_t = default_execution_interface<execution_path>>
    inline auto submit(mem_move_operation           operation,
                       const_data_view              src_view,
                       data_view                    dst_view,
                       const execution_interface_t &executor = execution_interface_t(),
                       std::uint32_t numa_id = std::numeric_limits<std::uint32_t>::max())
        -> handler<mem_move_operation, typename execution_interface_t::allocator_type>
    {
        return detail::submit<execution_path, mem_move_operation>(
            numa_id,
            executor,
            [&]
            {
                return detail::ml::make_mem_move_task(src_view.data(), dst_view.data(), src_view.size(), operation.get_options(), executor.get_allocator());
            },
            [&]
            {
                DML_VALIDATE_SIZE_CONSISTENCY(src_view.size(), dst_view.size());
                return status_code::ok;
            });
    }

    /**
     * @brief Submits Memory Copy operation on a specified execution path
     *
     * Submit function used to start an operation asynchronously
     * Asynchronous execution is controlled by @ref execution_interface
     *
     * See @ref mem_copy_operation for algorithm details
     *
     * @tparam execution_path        Type of @ref dmlhl_aux_path
     * @tparam execution_interface_t Type of @ref execution_interface
     * @param operation              Instance of @ref mem_copy_operation
     * @param src_view               @ref data_view to the source memory region
     * @param dst_view               @ref data_view to the destination memory region
     * @param executor               Instance of @ref execution_interface
     * @param numa_id                Custom numa id for submission
     *
     * Usage (software execution path):
     * @code
     * auto handler = dml::submit<dml::software>(dml::mem_copy, dml::make_view(src), dml::make_view(dst));
     * // Some code...
     * auto result = handler.get();
     * @endcode
     * Usage (hardware execution path):
     * @code
     * auto handler = dml::submit<dml::hardware>(dml::mem_copy, dml::make_view(src), dml::make_view(dst));
     * // Some code...
     * auto result = handler.get();
     * @endcode
     *
     * @note To use custom @ref execution_interface pass its instance as the last argument to this function
     *
     * @return @ref handler for @ref mem_copy_operation
     */
    template <typename execution_path, typename execution_interface_t = default_execution_interface<execution_path>>
    inline auto submit(mem_copy_operation           operation,
                       const_data_view              src_view,
                       data_view                    dst_view,
                       const execution_interface_t &executor = execution_interface_t(),
                       std::uint32_t numa_id = std::numeric_limits<std::uint32_t>::max())
        -> handler<mem_copy_operation, typename execution_interface_t::allocator_type>
    {
        return detail::submit<execution_path, mem_copy_operation>(
            numa_id,
            executor,
            [&]
            {
                return detail::ml::make_mem_move_task(src_view.data(), dst_view.data(), src_view.size(), operation.get_options(), executor.get_allocator());
            },
            [&]
            {
                DML_VALIDATE_SIZE_CONSISTENCY(src_view.size(), dst_view.size());
                return status_code::ok;
            });
    }

    /**
     * @brief Submits Fill operation on a specified execution path
     *
     * Submit function used to start an operation asynchronously
     * Asynchronous execution is controlled by @ref execution_interface
     *
     * See @ref fill_operation for algorithm details
     *
     * @tparam execution_path        Type of @ref dmlhl_aux_path
     * @tparam execution_interface_t Type of @ref execution_interface
     * @param operation              Instance of @ref fill_operation
     * @param pattern                64-bit pattern used to fill the destination
     * @param dst_view               @ref data_view to the destination memory region
     * @param executor               Instance of @ref execution_interface
     * @param numa_id                Custom numa id for submission
     *
     * Usage (software execution path):
     * @code
     * auto handler = dml::submit<dml::software>(dml::fill, pattern, dml::make_view(dst));
     * // Some code...
     * auto result = handler.get();
     * @endcode
     * Usage (hardware execution path):
     * @code
     * auto handler = dml::submit<dml::hardware>(dml::fill, pattern, dml::make_view(dst));
     * // Some code...
     * auto result = handler.get();
     * @endcode
     *
     * @note To use custom @ref execution_interface pass its instance as the last argument to this function
     *
     * @return @ref handler for @ref fill_operation
     */
    template <typename execution_path, typename execution_interface_t = default_execution_interface<execution_path>>
    inline auto submit(fill_operation               operation,
                       uint64_t                     pattern,
                       data_view                    dst_view,
                       const execution_interface_t &executor = execution_interface_t(),
                       std::uint32_t numa_id = std::numeric_limits<std::uint32_t>::max())
        -> handler<fill_operation, typename execution_interface_t::allocator_type>
    {
        return detail::submit<execution_path, fill_operation>(
            numa_id,
            executor,
            [&]
            {
                return detail::ml::make_fill_task(pattern, dst_view.data(), dst_view.size(), operation.get_options(), executor.get_allocator());
            });
    }

    /**
     * @brief Submits Dualcast operation on a specified execution path
     *
     * Submit function used to start an operation asynchronously
     * Asynchronous execution is controlled by @ref execution_interface
     *
     * See @ref dualcast_operation for algorithm details
     *
     * @tparam execution_path        Type of @ref dmlhl_aux_path
     * @tparam execution_interface_t Type of @ref execution_interface
     * @param operation              Instance of @ref dualcast_operation
     * @param src_view               @ref data_view to the source memory region
     * @param dst1_view              @ref data_view to the first destination memory region
     * @param dst2_view              @ref data_view to the second destination memory region
     * @param executor               Instance of @ref execution_interface
     * @param numa_id                Custom numa id for submission
     *
     * Usage (software execution path):
     * @code
     * auto handler = dml::submit<dml::software>(dml::dualcast, dml::make_view(src), dml::make_view(dst), dml::make_view(dst2));
     * // Some code...
     * auto result = handler.get();
     * @endcode
     * Usage (hardware execution path):
     * @code
     * auto handler = dml::submit<dml::hardware>(dml::dualcast, dml::make_view(src), dml::make_view(dst), dml::make_view(dst2));
     * // Some code...
     * auto result = handler.get();
     * @endcode
     *
     * @note To use custom @ref execution_interface pass its instance as the last argument to this function
     *
     * @return @ref handler for @ref dualcast_operation
     */
    template <typename execution_path, typename execution_interface_t = default_execution_interface<execution_path>>
    inline auto submit(dualcast_operation           operation,
                       const_data_view              src_view,
                       data_view                    dst1_view,
                       data_view                    dst2_view,
                       const execution_interface_t &executor = execution_interface_t(),
                       std::uint32_t numa_id = std::numeric_limits<std::uint32_t>::max())
        -> handler<dualcast_operation, typename execution_interface_t::allocator_type>
    {
        return detail::submit<execution_path, dualcast_operation>(
            numa_id,
            executor,
            [&]
            {
                return detail::ml::make_dualcast_task(src_view.data(),
                                                            dst1_view.data(),
                                                            dst2_view.data(),
                                                            src_view.size(),
                                                            operation.get_options(),
                                                            operation.get_specific_options(), executor.get_allocator());
            },
            [&]
            {
                DML_VALIDATE_SIZE_CONSISTENCY(src_view.size(), dst1_view.size());
                DML_VALIDATE_SIZE_CONSISTENCY(src_view.size(), dst2_view.size());
                return status_code::ok;
            });
    }

    /**
     * @brief Submits Compare operation on a specified execution path
     *
     * Submit function used to start an operation asynchronously
     * Asynchronous execution is controlled by @ref execution_interface
     *
     * See @ref compare_operation for algorithm details
     *
     * @tparam execution_path        Type of @ref dmlhl_aux_path
     * @tparam execution_interface_t Type of @ref execution_interface
     * @param operation              Instance of @ref compare_operation
     * @param src1_view              @ref data_view to the first source memory region
     * @param src2_view              @ref data_view to the second source memory region
     * @param executor               Instance of @ref execution_interface
     * @param numa_id                Custom numa id for submission
     *
     * Usage (software execution path):
     * @code
     * auto handler = dml::submit<dml::software>(dml::compare, dml::make_view(src1), dml::make_view(src2));
     * // Some code...
     * auto result = handler.get();
     * @endcode
     * Usage (hardware execution path):
     * @code
     * auto handler = dml::submit<dml::hardware>(dml::compare, dml::make_view(src1), dml::make_view(src2));
     * // Some code...
     * auto result = handler.get();
     * @endcode
     *
     * @note To use custom @ref execution_interface pass its instance as the last argument to this function
     *
     * @return @ref handler for @ref compare_operation
     */
    template <typename execution_path, typename execution_interface_t = default_execution_interface<execution_path>>
    inline auto submit(compare_operation            operation,
                       const_data_view              src1_view,
                       const_data_view              src2_view,
                       const execution_interface_t &executor = execution_interface_t(),
                       std::uint32_t numa_id = std::numeric_limits<std::uint32_t>::max())
        -> handler<compare_operation, typename execution_interface_t::allocator_type>
    {
        return detail::submit<execution_path, compare_operation>(
            numa_id,
            executor,
            [&]
            {
                return detail::ml::make_compare_task(src1_view.data(),
                                                           src2_view.data(),
                                                           src1_view.size(),
                                                           operation.get_options(),
                                                           operation.get_expected_result(), executor.get_allocator());
            },
            [&]
            {
                DML_VALIDATE_SIZE_CONSISTENCY(src1_view.size(), src2_view.size());
                return status_code::ok;
            });
    }

    /**
     * @brief Submits Compare with Pattern operation on a specified execution path
     *
     * Submit function used to start an operation asynchronously
     * Asynchronous execution is controlled by @ref execution_interface
     *
     * See @ref compare_pattern_operation for algorithm details
     *
     * @tparam execution_path        Type of @ref dmlhl_aux_path
     * @tparam execution_interface_t Type of @ref execution_interface
     * @param operation              Instance of @ref compare_pattern_operation
     * @param pattern                64-bit pattern to compare with the memory region
     * @param src_view               @ref data_view to the source memory region
     * @param executor               Instance of @ref execution_interface
     * @param numa_id                Custom numa id for submission
     *
     * Usage (software execution path):
     * @code
     * auto handler = dml::submit<dml::software>(dml::compare_pattern, pattern, dml::make_view(src));
     * // Some code...
     * auto result = handler.get();
     * @endcode
     * Usage (hardware execution path):
     * @code
     * auto handler = dml::submit<dml::hardware>(dml::compare_pattern, pattern, dml::make_view(src));
     * // Some code...
     * auto result = handler.get();
     * @endcode
     *
     * @note To use custom @ref execution_interface pass its instance as the last argument to this function
     *
     * @return @ref handler for @ref compare_pattern_operation
     */
    template <typename execution_path, typename execution_interface_t = default_execution_interface<execution_path>>
    inline auto submit(compare_pattern_operation    operation,
                       uint64_t                     pattern,
                       const_data_view              src_view,
                       const execution_interface_t &executor = execution_interface_t(),
                       std::uint32_t numa_id = std::numeric_limits<std::uint32_t>::max())
        -> handler<compare_pattern_operation, typename execution_interface_t::allocator_type>
    {
        return detail::submit<execution_path, compare_pattern_operation>(numa_id,
                                                                         executor,
                                                                         [&]
                                                                         {
                                                                             return detail::ml::make_compare_pattern_task(
                                                                                 pattern,
                                                                                 src_view.data(),
                                                                                 src_view.size(),
                                                                                 operation.get_options(),
                                                                                 operation.get_expected_result(), executor.get_allocator());
                                                                         });
    }

    /**
     * @brief Submits Create Delta operation on a specified execution path
     *
     * Submit function used to start an operation asynchronously
     * Asynchronous execution is controlled by @ref execution_interface
     *
     * See @ref create_delta_operation for algorithm details
     *
     * @tparam execution_path        Type of @ref dmlhl_aux_path
     * @tparam execution_interface_t Type of @ref execution_interface
     * @param operation              Instance of @ref create_delta_operation
     * @param src1_view              @ref data_view to the first source memory region
     * @param src2_view              @ref data_view to the second source memory region
     * @param delta_view             @ref data_view to the memory region for delta record
     * @param executor               Instance of @ref execution_interface
     * @param numa_id                Custom numa id for submission
     *
     * Usage (software execution path):
     * @code
     * auto handler = dml::submit<dml::software>dml::create_delta, dml::make_view(src1), dml::make_view(src2), dml::make_view(delta));
     * // Some code...
     * auto result = handler.get();
     * @endcode
     * Usage (hardware execution path):
     * @code
     * auto handler = dml::submit<dml::hardware>dml::create_delta, dml::make_view(src1), dml::make_view(src2), dml::make_view(delta));
     * // Some code...
     * auto result = handler.get();
     * @endcode
     *
     * @note To use custom @ref execution_interface pass its instance as the last argument to this function
     *
     * @return @ref handler for @ref create_delta_operation
     */
    template <typename execution_path, typename execution_interface_t = default_execution_interface<execution_path>>
    inline auto submit(create_delta_operation       operation,
                       const_data_view              src1_view,
                       const_data_view              src2_view,
                       data_view                    delta_view,
                       const execution_interface_t &executor = execution_interface_t(),
                       std::uint32_t numa_id = std::numeric_limits<std::uint32_t>::max())
        -> handler<create_delta_operation, typename execution_interface_t::allocator_type>
    {
        return detail::submit<execution_path, create_delta_operation>(
            numa_id,
            executor,
            [&]
            {
                return detail::ml::make_create_delta_task(src1_view.data(),
                                                                src2_view.data(),
                                                                src1_view.size(),
                                                                delta_view.data(),
                                                                delta_view.size(),
                                                                operation.get_options(),
                                                                operation.get_expected_result(), executor.get_allocator());
            },
            [&]
            {
                DML_VALIDATE_SIZE_CONSISTENCY(src1_view.size(), src2_view.size());
                return status_code::ok;
            });
    }

    /**
     * @brief Submits Apply Delta operation on a specified execution path
     *
     * Submit function used to start an operation asynchronously
     * Asynchronous execution is controlled by @ref execution_interface
     *
     * See @ref apply_delta_operation for algorithm details
     *
     * @tparam execution_path        Type of @ref dmlhl_aux_path
     * @tparam execution_interface_t Type of @ref execution_interface
     * @param operation              Instance of @ref apply_delta_operation
     * @param delta_view             @ref data_view to the memory region with delta record
     * @param dst_view               @ref data_view to the destination memory region (the first source from Create Delta)
     * @param delta_result           Result from Create Delta operation
     * @param executor               Instance of @ref execution_interface
     * @param numa_id                Custom numa id for submission
     *
     * Usage (software execution path):
     * @code
     * auto handler = dml::submit<dml::software>(dml::apply_delta, dml::make_view(delta), dml::make_view(src1), create_result);
     * // Some code...
     * auto result = handler.get();
     * @endcode
     * Usage (hardware execution path):
     * @code
     * auto handler = dml::submit<dml::hardware>(dml::apply_delta, dml::make_view(delta), dml::make_view(src1), create_result);
     * // Some code...
     * auto result = handler.get();
     * @endcode
     *
     * @note To use custom @ref execution_interface pass its instance as the last argument to this function
     *
     * @return @ref handler for @ref apply_delta_operation
     */
    template <typename execution_path, typename execution_interface_t = default_execution_interface<execution_path>>
    inline auto submit(apply_delta_operation        operation,
                       const_data_view              delta_view,
                       data_view                    dst_view,
                       create_delta_result          delta_result,
                       const execution_interface_t &executor = execution_interface_t(),
                       std::uint32_t numa_id = std::numeric_limits<std::uint32_t>::max())
        -> handler<apply_delta_operation, typename execution_interface_t::allocator_type>
    {
        return detail::submit<execution_path, apply_delta_operation>(
            numa_id,
            executor,
            [&]
            {
                return detail::ml::make_apply_delta_task(delta_view.data(),
                                                               delta_result.delta_record_size,
                                                               dst_view.data(),
                                                               dst_view.size(),
                                                               operation.get_options(), executor.get_allocator());
            },
            [&]
            {
                if (delta_result.result != comparison_result::not_equal)
                {
                    return status_code::delta_delta_empty;
                }
                return status_code::ok;
            });
    }

    /**
     * @brief Submits CRC operation on a specified execution path
     *
     * Submit function used to start an operation asynchronously
     * Asynchronous execution is controlled by @ref execution_interface
     *
     * See @ref crc_operation for algorithm details
     *
     * @tparam execution_path        Type of @ref dmlhl_aux_path
     * @tparam execution_interface_t Type of @ref execution_interface
     * @param operation              Instance of @ref crc_operation
     * @param src_view               @ref data_view to the source memory region
     * @param crc_seed               Initial CRC value
     * @param executor               Instance of @ref execution_interface
     * @param numa_id                Custom numa id for submission
     *
     * Usage (software execution path):
     * @code
     * auto handler = dml::submit<dml::software>(dml::crc, dml::make_view(src), crc_seed);
     * // Some code...
     * auto result = handler.get();
     * @endcode
     * Usage (hardware execution path):
     * @code
     * auto handler = dml::submit<dml::hardware>(dml::crc, dml::make_view(src), crc_seed);
     * // Some code...
     * auto result = handler.get();
     * @endcode
     *
     * @note To use custom @ref execution_interface pass its instance as the last argument to this function
     *
     * @return @ref handler for @ref crc_operation
     */
    template <typename execution_path, typename execution_interface_t = default_execution_interface<execution_path>>
    inline auto submit(crc_operation                operation,
                       const_data_view              src_view,
                       uint32_t                     crc_seed,
                       const execution_interface_t &executor = execution_interface_t(),
                       std::uint32_t numa_id = std::numeric_limits<std::uint32_t>::max())
        -> handler<crc_operation, typename execution_interface_t::allocator_type>
    {
        return detail::submit<execution_path, crc_operation>(numa_id,
                                                             executor,
                                                             [&]
                                                             {
                                                                 return detail::ml::make_crc_task(src_view.data(),
                                                                                                        src_view.size(),
                                                                                                        crc_seed,
                                                                                                        operation.get_options(),
                                                                                                        operation.get_specific_options(), executor.get_allocator());
                                                             });
    }

    /**
     * @brief Submits Copy + CRC operation on a specified execution path
     *
     * Submit function used to start an operation asynchronously
     * Asynchronous execution is controlled by @ref execution_interface
     *
     * See @ref copy_crc_operation for algorithm details
     *
     * @tparam execution_path        Type of @ref dmlhl_aux_path
     * @tparam execution_interface_t Type of @ref execution_interface
     * @param operation              Instance of @ref copy_crc_operation
     * @param src_view               @ref data_view to the source memory region
     * @param dst_view               @ref data_view to the destination memory region
     * @param crc_seed               Initial CRC value
     * @param executor               Instance of @ref execution_interface
     * @param numa_id                Custom numa id for submission
     *
     * Usage (software execution path):
     * @code
     * auto handler = dml::submit<dml::software>(dml::copy_crc, dml::make_view(src), dml::make_view(dst), crc_seed);
     * // Some code...
     * auto result = handler.get();
     * @endcode
     * Usage (hardware execution path):
     * @code
     * auto handler = dml::submit<dml::hardware>(dml::copy_crc, dml::make_view(src), dml::make_view(dst), crc_seed);
     * // Some code...
     * auto result = handler.get();
     * @endcode
     *
     * @note To use custom @ref execution_interface pass its instance as the last argument to this function
     *
     * @return @ref handler for @ref copy_crc_operation
     */
    template <typename execution_path, typename execution_interface_t = default_execution_interface<execution_path>>
    inline auto submit(copy_crc_operation           operation,
                       const_data_view              src_view,
                       data_view                    dst_view,
                       uint32_t                     crc_seed,
                       const execution_interface_t &executor = execution_interface_t(),
                       std::uint32_t numa_id = std::numeric_limits<std::uint32_t>::max())
        -> handler<copy_crc_operation, typename execution_interface_t::allocator_type>
    {
        return detail::submit<execution_path, copy_crc_operation>(
            numa_id,
            executor,
            [&]
            {
                return detail::ml::make_copy_crc_task(src_view.data(),
                                                            dst_view.data(),
                                                            src_view.size(),
                                                            crc_seed,
                                                            operation.get_options(),
                                                            operation.get_specific_options(), executor.get_allocator());
            },
            [&]
            {
                DML_VALIDATE_SIZE_CONSISTENCY(src_view.size(), dst_view.size());
                return status_code::ok;
            });
    }

    /**
     * @brief Submits Cache Flush operation on a specified execution path
     *
     * Submit function used to start an operation asynchronously
     * Asynchronous execution is controlled by @ref execution_interface
     *
     * See @ref cache_flush_operation for algorithm details
     *
     * @tparam execution_path        Type of @ref dmlhl_aux_path
     * @tparam execution_interface_t Type of @ref execution_interface
     * @param operation              Instance of @ref cache_flush_operation
     * @param dst_view               @ref data_view to the destination memory region
     * @param executor               Instance of @ref execution_interface
     * @param numa_id                Custom numa id for submission
     *
     * Usage (software execution path):
     * @code
     * auto handler = dml::submit<dml::software>(dml::cache_flush, dml::make_view(dst));
     * // Some code...
     * auto result = handler.get();
     * @endcode
     * Usage (hardware execution path):
     * @code
     * auto handler = dml::submit<dml::hardware>(dml::cache_flush, dml::make_view(dst));
     * // Some code...
     * auto result = handler.get();
     * @endcode
     *
     * @note To use custom @ref execution_interface pass its instance as the last argument to this function
     *
     * @return @ref handler for @ref cache_flush_operation
     */
    template <typename execution_path, typename execution_interface_t = default_execution_interface<execution_path>>
    inline auto submit(cache_flush_operation operation,
                       data_view dst_view,
                       const execution_interface_t &executor = execution_interface_t(),
                       std::uint32_t numa_id = std::numeric_limits<std::uint32_t>::max())
        -> handler<cache_flush_operation, typename execution_interface_t::allocator_type>
    {
        return detail::submit<execution_path, cache_flush_operation>(
            numa_id,
            executor,
            [&]
            {
                return detail::ml::make_cache_flush_task(dst_view.data(), dst_view.size(), operation.get_options(), executor.get_allocator());
            });
    }
}  // namespace dml

#endif  //DML_SUBMIT_HPP
