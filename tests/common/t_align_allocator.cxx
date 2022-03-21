/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contain definition of the Align Allocator functions
 * @date 2/18/2020
 *
 */

#include <cstdlib>
#include <new>
#include <limits>

template <class T, typename AT1, class U, typename AT2>
bool operator==(const align_allocator_t<T, AT1> &, const align_allocator_t<U, AT2> &)
{
    return true;
}

template <class T, typename AT1, class U, typename AT2>
bool operator!=(const align_allocator_t<T, AT1> &, const align_allocator_t<U, AT2> &)
{
    return false;
}

template<class T, typename AT>
[[nodiscard]] T *align_allocator_t<T, AT>::allocate(std::size_t size)
{
    const size_t align = sizeof(AT);
    const size_t byte_size = size * sizeof(T);

    if (byte_size > std::numeric_limits<std::size_t>::max())
    {
        throw std::bad_alloc();
    }

    auto p = static_cast<T *>(std::aligned_alloc(align, byte_size));

    if (p != nullptr)
    {
        return p;
    }

    throw std::bad_alloc();
}

template<class T, typename AT>
void align_allocator_t<T, AT>::deallocate(T *p, std::size_t) noexcept
{
    std::free(p);
}
