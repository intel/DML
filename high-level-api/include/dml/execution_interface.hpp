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
 * @brief Contains @ref execution_interface
 */

#ifndef DML_EXECUTION_INTERFACE_HPP
#define DML_EXECUTION_INTERFACE_HPP

#include <dml/handler.hpp>

namespace dml
{
    /**
     * @ingroup dmlhl dmlhl_aux
     * @brief Abstracts asynchronous execution mechanisms
     *
     * Those mechanism includes threading and memory management
     *
     * @tparam executor_t  Type of asynchronous executor (threads)
     * @tparam allocator_t Type of memory manager (allocator)
     *
     * Usage:
     * @code
     * auto my_exec_iface = dml::execution_interface(
     *     [&my_thread_pool] (auto&& task)
     *     {
     *         using task_t = std::decay_t<decltype(task)>;
     *         my_thread_pool.run(std::forward<task_t>(task));
     *     },
     *     my_allocator_t()
     * );
     *
     * auto handler = dml::submit<dml::software>(dml::mem_move, dml::make_view(src), dml::make_view(dst), my_exec_iface);
     * @endcode
     */
    template <typename executor_t, typename allocator_t>
    class execution_interface
    {
    public:
        /**
         * @brief Alias for type of the allocator
         */
        using allocator_type = allocator_t;

        /**
         * @brief Constructs execution interface from executor and allocator
         *
         * @param executor  Instance of asynchronous executor
         * @param allocator Instance of allocator
         */
        explicit execution_interface(executor_t executor = executor_t(), allocator_t allocator = allocator_t()):
            executor_(executor), allocator_(allocator)
        {
        }

        /**
         * @brief Starts execution based on provided executor
         *
         * @tparam task_t Type of callable task
         * @param task    Instance of callable task
         *
         * @return Executor return value (if present)
         */
        template <typename task_t>
        decltype(auto) execute(task_t &&task) const
            noexcept(noexcept(std::declval<executor_t>()(std::forward<task_t>(task))))
        {
            return executor_(std::forward<task_t>(task));
        }

        /**
         * @brief Constructs a @ref handler, but with range check
         *
         * @tparam operation     Type of operation
         * @tparam range_check_t Type of callable range check
         * @param check          Instance of callable range check
         *
         * @return Operation @ref handler, if range check fails - empty handler.
         */
        template <typename operation, typename range_check_t>
        auto make_handler_with_range_check(range_check_t &&check) const
        {
            return handler<operation, allocator_t>(check(), allocator_);
        }

        /**
         * @brief Constructs a @ref handler
         *
         * @tparam operation Type of operation
         * @param status     Initial status for a handler
         *
         * @return Operation @ref handler
         */
        template <typename operation>
        auto make_handler(status_code status) const
        {
            return handler<operation, allocator_t>(status, allocator_);
        }

    private:
        executor_t  executor_;  /**< Asynchronous executor */
        allocator_t allocator_; /**< Memory allocator */
    };

    /**
     * @ingroup dmlhl dmlhl_aux
     * @brief Default execution interface type, based on execution path
     *
     * @tparam execution_path Type of execution path
     */
    template <typename execution_path>
    using default_execution_interface = execution_interface<typename execution_path::default_thread_spawner,
                                                            typename execution_path::default_allocator>;
}  // namespace dml

#endif  //DML_EXECUTION_INTERFACE_HPP
