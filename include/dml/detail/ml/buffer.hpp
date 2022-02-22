/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_DETAIL_ML_BUFFER_HPP
#define DML_DETAIL_ML_BUFFER_HPP

#include <dml/detail/ml/allocator.hpp>
#include <dml/detail/ml/utils.hpp>
#include <memory>

namespace dml::detail::ml
{
    template <typename buffer_t>
    class buffer_view;

    template <typename allocator_t, typename... elements_t>
    class buffer
    {
        friend buffer_view<buffer>;

    private:
        using allocator_type = allocator_t;
        using element_type   = utils::structure_from<elements_t...>;

        static constexpr auto memory_size      = sizeof(element_type);
        static constexpr auto memory_alignment = alignof(element_type);
        static constexpr auto allocate_size    = memory_size + memory_alignment;

        using own_alloc_t =
            typename std::allocator_traits<allocator_t>::template rebind_alloc<element_type>;

        using own_traits_t =
            typename std::allocator_traits<allocator_t>::template rebind_traits<element_type>;

    public:
        explicit buffer(allocator_t allocator = allocator_t()):
            allocated_memory_(nullptr),
            aligned_memory_(nullptr),
            allocator_(allocator)
        {
            allocated_memory_ = own_traits_t::allocate(allocator_, allocate_size);
            aligned_memory_   = utils::align<memory_alignment>(allocated_memory_);

            own_traits_t::construct(allocator_, aligned_memory_);
        }

        buffer(const buffer& other) = delete;

        buffer(buffer&& other) noexcept:
            allocated_memory_(std::exchange(other.allocated_memory_, nullptr)),
            aligned_memory_(std::exchange(other.aligned_memory_, nullptr)),
            allocator_(std::move(other.allocator_))
        {
        }

        buffer& operator=(const buffer& other) = delete;

        buffer& operator=(buffer&& other) noexcept
        {
            if (this != &other)
            {
                std::swap(allocated_memory_, other.allocated_memory_);
                std::swap(aligned_memory_, other.aligned_memory_);
                std::swap(allocator_, other.allocator_);
            }

            return *this;
        }

        ~buffer() noexcept
        {
            if (allocated_memory_)
            {
                own_traits_t::destroy(allocator_, aligned_memory_);
                own_traits_t::deallocate(allocator_, allocated_memory_, allocate_size);
            }
        }

    private:
        element_type* allocated_memory_;
        element_type* aligned_memory_;
        own_alloc_t   allocator_;
    };

    template <typename... elements_t>
    class buffer<stack_allocator, elements_t...>
    {
        friend buffer_view<buffer>;

    private:
        using allocator_type = stack_allocator;
        using element_type   = utils::structure_from<elements_t...>;

    public:
        explicit buffer(stack_allocator allocator): element_()
        {
            static_cast<void>(allocator);
        }

        buffer(const buffer& other) = delete;

        buffer(buffer&& other) noexcept = default;

        buffer& operator=(const buffer& other) = delete;

        buffer& operator=(buffer&& other) noexcept = default;

        ~buffer() noexcept = default;

    private:
        element_type element_;
    };
}  // namespace dml::detail::ml

#endif  //DML_DETAIL_ML_BUFFER_HPP
