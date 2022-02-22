/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @date 05/20/2021
 * @brief Contains @ref sequence definition
 */

#ifndef DML_SEQUENCE_HPP
#define DML_SEQUENCE_HPP

#include <dml/detail/common/status.hpp>
#include <dml/detail/ml/make_task.hpp>
#include <dml/detail/ml/result.hpp>
#include <dml/hl/detail/buffer.hpp>
#include <dml/hl/detail/utils.hpp>
#include <dml/hl/handler.hpp>
#include <dml/hl/operations.hpp>

namespace dml
{
    /**
     * @ingroup dmlhl_aux
     *
     * @brief Container for a contiguous sequence of operations
     *
     * Used with @ref batch_operation
     *
     * @tparam allocator_t Type of memory allocator
     */
    template <typename allocator_t = std::allocator<byte_t>>
    class sequence
    {
        /**
         * @brief Type of buffer for Middle Layer operations
         */
        using op_buffer_t = detail::buffer_array<detail::descriptor, allocator_t>;

        /**
         * @brief Type of buffer for Middle Layer results
         */
        using res_buffer_t = detail::buffer_array<detail::completion_record, allocator_t>;

    public:
        /**
         * @brief Constructs a sequence for a specified number of operations
         *
         * @param length    Sequence capacity
         * @param allocator Instance of memory allocator
         */
        explicit sequence(size_t length, allocator_t allocator = allocator_t()):
            operations_(length, allocator),
            results_(length, allocator),
            current_length_(0u)
        {
        }

        /**
         * @brief Returns current number of operations stored in the sequence
         *
         * @return Current number of operations
         */
        [[nodiscard]] auto length() const noexcept
        {
            return current_length_;
        }

        /**
         * @brief Returns a pointer to the memory regions with operations
         *
         * @return Pointer to operations array
         */
        [[nodiscard]] auto data() const noexcept
        {
            return &operations_.get(0);
        }

        /**
         * @brief Adds Memory Move operation to the sequence
         *
         * See @ref mem_move_operation for algorithm details
         *
         * @param operation Instance of @ref mem_move_operation
         * @param src_view @ref data_view to the source memory region
         * @param dst_view @ref data_view to the destination memory region
         *
         * Usage:
         * @code
         * auto status = sequence.add(dml::mem_move, dml::make_view(src), dml::make_view(dst));
         * if (status != dml::status_code::ok) return error;
         * @endcode
         *
         * @warning In case failure is occurred, the sequence remains the same as it was before the call
         *
         * @return @ref status_code to report success or failure
         */
        inline status_code add(mem_move_operation operation, const_data_view src_view, data_view dst_view);

        /**
         * @brief Adds Memory Copy operation to the sequence
         *
         * See @ref mem_copy_operation for algorithm details
         *
         * @param operation Instance of @ref mem_copy_operation
         * @param src_view @ref data_view to the source memory region
         * @param dst_view @ref data_view to the destination memory region
         *
         * Usage:
         * @code
         * auto status = sequence.add(dml::mem_copy, dml::make_view(src), dml::make_view(dst));
         * if (status != dml::status_code::ok) return error;
         * @endcode
         *
         * @warning In case failure is occurred, the sequence remains the same as it was before the call
         *
         * @return @ref status_code to report success or failure
         */
        inline status_code add(mem_copy_operation operation, const_data_view src_view, data_view dst_view);

        /**
         * @brief AddsFill operation to the sequence
         *
         * See @ref fill_operation for algorithm details
         *
         * @param operation Instance of @ref fill_operation
         * @param pattern   64-bit pattern used to fill the destination
         * @param dst_view  @ref data_view to the destination memory region
         *
         * Usage:
         * @code
         * auto status = sequence.add(dml::fill, pattern, dml::make_view(dst));
         * if (status != dml::status_code::ok) return error;
         * @endcode
         *
         * @warning In case failure is occurred, the sequence remains the same as it was before the call
         *
         * @return @ref status_code to report success or failure
         */
        inline status_code add(fill_operation operation, uint64_t pattern, data_view dst_view);

