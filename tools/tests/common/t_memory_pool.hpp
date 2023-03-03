/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @file
 * @brief
 * @date 4/17/2020
 */
#ifndef DML_TEST_MEMORY_POOL
#define DML_TEST_MEMORY_POOL


#include <vector>
#include <string>
#include <mutex>


namespace dml::test
{

    /**
     * @brief Class that performs memory allocation handling
     */
    template<typename T>
    class memory_pool
    {
    public:
        /**
         * @brief Wrapper for memory blocks
         */
        using inner_buffer_t  = std::vector<T>;

        /**
         * @brief Main buffer that constrols memory allocation
         */
        using buffer_t        = std::vector<inner_buffer_t>;

        /**
         * @brief Using value type
         */
        using value_type_t    = typename inner_buffer_t::value_type;

        /**
         * @brief Using pointer type
         */
        using pointer_t       = typename inner_buffer_t::pointer;

        /**
         * @brief Using const pointer type
         */
        using const_pointer_t = typename inner_buffer_t::const_pointer;

        /**
         * @brief Using size type
         */
        using size_type_t     = typename buffer_t::size_type;

        
        /**
         * @brief Forbids object creation
         */
        constexpr memory_pool() = delete;

        /**
         * @brief Allocates size of memory
         */
        [[nodiscard]]
        static auto allocate(const size_type_t size = 0u) noexcept -> inner_buffer_t &;

        /**
         * @brief Copy allocator
         */
        template<class input_iterator> 
        [[nodiscard]]
        static auto allocate(const input_iterator &first, 
                                const input_iterator &last) noexcept -> inner_buffer_t &;

        /**
         * @brief Allocates size of memory than fill it with specified value
         */
        [[nodiscard]]  
        static auto allocate(const size_type_t size, 
                                const value_type_t value) noexcept -> inner_buffer_t &;


        /**
         * @brief Allocates size of memory 
         */
        [[nodiscard]]
        static auto allocate_ptr(const size_type_t size) noexcept -> pointer_t;

        /**
         * @brief Copy allocator
         */
        template<class input_iterator> 
        [[nodiscard]]
        static auto allocate_ptr(const input_iterator &first, 
                                    const input_iterator &last) noexcept -> pointer_t;

        /**
         * @brief Allocate size of memory than fill it with specified value
         */
        [[nodiscard]]  
        static auto allocate_ptr(const size_type_t size, 
                                    const value_type_t value) noexcept -> pointer_t;

        /**
         * @brief Sets size to zero.
         *        Does not really deallocate memory
         */
        constexpr static auto deallocate_all() noexcept -> void;

        /**
         * @brief Deallocates unused memory
         */
        static auto shrink_to_fit() noexcept -> void;

        /**
         * @brief Returns size of the main buffer
         */
        [[nodiscard]]
        constexpr static auto size() noexcept -> size_type_t;

        /**
         * @brief Returns total capacity of the main buffer
         */
        [[nodiscard]]
        constexpr static auto capacity() noexcept -> size_type_t;

        /**
         * @brief Reserves space for futher allocations
         */
        static auto reserve(const size_type_t size) noexcept -> void;

    private:
        /**
         * @brief Main buffer that holds all allocated buffers
         */
        static inline auto m_buffer           = buffer_t();

        /**
         * @brief Points to last unused @ref inner_buffer_t
         */
        static inline auto m_last_free_buffer = size_type_t(0);

        /**
         * @brief Allows thread-safe memory allocation
         */
        static inline auto m_allocation_mutex = std::mutex();
    };


    /**
     * @brief Special type allocator for testing
     */
    using global_allocator = dml::test::memory_pool<uint8_t>;

}


#include "t_memory_pool.cxx"    


#endif // DML_TEST_MEMORY_POOL
