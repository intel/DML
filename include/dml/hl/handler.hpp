/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @date 05/20/2021
 * @brief Contains @ref handler definition
 */

#ifndef DML_HANDLER_HPP
#define DML_HANDLER_HPP

#include <dml/detail/ml/result.hpp>
#include <dml/detail/ml/task.hpp>
#include <dml/hl/detail/handler.hpp>

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
         * @brief Actual operation's result type
         */
        using result_type = typename operation_t::result_type;

        template <typename executor_t, typename allocator_type>
        friend class execution_interface;

    public:
        /**
         * @brief Construct empty and invalid handler
         */
        handler(): task_(), status_(status_code::error), is_hw(false)
        {
        }

        /**
         * @brief Construct a handler from a task
         *
         * @param some_task Task for handler construction
         * @param allocator Memory allocator to use
         */
        explicit handler(detail::ml::task<allocator_t> &&some_task, allocator_t allocator):
            task_(std::move(some_task)),
            status_(status_code::ok),
            is_hw(false)
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
        [[nodiscard]] bool valid() const noexcept
        {
            return status_ == status_code::ok;
        }

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
            auto  task_view         = make_view(task_);
            auto &completion_record = task_view.get_completion_record();

            if (status_ == status_code::ok)
            {
                if (is_hw)
                {
                    detail::ml::wait<detail::ml::execution_path::hardware>(task_view);
                }
                else
                {
                    detail::ml::wait<detail::ml::execution_path::software>(task_view);
                }

                return detail::make_result<result_type>(completion_record);
            }
            else
            {
                // Aggregate initialization ensures only first element initialized
                return result_type{ status_ };
            }
        }

        /**
         * @brief Checks whether asynchronous operation is finished
         *
         * @return False if an operation is still being processed, True otherwise.
         */
        [[nodiscard]] bool is_finished() noexcept
        {
            auto  task_view  = make_view(task_);

            if (status_ == status_code::ok)
            {
                return is_hw ? detail::ml::finished<detail::ml::execution_path::hardware>(task_view)
                             : detail::ml::finished<detail::ml::execution_path::software>(task_view);
            }
            else
            {
                return true;
            }
        }

    private:
        template <typename operation_t_, typename allocator_t_>
        friend dml::detail::ml::task_view detail::get_task_view(handler<operation_t_, allocator_t_> &h) noexcept;

        template <typename operation_t_, typename allocator_t_>
        friend void detail::set_hw_path(handler<operation_t_, allocator_t_> &h) noexcept;

        template <typename operation_t_, typename allocator_t_>
        friend void detail::set_status(handler<operation_t_, allocator_t_> &h, status_code status) noexcept;

    private:
        detail::ml::task<allocator_t> task_;   /**< @todo */
        status_code                   status_; /**< This handler status */
        bool                          is_hw;   /**< @todo */
    };
}  // namespace dml

#endif  //DML_HANDLER_HPP
