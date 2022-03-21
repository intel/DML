/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contain allocator to allocate aligned memory
 * @date 2/18/2020
 *
 */

#ifndef DML_T_ALIGN_ALLOCATOR_HPP__
#define DML_T_ALIGN_ALLOCATOR_HPP__

#if 0
/**
 * @brief      Allocate an aligned memory region
 * @tparam T   Allocated type
 * @tparam AT  Type that size will be used as align
 */
template <class T, typename AT>
struct align_allocator_t
{
    using value_type = T; /**< Redefinition of template parameter*/

    /**
     * @brief Constructor
     */
    align_allocator_t () = default;

    /**
     * @brief Copy constructor
     */
    template <class U, typename A>
    constexpr explicit align_allocator_t(const align_allocator_t<U, A> &) noexcept
    {
        // No action required;
    }

    /**
     * @brief Allocates memory region
     *
     * @param[in] size  memory region size for allocation
     *
     * @return Pointer of template type to the first element of the allocated memory
     */
    [[nodiscard]] T *allocate(std::size_t size);

    /**
     * @brief Frees  memory region which was allocated with allocate method of @ref align_allocator_t
     *
     * @param[in] p pointer to the allocated memory
     */
    void deallocate(T *p, std::size_t) noexcept;
};

#include "t_align_allocator.cxx"

#endif

#endif //DML_T_ALIGN_ALLOCATOR_HPP__
