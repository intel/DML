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
 * @date 05/19/2021
 * @brief Contains definitions of @ref dml::ml::result type
 */

#ifndef DML_ML_RESULT_HPP
#define DML_ML_RESULT_HPP

#include <dml_common/result.hpp>
#include <dml_common/types.hpp>
#include <dml_ml/awaiter.hpp>

namespace dml::ml {
    /**
     * @ingroup dmlml_operations
     * @brief Generic result of all operations
     */
    class alignas(32u) result {
        /**
         * @brief This class memory size requirement
         */
        static constexpr auto memory_size = 32u;

    public:
        /**
         * @brief Default constructor.
         *
         * Fills all the data with zeroes
         */
        result() noexcept = default;

        /**
         * @brief Blocks execution until result is written and ready for extraction
         */
        void wait() const volatile noexcept {
            awaiter wait_for(static_cast<volatile void *>(data_), 0);
        }

        /**
         * @brief Checks whether status written is a success one
         *
         * @return Boolean result of the check
         */
        [[nodiscard]] bool is_success() const volatile noexcept { return 1 == data_[0]; }

        /**
         * @brief Checks whether operation is finished
         *
         * @return True is operation is finished, False otherwise.
         */
        [[nodiscard]] bool is_finished() const volatile noexcept { return 0 != data_[0]; }

        /**
         * @brief Extracts Memory Move operation result
         *
         * @return @ref mem_move_result instance
         */
        explicit operator mem_move_result() const noexcept;

        /**
         * @brief Extracts Memory Copy operation result
         *
         * @return @ref mem_copy_result instance
         */
        explicit operator mem_copy_result() const noexcept;

        /**
         * @brief Extracts Fill operation result
         *
         * @return @ref fill_result instance
         */
        explicit operator fill_result() const noexcept;

        /**
         * @brief Extracts Dualcast operation result
         *
         * @return @ref dualcast_result instance
         */
        explicit operator dualcast_result() const noexcept;

        /**
         * @brief Extracts Compare operation result
         *
         * @return @ref compare_result instance
         */
        explicit operator compare_result() const noexcept;

        /**
         * @brief Extracts Create Delta operation result
         *
         * @return @ref create_delta_result instance
         */
        explicit operator create_delta_result() const noexcept;

        /**
         * @brief Extracts Apply Delta operation result
         *
         * @return @ref apply_delta_result instance
         */
        explicit operator apply_delta_result() const noexcept;

        /**
         * @brief Extracts CRC operation result
         *
         * @return @ref crc_result instance
         */
        explicit operator crc_result() const noexcept;

        /**
         * @brief Extracts Cache Flush operation result
         *
         * @return @ref cache_flush_result instance
         */
        explicit operator cache_flush_result() const noexcept;

        /**
         * @brief Extracts Batch operation result
         *
         * @return @ref batch_result instance
         */
        explicit operator batch_result() const noexcept;

    private:
        mutable byte_t data_[memory_size]{}; /**<Underlying data array */
    };
}  // namespace dml::ml

#endif  //DML_ML_RESULT_HPP
