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
 * @brief Contains @ref sequence definition
 */

#ifndef DML_SEQUENCE_HPP
#define DML_SEQUENCE_HPP

#include <dml/detail/buffer.hpp>
#include <dml/detail/utils.hpp>

#include <dml/handler.hpp>
#include <dml/operations.hpp>
#include <dml_common/range_check.hpp>
#include <dml_ml/apply_delta.hpp>
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
        using op_buffer_t = detail::buffer_array<ml::operation, allocator_t>;

        /**
         * @brief Type of buffer for Middle Layer results
         */
        using res_buffer_t = detail::buffer_array<ml::result, allocator_t>;

    public:
        /**
         * @brief Constructs a sequence for a specified number of operations
         *
         * @param length    Sequence capacity
         * @param allocator Instance of memory allocator
         */
        explicit sequence(size_t length, allocator_t allocator = allocator_t()):
            operations_(length, allocator), records_(length, allocator), current_length_(0u)
        {
        }

        /**
         * @brief Returns current number of operations stored in the sequence
         *
         * @return Current number of operations
         */
        [[nodiscard]] auto length() const noexcept { return current_length_; }

        /**
         * @brief Returns a pointer to the memory regions with operations
         *
         * @return Pointer to operations array
         */
        [[nodiscard]] auto data() const noexcept { return &operations_.get(0); }

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
        inline status_code add(mem_move_operation operation,
                               const_data_view    src_view,
                               data_view          dst_view);

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
        inline status_code add(mem_copy_operation operation,
                               const_data_view    src_view,
                               data_view          dst_view);

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
        inline status_code add(dualcast_operation operation,
                               const_data_view    src_view,
                               data_view          dst1_view,
                               data_view          dst2_view);

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
        inline status_code add(compare_operation operation,
                               const_data_view   src1_view,
                               const_data_view   src2_view);

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
        inline status_code add(copy_crc_operation operation,
                               const_data_view    src_view,
                               data_view          dst_view,
                               uint32_t           crc_seed);

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
        op_buffer_t  operations_;     /**< Buffer for operations array */
        res_buffer_t records_;        /**< Buffer for results array */
        size_t       current_length_; /**< Current number of operation stored in the sequence */
    };

    template <typename allocator_t>
    inline status_code sequence<allocator_t>::add(mem_move_operation operation,
                                                  const_data_view    src_view,
                                                  data_view          dst_view)
    {
        if (current_length_ == operations_.get_count())
        {
            return status_code::batch_overflow;
        }

        static_cast<void>(operation);

        DML_VALIDATE_SIZE_CONSISTENCY(src_view.size(), dst_view.size());
        auto status = range_check::mem_move(src_view.data(), dst_view.data(), src_view.size());
        if (status != status_code::ok)
        {
            return status;
        }

        operations_.get(current_length_) =
            ml::mem_move(src_view.data(), dst_view.data(), src_view.size());
        operations_.get(current_length_).associate(records_.get(current_length_));

        current_length_++;
        return status_code::ok;
    }

    template <typename allocator_t>
    inline status_code sequence<allocator_t>::add(mem_copy_operation operation,
                                                  const_data_view    src_view,
                                                  data_view          dst_view)
    {
        if (current_length_ == operations_.get_count())
        {
            return status_code::batch_overflow;
        }

        static_cast<void>(operation);

        DML_VALIDATE_SIZE_CONSISTENCY(src_view.size(), dst_view.size());
        auto status = range_check::mem_copy(src_view.data(), dst_view.data(), src_view.size());
        if (status != status_code::ok)
        {
            return status;
        }

        operations_.get(current_length_) = ml::mem_copy(src_view.data(), dst_view.data(), src_view.size());
        operations_.get(current_length_).associate(records_.get(current_length_));

        current_length_++;
        return status_code::ok;
    }

    template <typename allocator_t>
    inline status_code sequence<allocator_t>::add(fill_operation operation,
                                                  uint64_t       pattern,
                                                  data_view      dst_view)
    {
        if (current_length_ == operations_.get_count())
        {
            return status_code::batch_overflow;
        }

        static_cast<void>(operation);

        auto status = range_check::fill(dst_view.data(), dst_view.size());

        if (status != status_code::ok)
        {
            return status;
        }

        operations_.get(current_length_) =
            ml::fill(pattern, dst_view.data(), dst_view.size());
        operations_.get(current_length_).associate(records_.get(current_length_));

        current_length_++;
        return status_code::ok;
    }

    template <typename allocator_t>
    inline status_code sequence<allocator_t>::add(dualcast_operation operation,
                                                  const_data_view    src_view,
                                                  data_view          dst1_view,
                                                  data_view          dst2_view)
    {
        if (current_length_ == operations_.get_count())
        {
            return status_code::batch_overflow;
        }

        static_cast<void>(operation);

        DML_VALIDATE_SIZE_CONSISTENCY(src_view.size(), dst1_view.size());
        DML_VALIDATE_SIZE_CONSISTENCY(src_view.size(), dst2_view.size());
        auto status = range_check::dualcast(src_view.data(), dst1_view.data(), dst2_view.data(), src_view.size());

        if (status != status_code::ok)
        {
            return status;
        }

        operations_.get(current_length_) = ml::dualcast(
            src_view.data(), dst1_view.data(), dst2_view.data(), src_view.size());
        operations_.get(current_length_).associate(records_.get(current_length_));

        current_length_++;
        return status_code::ok;
    }

    template <typename allocator_t>
    inline status_code sequence<allocator_t>::add(compare_operation operation,
                                                  const_data_view   src1_view,
                                                  const_data_view   src2_view)
    {
        if (current_length_ == operations_.get_count())
        {
            return status_code::batch_overflow;
        }

        DML_VALIDATE_SIZE_CONSISTENCY(src1_view.size(), src2_view.size());
        auto status = range_check::compare(src1_view.data(), src2_view.data(), src1_view.size());

        if (status != status_code::ok)
        {
            return status;
        }

        operations_.get(current_length_) = ml::compare(src1_view.data(),
                                                       src2_view.data(),
                                                       src1_view.size(),
                                                       operation.get_expected_result());
        operations_.get(current_length_).associate(records_.get(current_length_));

        current_length_++;
        return status_code::ok;
    }

    template <typename allocator_t>
    inline status_code sequence<allocator_t>::add(compare_pattern_operation operation,
                                                  uint64_t                  pattern,
                                                  const_data_view           src_view)
    {
        if (current_length_ == operations_.get_count())
        {
            return status_code::batch_overflow;
        }

        auto status = range_check::compare_pattern(src_view.data(), src_view.size());

        if (status != status_code::ok)
        {
            return status;
        }

        operations_.get(current_length_) =
            ml::compare_pattern(pattern,
                                src_view.data(),
                                src_view.size(),
                                operation.get_expected_result());
        operations_.get(current_length_).associate(records_.get(current_length_));

        current_length_++;
        return status_code::ok;
    }

    template <typename allocator_t>
    inline status_code sequence<allocator_t>::add(create_delta_operation operation,
                                                  const_data_view        src1_view,
                                                  const_data_view        src2_view,
                                                  data_view              delta_view)
    {
        if (current_length_ == operations_.get_count())
        {
            return status_code::batch_overflow;
        }

        static_cast<void>(operation);

        DML_VALIDATE_SIZE_CONSISTENCY(src1_view.size(), src2_view.size());
        auto status = range_check::create_delta(
            src1_view.data(), src2_view.data(), src1_view.size(), delta_view.data(), delta_view.size());

        if (status != status_code::ok)
        {
            return status;
        }

        operations_.get(current_length_) = ml::create_delta(src1_view.data(),
                                                            src2_view.data(),
                                                            src1_view.size(),
                                                            delta_view.data(),
                                                            delta_view.size());
        operations_.get(current_length_).associate(records_.get(current_length_));

        current_length_++;
        return status_code::ok;
    }

    template <typename allocator_t>
    inline status_code sequence<allocator_t>::add(apply_delta_operation operation,
                                                  const_data_view       delta_view,
                                                  data_view             dst_view,
                                                  create_delta_result   delta_result)
    {
        if (current_length_ == operations_.get_count())
        {
            return status_code::batch_overflow;
        }

        static_cast<void>(operation);

        if (delta_result.result != 1)
        {
            return status_code::delta_delta_empty;
        }

        auto status = range_check::apply_delta(delta_view.data(),
                                               delta_result.delta_record_size,
                                               dst_view.data(),
                                               dst_view.size());

        if (status != status_code::ok)
        {
            return status;
        }

        operations_.get(current_length_) = ml::apply_delta(delta_view.data(),
                                                           delta_result.delta_record_size,
                                                           dst_view.data(),
                                                           dst_view.size());
        operations_.get(current_length_).associate(records_.get(current_length_));

        current_length_++;
        return status_code::ok;
    }

    template <typename allocator_t>
    inline status_code sequence<allocator_t>::add(crc_operation   operation,
                                                  const_data_view src_view,
                                                  uint32_t        crc_seed)
    {
        if (current_length_ == operations_.get_count())
        {
            return status_code::batch_overflow;
        }

        auto status = range_check::crc(src_view.data(), src_view.size());

        if (status != status_code::ok)
        {
            return status;
        }

        operations_.get(current_length_) =
            ml::crc(src_view.data(), src_view.size(), crc_seed, operation.get_params());
        operations_.get(current_length_).associate(records_.get(current_length_));

        current_length_++;
        return status_code::ok;
    }

    template <typename allocator_t>
    inline status_code sequence<allocator_t>::add(copy_crc_operation operation,
                                                  const_data_view    src_view,
                                                  data_view          dst_view,
                                                  uint32_t           crc_seed)
    {
        if (current_length_ == operations_.get_count())
        {
            return status_code::batch_overflow;
        }

        DML_VALIDATE_SIZE_CONSISTENCY(src_view.size(), dst_view.size());
        auto status = range_check::copy_crc(src_view.data(), dst_view.data(), src_view.size());

        if (status != status_code::ok)
        {
            return status;
        }

        operations_.get(current_length_) = ml::copy_crc(src_view.data(),
                                                        dst_view.data(),
                                                        src_view.size(),
                                                        crc_seed,
                                                        operation.get_params());
        operations_.get(current_length_).associate(records_.get(current_length_));

        current_length_++;
        return status_code::ok;
    }

    template <typename allocator_t>
    inline status_code sequence<allocator_t>::add(cache_flush_operation operation, data_view dst_view)
    {
        if (current_length_ == operations_.get_count())
        {
            return status_code::batch_overflow;
        }

        auto status = range_check::cache_flush(dst_view.data(), dst_view.size());

        if (status != status_code::ok)
        {
            return status;
        }

        operations_.get(current_length_) =
            ml::cache_flush(dst_view.data(), dst_view.size(), operation.get_params());
        operations_.get(current_length_).associate(records_.get(current_length_));

        current_length_++;
        return status_code::ok;
    }
}  // namespace dml

#endif  //_DML_SEQUENCE_HPP_
