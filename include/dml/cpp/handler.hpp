/*
 * Copyright 2020-2021 Intel Corporation.
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
 * @brief Contains @ref handler definition
 */

#ifndef DML_HANDLER_HPP
#define DML_HANDLER_HPP

#include <dml/cpp/detail/buffer.hpp>
#include <dml/cpp/detail/handler.hpp>
#include <dml/cpp/middle_layer/completion_record.hpp>

namespace dml
{
    /**
     * @ingroup dmlhl_aux
     * @brief Handler to a (possibly) asynchronously running operation
     *
     * @tparam operation_t Type of operation
     * @tparam allocator_t Type of allocator
     */
    template <typename operation_t, typename allocator_t>
    class handler
    {
        /**
         * @brief Internal buffer type for a result
         */
        using buffer_type = detail::buffer<ml::completion_record, allocator_t>;

        /**
         * @brief Actual operation's result type
         */
        using result_type = typename operation_t::result_type;

        template <typename executor_t, typename allocator_type>
        friend class execution_interface;

    public:
        /**
         * @brief Construct empty handler
         *
         * Handler is invalid until assigned with a valid one
         *
         * @param allocator Memory allocator to use
         */
        explicit handler(allocator_t allocator = allocator_t()) noexcept:
            record_(allocator, false), status_(status_code::error)
        {
        }

        /**
         * @brief Checks whether handler is valid
         *
         * Handler is invalid when it is not bind to an operation.
         * Use @ref get to find out why it is invalid.
         *
         * @return True if hanlder is valid, false otherwise
         */
        [[nodiscard]] bool valid() const noexcept { return status_ == status_code::ok; }

        /**
         * @brief Get result for a submitted operation
         *
         * This methods waits for an operation to finish, blocking current thread.
         *
         * In case handler is not valid, resulting structure status field will contain error status code.
         *
         * @return Result structure for an operation
         */
        auto get() noexcept
        {
            if (status_ == status_code::ok)
            {
                ml::wait(record_.get());

                return detail::make_result<result_type>(record_.get());
            }
            else
            {
                // Aggregate initialization ensures only first element initialized
                return result_type{status_};
            }
        }

        /**
         * @brief Checks whether asynchronous operation is finished
         *
         * @return False if an operation is still being processed, True otherwise.
         */
        [[nodiscard]] bool is_finished() const noexcept
        {
            if (status_ == status_code::ok)
            {
                return ml::is_finished(record_.get());
            }
            else
            {
                return true;
            }
        }

    private:
        /**
         * @brief Constructs a handler with initial status and allocator
         *
         * Memory allocation is done only if status is @ref status::ok, otherwise hanlder is empty.
         *
         * @param status    Initial status
         * @param allocator Instance of memory allocator
         */
        explicit handler(status_code status, allocator_t allocator):
            record_(allocator, status == status_code::ok), status_(status)
        {
        }

        friend ml::completion_record &detail::get_ml_result<>(handler<operation_t, allocator_t> &h) noexcept;

    private:
        buffer_type record_; /**< Memory buffer for a result */
        status_code status_; /**< This handler status */
    };
}  // namespace dml

#endif  //DML_HANDLER_HPP
