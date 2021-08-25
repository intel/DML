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

/**
 * @date 05/20/2021
 * @brief Contains @ref submit definition
 */

#ifndef DML_SUBMIT_HPP
#define DML_SUBMIT_HPP

#include <dml/operations.hpp>
#include <dml/sequence.hpp>
#include <dml_common/range_check.hpp>
#include <dml_ml/apply_delta.hpp>
#include <dml_ml/batch.hpp>
#include <dml_ml/cache_flush.hpp>
#include <dml_ml/compare.hpp>
#include <dml_ml/compare_pattern.hpp>
#include <dml_ml/copy_crc.hpp>
#include <dml_ml/crc.hpp>
#include <dml_ml/create_delta.hpp>
#include <dml_ml/dualcast.hpp>
#include <dml_ml/fill.hpp>
#include <dml_ml/mem_move.hpp>
#include <dml_ml/mem_copy.hpp>

#include <dml/detail/handler.hpp>
#include <dml/detail/submit.hpp>
#include <dml/detail/utils.hpp>
#include <dml/execution_interface.hpp>

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
                       const execution_interface_t &         executor = execution_interface_t())
        -> handler<batch_operation, typename execution_interface_t::allocator_type>
    {
        static_cast<void>(operation);

        return detail::submit<execution_path, batch_operation>(
            executor,
            [&]
            {
                return range_check::batch(seq.data(), seq.length());
            },
            [&]()
            {
                return ml::batch(seq.data(), seq.length());
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
                       const execution_interface_t &executor = execution_interface_t())
        -> handler<mem_move_operation, typename execution_interface_t::allocator_type>
    {
        static_cast<void>(operation);

        return detail::submit<execution_path, mem_move_operation>(
            executor,
            [&]
            {
                DML_VALIDATE_SIZE_CONSISTENCY(src_view.size(), dst_view.size());
                return range_check::mem_move(src_view.data(), dst_view.data(), src_view.size());
            },
            [&]()
            {
                return ml::mem_move(src_view.data(), dst_view.data(), src_view.size());
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
                       const execution_interface_t &executor = execution_interface_t())
        -> handler<mem_copy_operation, typename execution_interface_t::allocator_type>
    {
        static_cast<void>(operation);

        return detail::submit<execution_path, mem_copy_operation>(
            executor,
            [&]
            {
                DML_VALIDATE_SIZE_CONSISTENCY(src_view.size(), dst_view.size());
                return range_check::mem_copy(src_view.data(), dst_view.data(), src_view.size());
            },
            [&]()
            {
                return ml::mem_copy(src_view.data(), dst_view.data(), src_view.size());
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
                       const execution_interface_t &executor = execution_interface_t())
        -> handler<fill_operation, typename execution_interface_t::allocator_type>
    {
        static_cast<void>(operation);

        return detail::submit<execution_path, fill_operation>(
            executor,
            [&]
            {
                return range_check::fill(dst_view.data(), dst_view.size());
            },
            [&]()
            {
                return ml::fill(pattern, dst_view.data(), dst_view.size());
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
                       const execution_interface_t &executor = execution_interface_t())
        -> handler<dualcast_operation, typename execution_interface_t::allocator_type>
    {
        static_cast<void>(operation);

        return detail::submit<execution_path, dualcast_operation>(
            executor,
            [&]
            {
                DML_VALIDATE_SIZE_CONSISTENCY(src_view.size(), dst1_view.size());
                DML_VALIDATE_SIZE_CONSISTENCY(src_view.size(), dst2_view.size());
                return range_check::dualcast(src_view.data(), dst1_view.data(), dst2_view.data(), src_view.size());
            },
            [&]()
            {
                return ml::dualcast(src_view.data(), dst1_view.data(), dst2_view.data(), src_view.size());
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
                       const execution_interface_t &executor = execution_interface_t())
        -> handler<compare_operation, typename execution_interface_t::allocator_type>
    {
        return detail::submit<execution_path, compare_operation>(
            executor,
            [&]
            {
                DML_VALIDATE_SIZE_CONSISTENCY(src1_view.size(), src2_view.size());
                return range_check::compare(src1_view.data(), src2_view.data(), src1_view.size());
            },
            [&]()
            {
                return ml::compare(src1_view.data(),
                                   src2_view.data(),
                                   src1_view.size(),
                                   operation.get_expected_result());
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
                       const execution_interface_t &executor = execution_interface_t())
        -> handler<compare_pattern_operation, typename execution_interface_t::allocator_type>
    {
        return detail::submit<execution_path, compare_pattern_operation>(
            executor,
            [&]
            {
                return range_check::compare_pattern(src_view.data(), src_view.size());
            },
            [&]()
            {
                return ml::compare_pattern(pattern,
                                           src_view.data(),
                                           src_view.size(),
                                           operation.get_expected_result());
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
                       const execution_interface_t &executor = execution_interface_t())
        -> handler<create_delta_operation, typename execution_interface_t::allocator_type>
    {
        static_cast<void>(operation);

        return detail::submit<execution_path, create_delta_operation>(
            executor,
            [&]
            {
                DML_VALIDATE_SIZE_CONSISTENCY(src1_view.size(), src2_view.size());
                return range_check::create_delta(
                    src1_view.data(), src2_view.data(), src1_view.size(), delta_view.data(), delta_view.size());
            },
            [&]()
            {
                return ml::create_delta(
                    src1_view.data(), src2_view.data(), src1_view.size(), delta_view.data(), delta_view.size());
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
                       const execution_interface_t &executor = execution_interface_t())
        -> handler<apply_delta_operation, typename execution_interface_t::allocator_type>
    {
        static_cast<void>(operation);

        return detail::submit<execution_path, apply_delta_operation>(
            executor,
            [&]
            {
                if (delta_result.result != 1)
                {
                    return status_code::delta_delta_empty;
                }

                return range_check::apply_delta(delta_view.data(),
                                                delta_result.delta_record_size,
                                                dst_view.data(),
                                                dst_view.size());
            },
            [&]()
            {
                return ml::apply_delta(
                    delta_view.data(), delta_result.delta_record_size, dst_view.data(), dst_view.size());
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
                       const execution_interface_t &executor = execution_interface_t())
        -> handler<crc_operation, typename execution_interface_t::allocator_type>
    {
        return detail::submit<execution_path, crc_operation>(
            executor,
            [&]
            {
                return range_check::crc(src_view.data(), src_view.size());
            },
            [&]()
            {
                return ml::crc(src_view.data(), src_view.size(), crc_seed, operation.get_params());
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
                       const execution_interface_t &executor = execution_interface_t())
        -> handler<copy_crc_operation, typename execution_interface_t::allocator_type>
    {
        return detail::submit<execution_path, copy_crc_operation>(
            executor,
            [&]
            {
                DML_VALIDATE_SIZE_CONSISTENCY(src_view.size(), dst_view.size());
                return range_check::copy_crc(src_view.data(), dst_view.data(), src_view.size());
            },
            [&]()
            {
                return ml::copy_crc(
                    src_view.data(), dst_view.data(), src_view.size(), crc_seed, operation.get_params());
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
    inline auto submit(cache_flush_operation        operation,
                       data_view                    dst_view,
                       const execution_interface_t &executor = execution_interface_t())
        -> handler<cache_flush_operation, typename execution_interface_t::allocator_type>
    {
        return detail::submit<execution_path, cache_flush_operation>(
            executor,
            [&]
            {
                return range_check::cache_flush(dst_view.data(), dst_view.size());
            },
            [&]()
            {
                return ml::cache_flush(dst_view.data(), dst_view.size(), operation.get_params());
            });
    }
}  // namespace dml

#endif  //_DML_SUBMIT_HPP_
