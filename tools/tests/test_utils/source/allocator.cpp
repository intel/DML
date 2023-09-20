/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <dml_test_utils/allocator.hpp>

#include <algorithm>
#include <stdexcept>

#include <malloc.h>

namespace dml::testing
{
    aligned_memory_resource::aligned_memory_resource(std::size_t alignment):
        own_alignment_(alignment)
    {
        constexpr std::size_t allowed_alignments[]{8u, 16u, 32u, 64u};
        if (std::none_of(
                std::begin(allowed_alignments), std::end(allowed_alignments), [this](auto it) {
                    return own_alignment_ == it;
                }))
        {
            throw std::runtime_error("Wrong alignment specified for aligned_memory_resource!");
        }
    }

    aligned_memory_resource::~aligned_memory_resource() = default;

    void *aligned_memory_resource::do_allocate(const size_t bytes, const size_t requested_alignment)
    {
        if (requested_alignment > own_alignment_)
        {
            throw std::runtime_error("Requested alignment is greater than specified for "
                                     "aligned_memory_resource");
        }
#ifdef _MSC_BUILD
        return _aligned_malloc(bytes, own_alignment_);
#else
        return memalign(own_alignment_, bytes);
#endif
    }

    void aligned_memory_resource::do_deallocate(void *       p,
                                                const size_t bytes,
                                                const size_t requested_alignment)
    {
        static_cast<void>(bytes);
        static_cast<void>(requested_alignment);
#ifdef _MSC_BUILD
        return _aligned_free(p);
#else
        return free(p);
#endif
    }

    bool aligned_memory_resource::do_is_equal(const std::PMR_NAMESPACE::memory_resource &other) const noexcept
    {
        if (auto other_ptr = dynamic_cast<const aligned_memory_resource *>(&other))
        {
            return this->own_alignment_ == other_ptr->own_alignment_;
        }
        else
        {
            return false;
        }
    }

    aligned_memory_resource *get_aligned_memory_resource(const std::size_t alignment)
    {
        switch (alignment)
        {
            case 8u:
            {
                static thread_local aligned_memory_resource s_resource{alignment};
                return &s_resource;
            }
            case 16u:
            {
                static thread_local aligned_memory_resource s_resource{alignment};
                return &s_resource;
            }
            case 32u:
            {
                static thread_local aligned_memory_resource s_resource{alignment};
                return &s_resource;
            }
            case 64u:
            {
                static thread_local aligned_memory_resource s_resource{alignment};
                return &s_resource;
            }
            default:
            {
                throw std::runtime_error("Wrong alignment specified for memory_resource");
            }
        }
    }
}  // namespace dml::testing
