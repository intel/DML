/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef _DML_TEST_UTILS_DATA_STORAGE_HPP_
#define _DML_TEST_UTILS_DATA_STORAGE_HPP_

#include <dml_test_utils/allocator.hpp>
#include <vector>
#include <algorithm>

namespace dml::testing
{

#if (__GNUC__ < 9) && !defined(_MSC_BUILD)
using vector_impl_t = std::vector<uint8_t, std::experimental::pmr::polymorphic_allocator<uint8_t>>;
#else
using vector_impl_t = std::pmr::vector<uint8_t>;
#endif

    /**
     * @todo
     */
    class data_storage: public vector_impl_t
    {
        using base        = vector_impl_t;
        using allocator_t = base::allocator_type;

    public:
        /**
         * @todo
         */
        explicit data_storage(uint32_t size, uint32_t alignment);

        template <typename generator_t>
        void generate(generator_t&& generator)
        {
            std::generate(this->begin(), this->end(), std::forward<generator_t>(generator));
        }
    };
}  // namespace dml::testing

#endif  //_DML_TEST_UTILS_DATA_STORAGE_HPP_
