/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @file
 * @brief Contains RNG for tests
 * @date 4/17/2020
 */


namespace dml::test
{

    template<typename value_type, typename result_type>
    inline base_random_t<value_type, result_type>::base_random_t(const seed_type_t seed) noexcept
        :m_range()
    {
        m_generator.seed(seed);
    }


    template<typename value_type, typename result_type>
    inline base_random_t<value_type, result_type>::base_random_t(const range_type_t &range,
                                                                 const seed_type_t seed) noexcept
        : base_random_t(seed)
    {
        m_range = range;
    }


    template<typename value_type, typename result_type>
    inline auto base_random_t<value_type, result_type>::get_next() noexcept -> result_type
    {
        std::uniform_int_distribution<value_type_t> distribution(m_range.min, m_range.max);

        return static_cast<result_type_t>(distribution(m_generator));
    }


    template<typename value_type, typename result_type>
    inline auto base_random_t<value_type, result_type>::operator()() noexcept -> result_type
    {
        return get_next();
    }



    template<typename result_type>
    inline base_random_t<float, result_type>::base_random_t(const seed_type_t seed) noexcept
    {
        m_generator.seed(seed);
    }


    template<typename result_type>
    inline base_random_t<float, result_type>::base_random_t(const range_type_t &range, 
                                                            const seed_type_t seed) noexcept
        : base_random_t(seed)
    {
        m_range = range;
    }


    template<typename result_type>
    inline auto base_random_t<float, result_type>::get_next() noexcept -> result_type
    {
        std::uniform_real_distribution<value_type_t> distribution(m_range.min, m_range.max);

        return static_cast<result_type_t>(distribution(m_generator));
    }


    template<typename result_type>
    inline auto base_random_t<float, result_type>::operator()() noexcept -> result_type
    {
        return get_next();
    }



    template<typename result_type>
    inline base_random_t<double, result_type>::base_random_t(const seed_type_t seed) noexcept
    {
        m_generator.seed(seed);
    }


    template<typename result_type>
    inline base_random_t<double, result_type>::base_random_t(const range_type_t &range, 
                                                            const seed_type_t seed) noexcept
        : base_random_t(seed)
    {
        m_range = range;
    }


    template<typename result_type>
    inline auto base_random_t<double, result_type>::get_next() noexcept -> result_type
    {
        std::uniform_real_distribution<value_type_t> distribution(m_range.min, m_range.max);

        return static_cast<result_type_t>(distribution(m_generator));
    }


    template<typename result_type>
    inline auto base_random_t<double, result_type>::operator()() noexcept -> result_type
    {
        return get_next();
    }

}