        /**
         * @brief Adds Dualcast operation to the sequence
         *
         * See @ref dualcast_operation for algorithm details
         *
         * @param operation Instance of @ref dualcast_operation
         * @param src_view  @ref data_view to the source memory region
         * @param dst1_view @ref data_view to the first destination memory region
         * @param dst2_view @ref data_view to the second destination memory region
         *
         * Usage:
         * @code
         * auto status = sequence.add(dml::dualcast, dml::make_view(src), dml::make_view(dst1),  dml::make_view(dst2));
         * if (status != dml::status_code::ok) return error;
         * @endcode
         *
         * @warning In case failure is occurred, the sequence remains the same as it was before the call
         *
         * @return @ref status_code to report success or failure
         */
        inline status_code add(dualcast_operation operation, const_data_view src_view, data_view dst1_view, data_view dst2_view);

        /**
         * @brief Adds Compare operation to the sequence
         *
         * See @ref compare_operation for algorithm details
         *
         * @param operation Instance of @ref compare_operation
         * @param src1_view @ref data_view to the first source memory region
         * @param src2_view @ref data_view to the second source memory region
         *
         * Usage:
         * @code
         * auto status = sequence.add(dml::compare, dml::make_view(src1), dml::make_view(src2));
         * if (status != dml::status_code::ok) return error;
         * @endcode
         *
         * @warning In case failure is occurred, the sequence remains the same as it was before the call
         *
         * @return @ref status_code to report success or failure
         */
        inline status_code add(compare_operation operation, const_data_view src1_view, const_data_view src2_view);

        /**
         * @brief Adds Compare with Pattern operation to the sequence
         *
         * See @ref compare_pattern_operation for algorithm details
         *
         * @param operation Instance of @ref compare_pattern_operation
         * @param pattern   64-bit pattern to compare with the memory region
         * @param src_view  @ref data_view to the source memory region
         *
         * Usage:
         * @code
         * auto status = sequence.add(dml::compare_pattern, pattern, dml::make_view(src));
         * if (status != dml::status_code::ok) return error;
         * @endcode
         *
         * @warning In case failure is occurred, the sequence remains the same as it was before the call
         *
         * @return @ref status_code to report success or failure
         */
        inline status_code add(compare_pattern_operation operation, uint64_t pattern, const_data_view src_view);

        /**
         * @brief Adds Create Delta operation to the sequence
         *
         * See @ref create_delta_operation for algorithm details
         *
         * @param operation  Instance of @ref create_delta_operation
         * @param src1_view  @ref data_view to the first source memory region
         * @param src2_view  @ref data_view to the second source memory region
         * @param delta_view @ref data_view to the memory region for delta record
         *
         * Usage:
         * @code
         * auto status = sequence.add(dml::create_delta, dml::make_view(src1), dml::make_view(src2), dml::make_view(delta));
         * if (status != dml::status_code::ok) return error;
         * @endcode
         *
         * @warning In case failure is occurred, the sequence remains the same as it was before the call
         *
         * @return @ref status_code to report success or failure
         */
        inline status_code add(create_delta_operation operation,
                               const_data_view        src1_view,
                               const_data_view        src2_view,
                               data_view              delta_view);

        /**
         * @brief Adds Apply Delta operation to the sequence
         *
         * See @ref apply_delta_operation for algorithm details
         *
         * @param operation    Instance of @ref apply_delta_operation
         * @param delta_view   @ref data_view to the memory region with delta record
         * @param dst_view     @ref data_view to the destination memory region (the first source from Create Delta)
         * @param delta_result Result from Create Delta operation
         *
         * Usage:
         * @code
         * auto status = sequence.add(dml::apply_delta, dml::make_view(delta), dml::make_view(src1), create_result);
         * if (status != dml::status_code::ok) return error;
         * @endcode
         *
         * @warning In case failure is occurred, the sequence remains the same as it was before the call
         *
         * @return @ref status_code to report success or failure
         */
        inline status_code add(apply_delta_operation operation,
                               const_data_view       delta_view,
                               data_view             dst_view,
                               create_delta_result   delta_result);

