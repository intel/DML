/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef _DML_TEST_UTILS_ALLOCATOR_HPP_
#define _DML_TEST_UTILS_ALLOCATOR_HPP_

#if (__GNUC__ < 9) && !defined(_MSC_BUILD)
#include <experimental/memory_resource>
#define PMR_NAMESPACE experimental::pmr
#else
#include <memory_resource>
#define PMR_NAMESPACE pmr
#endif

#include <cstdint>

namespace dml::testing
{
    /**
     * @todo
     */
    class aligned_memory_resource: public std::PMR_NAMESPACE::memory_resource
    {
    public:
        /**
         * @todo
         */
        explicit aligned_memory_resource(std::size_t alignment);

        /**
         * @todo
         */
        ~aligned_memory_resource() override;

    private:
        /**
         * @todo
         */
        void *do_allocate(std::size_t bytes, std::size_t requested_alignment) override;

        /**
         * @todo
         */
        void do_deallocate(void *p, std::size_t bytes, std::size_t requested_alignment) override;

        /**
         * @todo
         */
        [[nodiscard]] bool do_is_equal(const memory_resource &other) const noexcept override;

    private:
        std::size_t own_alignment_; /**< @todo */
    };

    /**
     * @todo
     */
    aligned_memory_resource* get_aligned_memory_resource(std::size_t alignment);
}  // namespace dml::testing

#endif  //_DML_TEST_UTILS_ALLOCATOR_HPP_
