/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @date 05/20/2021
 * @brief Contains buffer definitions
 */

#ifndef DML_DETAIL_BUFFER_HPP
#define DML_DETAIL_BUFFER_HPP

#include <dml/hl/types.hpp>
#include <memory>

namespace dml::detail
{
    /**
     * @brief Abstract memory management for an array of objects
     *
     * @tparam elem_t      Type of element of the buffer
     * @tparam allocator_t Type of memory allocator
     */
    template <typename elem_t, typename allocator_t>
    class buffer_array
    {
        /**
         * @brief Memory size required for the element
         */
        static constexpr auto memory_size = sizeof(elem_t);

        /**
         * @brief Alignment requirement of the element
         */
        static constexpr auto alignment = alignof(elem_t);

        /**
         * @brief Own allocator type
         */
        using own_alloc_t = typename std::allocator_traits<allocator_t>::template rebind_alloc<elem_t>;

        /**
         * @brief Own allocator traits
         */
        using own_traits_t = typename std::allocator_traits<allocator_t>::template rebind_traits<elem_t>;

    public:
        /**
         * @brief Creates buffer. Allocates an array of elements
         *
         * @param count     Number of elements
         * @param allocator Instance of allocator
         */
        buffer_array(size_t count, allocator_t allocator): count_(count), data_(nullptr), aligned_data_(nullptr), allocator_(allocator)
        {
            data_ = own_traits_t::allocate(allocator_, (count_ * memory_size) + alignment);

            // TODO: Add dml::assert

            // Align
            auto align_offset = static_cast<std::size_t>(reinterpret_cast<uintptr_t>(data_) & (alignment - 1));
            if (align_offset != 0)
            {
                align_offset = alignment - align_offset;
            }
            aligned_data_ = reinterpret_cast<elem_t *>(reinterpret_cast<byte_t *>(data_) + align_offset);

            for (auto i = 0u; i < count_; ++i)
            {
                own_traits_t::construct(allocator_, aligned_data_ + i);
            }
        }

        /**
         * @brief Deallocate the array of elements
         */
        ~buffer_array() noexcept
        {
            if (data_)
            {
                for (auto i = 0u; i < count_; ++i)
                {
                    own_traits_t::destroy(allocator_, aligned_data_ + i);
                }
                own_traits_t::deallocate(allocator_, data_, (count_ * memory_size) + alignment);
            }
        }

        /**
         * @brief Move constructor
         */
        buffer_array(buffer_array &&other) noexcept:
            count_(std::exchange(other.count_, 0u)),
            data_(std::exchange(other.data_, nullptr)),
            aligned_data_(std::exchange(other.aligned_data_, nullptr)),
            allocator_(std::move(other.allocator_))
        {
        }

        /**
         * @brief Deleted copy constructor
         */
        buffer_array(const buffer_array &) = delete;

        /**
         * @brief Deleted copy assignment
         */
        buffer_array &operator=(const buffer_array &) = delete;

        /**
         * @brief Move assignment
         */
        buffer_array &operator=(buffer_array &&other) noexcept
        {
            if (this != &other)
            {
                std::swap(count_, other.count_);
                std::swap(allocator_, other.allocator_);
                std::swap(data_, other.data_);
                std::swap(aligned_data_, other.aligned_data_);
            }

            return *this;
        }

        /**
         * @brief Returns number of elements
         *
         * @return Number of elements
         */
        [[nodiscard]] size_t get_count() const noexcept
        {
            return count_;
        }

        /**
         * @brief Returns reference to the element by index
         *
         * Does no checking and assumes the element was successfully allocated
         *
         * @return Reference to the element
         */
        [[nodiscard]] auto &get(size_t index) noexcept
        {
            return aligned_data_[index];
        }

        /**
         * @brief Returns reference to the element by index (const version)
         *
         * Does no checking and assumes the element was successfully allocated
         *
         * @return Const reference to the element
         */
        [[nodiscard]] const auto &get(size_t index) const noexcept
        {
            return aligned_data_[index];
        }

    private:
        size_t      count_{};        /**< Number of elements in the array */
        elem_t     *data_{};         /**< Pointer to the allocated memory */
        elem_t     *aligned_data_{}; /**< Pointer to the array */
        own_alloc_t allocator_{};    /**< Allocator instance */
    };
}  // namespace dml::detail

#endif  //DML_DETAIL_BUFFER_HPP