        /**
         * @brief Adds CRC operation to the sequence
         *
         * See @ref crc_operation for algorithm details
         *
         * @param operation Instance of @ref crc_operation
         * @param src_view  @ref data_view to the source memory region
         * @param crc_seed  Initial CRC value
         *
         * Usage:
         * @code
         * auto status = sequence.add(dml::crc, dml::make_view(src), crc_seed);
         * if (status != dml::status_code::ok) return error;
         * @endcode
         *
         * @warning In case failure is occurred, the sequence remains the same as it was before the call
         *
         * @return @ref status_code to report success or failure
         */
        inline status_code add(crc_operation operation, const_data_view src_view, uint32_t crc_seed);

        /**
         * @brief Adds Copy + CRC operation to the sequence
         *
         * See @ref copy_crc_operation for algorithm details
         *
         * @param operation Instance of @ref copy_crc_operation
         * @param src_view  @ref data_view to the source memory region
         * @param dst_view  @ref data_view to the destination memory region
         * @param crc_seed  Initial CRC value
         *
         * Usage:
         * @code
         * auto status = sequence.add(dml::copy_crc, dml::make_view(src), dml::make_view(dst), crc_seed);
         * if (status != dml::status_code::ok) return error;
         * @endcode
         *
         * @warning In case failure is occurred, the sequence remains the same as it was before the call
         *
         * @return @ref status_code to report success or failure
         */
        inline status_code add(copy_crc_operation operation, const_data_view src_view, data_view dst_view, uint32_t crc_seed);

        /**
         * @brief Adds Cache Flush operation to the sequence
         *
         * See @ref cache_flush_operation for algorithm details
         *
         * @param operation Instance of @ref cache_flush_operation
         * @param dst_view  @ref data_view to the destination memory region
         *
         * Usage:
         * @code
         * auto status = sequence.add(dml::cache_flush, dml::make_view(dst));
         * if (status != dml::status_code::ok) return error;
         * @endcode
         *
         * @warning In case failure is occurred, the sequence remains the same as it was before the call
         *
         * @return @ref status_code to report success or failure
         */
        inline status_code add(cache_flush_operation operation, data_view dst_view);

    private:
        /**
         * @todo
         */
        template <typename make_task_t>
        inline status_code add(make_task_t&& make_task) noexcept
        {
            if (current_length_ == operations_.get_count())
            {
                return status_code::batch_overflow;
            }

            auto& descriptor = operations_.get(current_length_);
            auto& record     = results_.get(current_length_);

            detail::ml::make_view(descriptor, record) = make_task();

            // Auto execution path validation is the strictest.
            if (auto status = detail::ml::execution_path::automatic::validate(descriptor); status != detail::validation_status::success)
            {
                return detail::to_own(status);
            }

            current_length_++;
            return status_code::ok;
        }

    private:
        op_buffer_t  operations_;     /**< Buffer for operations array */
        res_buffer_t results_;        /**< Buffer for results array */
        size_t       current_length_; /**< Current number of operation stored in the sequence */
    };

    template <typename allocator_t>
    inline status_code sequence<allocator_t>::add(mem_move_operation operation, const_data_view src_view, data_view dst_view)
    {
        DML_VALIDATE_SIZE_CONSISTENCY(src_view.size(), dst_view.size());

        return add(
            [&]
            {
                return detail::ml::make_mem_move_task(src_view.data(), dst_view.data(), src_view.size(), operation.get_options());
            });
    }

    template <typename allocator_t>
    inline status_code sequence<allocator_t>::add(mem_copy_operation operation, const_data_view src_view, data_view dst_view)
    {
        DML_VALIDATE_SIZE_CONSISTENCY(src_view.size(), dst_view.size());

        return add(
            [&]
            {
                return detail::ml::make_mem_move_task(src_view.data(), dst_view.data(), src_view.size(), operation.get_options());
            });
    }

    template <typename allocator_t>
    inline status_code sequence<allocator_t>::add(fill_operation operation, uint64_t pattern, data_view dst_view)
    {
        return add(
            [&]
            {
                return detail::ml::make_fill_task(pattern, dst_view.data(), dst_view.size(), operation.get_options());
            });
    }

    template <typename allocator_t>
    inline status_code sequence<allocator_t>::add(dualcast_operation operation,
                                                  const_data_view    src_view,
                                                  data_view          dst1_view,
                                                  data_view          dst2_view)
    {
        DML_VALIDATE_SIZE_CONSISTENCY(src_view.size(), dst1_view.size());
        DML_VALIDATE_SIZE_CONSISTENCY(src_view.size(), dst2_view.size());

        return add(
            [&]
            {
                return detail::ml::make_dualcast_task(src_view.data(),
                                                      dst1_view.data(),
                                                      dst2_view.data(),
                                                      src_view.size(),
                                                      operation.get_options(),
                                                      operation.get_specific_options());
            });
    }

    template <typename allocator_t>
    inline status_code sequence<allocator_t>::add(compare_operation operation, const_data_view src1_view, const_data_view src2_view)
    {
        DML_VALIDATE_SIZE_CONSISTENCY(src1_view.size(), src2_view.size());

        return add(
            [&]
            {
                return detail::ml::make_compare_task(src1_view.data(),
                                                     src2_view.data(),
                                                     src1_view.size(),
                                                     operation.get_options(),
                                                     operation.get_expected_result());
            });
    }

    template <typename allocator_t>
    inline status_code sequence<allocator_t>::add(compare_pattern_operation operation, uint64_t pattern, const_data_view src_view)
    {
        return add(
            [&]
            {
                return detail::ml::make_compare_pattern_task(pattern,
                                                             src_view.data(),
                                                             src_view.size(),
                                                             operation.get_options(),
                                                             operation.get_expected_result());
            });
    }

    template <typename allocator_t>
    inline status_code sequence<allocator_t>::add(create_delta_operation operation,
                                                  const_data_view        src1_view,
                                                  const_data_view        src2_view,
                                                  data_view              delta_view)
    {
        DML_VALIDATE_SIZE_CONSISTENCY(src1_view.size(), src2_view.size());

        return add(
            [&]
            {
                return detail::ml::make_create_delta_task(src1_view.data(),
                                                          src2_view.data(),
                                                          src1_view.size(),
                                                          delta_view.data(),
                                                          delta_view.size(),
                                                          operation.get_options(),
                                                          operation.get_expected_result());
            });
    }

    template <typename allocator_t>
    inline status_code sequence<allocator_t>::add(apply_delta_operation operation,
                                                  const_data_view       delta_view,
                                                  data_view             dst_view,
                                                  create_delta_result   delta_result)
    {
        if (delta_result.result != comparison_result::not_equal)
        {
            return status_code::delta_delta_empty;
        }
        return add(
            [&]
            {
                return detail::ml::make_apply_delta_task(delta_view.data(),
                                                         delta_result.delta_record_size,
                                                         dst_view.data(),
                                                         dst_view.size(),
                                                         operation.get_options());
            });
    }

    template <typename allocator_t>
    inline status_code sequence<allocator_t>::add(crc_operation operation, const_data_view src_view, uint32_t crc_seed)
    {
        return add(
            [&]
            {
                return detail::ml::make_crc_task(src_view.data(),
                                                 src_view.size(),
                                                 crc_seed,
                                                 operation.get_options(),
                                                 operation.get_specific_options());
            });
    }

    template <typename allocator_t>
    inline status_code sequence<allocator_t>::add(copy_crc_operation operation,
                                                  const_data_view    src_view,
                                                  data_view          dst_view,
                                                  uint32_t           crc_seed)
    {
        DML_VALIDATE_SIZE_CONSISTENCY(src_view.size(), dst_view.size());
        return add(
            [&]
            {
                return detail::ml::make_copy_crc_task(src_view.data(),
                                                      dst_view.data(),
                                                      src_view.size(),
                                                      crc_seed,
                                                      operation.get_options(),
                                                      operation.get_specific_options());
            });
    }

    template <typename allocator_t>
    inline status_code sequence<allocator_t>::add(cache_flush_operation operation, data_view dst_view)
    {
        return add(
            [&]
            {
                return detail::ml::make_cache_flush_task(dst_view.data(), dst_view.size(), operation.get_options());
            });
    }
}  // namespace dml

#endif  //_DML_SEQUENCE_HPP_
