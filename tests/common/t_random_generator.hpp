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
#ifndef DML_TEST_RNG
#define DAS_TEST_RNG


#include <random>
#include <cstdint>

#include "t_range.hpp"


namespace dml::test
{

    /**
     * @brief Class that performs base for random number generation
     */
    template<typename value_type, typename result_type>
    class base_random_t
    {
    public:
        /**
         * @brief Wrapper for value type
         */
        using value_type_t     = value_type;
        /**
         * @brief Wrapper for result type
         */
        using result_type_t    = result_type;
        /**
         * @brief Wrapper for seed type
         */
        using seed_type_t      = uint32_t;
        /**
         * @brief Wrapper for generator type
         */
        using generator_type_t = std::mt19937;
        /**
         * @brief Wrapper for ragne type
         */
        using range_type_t     = dml::test::range_t<value_type_t>;



        /**
         * @brief Default constructor
         */
        base_random_t(const seed_type_t seed = 0u) noexcept;

        /**
         * @brief Range constructor
         */
        base_random_t(const range_type_t &range, 
                      const seed_type_t seed = 0u) noexcept;


        /**
         * @brief Generates random number
         */
        [[nodiscard]] auto get_next() noexcept -> result_type;

        /**
         * @brief Generates random number
         */
        [[nodiscard]] auto operator()() noexcept -> result_type;

    private:
        /**
         * @brief Range for generation
         */
        range_type_t     m_range;
        /**
         * @brief Using generator
         */
        generator_type_t m_generator;
    };

    
    /**
     * @brief Specialization for float data type
     */
    template<typename result_type>
    class base_random_t<float, result_type>
    {
    public:
        /**
         * @brief Wrapper for value type
         */
        using value_type_t     = float;
        /**
         * @brief Wrapper for result type
         */
        using result_type_t    = result_type;
        /**
         * @brief Wrapper for seed type
         */
        using seed_type_t      = uint32_t;
        /**
         * @brief Wrapper for generator type
         */
        using generator_type_t = std::mt19937;
        /**
         * @brief Wrapper for ragne type
         */
        using range_type_t     = dml::test::range_t<result_type_t>;



        /**
         * @brief Default constructor
         */
        base_random_t(const seed_type_t seed = 0u) noexcept;

        /**
         * @brief Range constructor
         */
        base_random_t(const range_type_t &range, 
                      const seed_type_t seed = 0u) noexcept;


        /**
         * @brief Generates random number
         */
        [[nodiscard]] auto get_next() noexcept -> result_type;

        /**
         * @brief Generates random number
         */
        [[nodiscard]] auto operator()() noexcept -> result_type;

    private:
        /**
         * @brief Range for generation
         */
        range_type_t     m_range;
        /**
         * @brief Using generator
         */
        generator_type_t m_generator;
    };


    /**
     * @brief Specialization for double data type
     */
    template<typename result_type>
    class base_random_t<double, result_type>
    {
    public:
        /**
         * @brief Wrapper for value type
         */
        using value_type_t     = double;
        /**
         * @brief Wrapper for result type
         */
        using result_type_t    = result_type;
        /**
         * @brief Wrapper for seed type
         */
        using seed_type_t      = uint32_t;
        /**
         * @brief Wrapper for generator type
         */
        using generator_type_t = std::mt19937;
        /**
         * @brief Wrapper for ragne type
         */
        using range_type_t     = dml::test::range_t<result_type_t>;



        /**
         * @brief Default constructor
         */
        base_random_t(const seed_type_t seed = 0u) noexcept;

        /**
         * @brief Range constructor
         */
        base_random_t(const range_type_t &range, 
                      const seed_type_t seed = 0u) noexcept;


        /**
         * @brief Generates random number
         */
        [[nodiscard]] auto get_next() noexcept -> result_type;

        /**
         * @brief Generates random number
         */
        [[nodiscard]] auto operator()() noexcept -> result_type;

    private:
        /**
         * @brief Range for generation
         */
        range_type_t     m_range;
        /**
         * @brief Using generator
         */
        generator_type_t m_generator;
    };


    /**
     * @brief Random number generator itself
     */
    template<typename value_type>
    class random_t : public base_random_t<value_type, value_type>
    {
    public:
        /**
         * @brief Wrapper for base type
         */
        using base_type_t      = base_random_t<value_type, value_type>;
        /**
         * @brief Wrapper for value type
         */
        using value_type_t     = typename base_type_t::result_type_t;
        /**
         * @brief Wrapper for seed type
         */
        using seed_type_t      = typename base_type_t::seed_type_t;
        /**
         * @brief Wrapper for generator type
         */
        using generator_type_t = typename base_type_t::generator_type_t;
        /**
         * @brief Wrapper for range type
         */
        using range_type_t     = typename base_type_t::range_type_t;



        /**
         * @brief Default constructor
         */
        random_t(const seed_type_t seed = 0u) noexcept
            : base_type_t(seed)
        {
        }

        /**
         * @brief Range constructor
         */
        random_t(const range_type_t &range, const seed_type_t seed = 0u) noexcept
            : base_type_t(range, seed)
        {
        }

    };


    /**
     * @brief Specialization for uint8 data type 
     */
    template<>
    class random_t<uint8_t> : public base_random_t<uint32_t, uint8_t>
    {
    public:
        /**
         * @brief Wrapper for base type
         */
        using base_type_t      = base_random_t<uint32_t, uint8_t>;
        /**
         * @brief Wrapper for value type
         */
        using value_type_t     = typename base_type_t::result_type_t;
        /**
         * @brief Wrapper for seed type
         */
        using seed_type_t      = typename base_type_t::seed_type_t;
        /**
         * @brief Wrapper for generator type
         */
        using generator_type_t = typename base_type_t::generator_type_t;
        /**
         * @brief Wrapper for range type
         */
        using range_type_t     = typename base_type_t::range_type_t;



        /**
         * @brief Default constructor
         */
        random_t(const seed_type_t seed = 0u) noexcept
            : base_type_t(seed)
        {
        }

        /**
         * @brief Range constructor
         */
        random_t(const range_type_t &range, 
                 const seed_type_t seed = 0u) noexcept
            : base_type_t(range, seed)
        {
        }
    };


    /**
     * @brief Specialization for int8 data type 
     */
    template<>
    class random_t<int8_t> : public base_random_t<int32_t, int8_t>
    {
    public:
        /**
         * @brief Wrapper for base type
         */
        using base_type_t      = base_random_t<int32_t, int8_t>;
        /**
         * @brief Wrapper for value type
         */
        using value_type_t     = typename base_type_t::result_type_t;
        /**
         * @brief Wrapper for seed type
         */
        using seed_type_t      = typename base_type_t::seed_type_t;
        /**
         * @brief Wrapper for generator type
         */
        using generator_type_t = typename base_type_t::generator_type_t;
        /**
         * @brief Wrapper for range type
         */
        using range_type_t     = typename base_type_t::range_type_t;



        /**
         * @brief Default constructor
         */
        random_t(const seed_type_t seed = 0u) noexcept
            : base_type_t(seed)
        {
        }

        /**
         * @brief Range constructor
         */
        random_t(const range_type_t &range, 
                 const seed_type_t seed = 0u) noexcept
            : base_type_t(range, seed)
        {
        }
    };


    /**
     * @brief Specialization for uint16 data type 
     */
    template<>
    class random_t<uint16_t> : public base_random_t<uint32_t, uint16_t>
    {
    public:
        /**
         * @brief Wrapper for base type
         */
        using base_type_t      = base_random_t<uint32_t, uint16_t>;
        /**
         * @brief Wrapper for value type
         */
        using value_type_t     = typename base_type_t::result_type_t;
        /**
         * @brief Wrapper for seed type
         */
        using seed_type_t      = typename base_type_t::seed_type_t;
        /**
         * @brief Wrapper for generator type
         */
        using generator_type_t = typename base_type_t::generator_type_t;
        /**
         * @brief Wrapper for range type
         */
        using range_type_t     = typename base_type_t::range_type_t;



        /**
         * @brief Default constructor
         */
        random_t(const seed_type_t seed = 0u) noexcept
            : base_type_t(seed)
        {
        }

        /**
         * @brief Range constructor
         */
        random_t(const range_type_t &range,
                 const seed_type_t seed = 0u) noexcept
            : base_type_t(range, seed)
        {
        }
    };


    /**
     * @brief Specialization for int16 data type 
     */
    template<>
    class random_t<int16_t> : public base_random_t<int32_t, int16_t>
    {
    public:
        /**
         * @brief Wrapper for base type
         */
        using base_type_t      = base_random_t<int32_t, int16_t>;
        /**
         * @brief Wrapper for value type
         */
        using value_type_t     = typename base_type_t::result_type_t;
        /**
         * @brief Wrapper for seed type
         */
        using seed_type_t      = typename base_type_t::seed_type_t;
        /**
         * @brief Wrapper for generator type
         */
        using generator_type_t = typename base_type_t::generator_type_t;
        /**
         * @brief Wrapper for range type
         */
        using range_type_t     = typename base_type_t::range_type_t;



        /**
         * @brief Default constructor
         */
        random_t(const seed_type_t seed = 0u) noexcept
            : base_type_t(seed)
        {
        }

        /**
         * @brief Range constructor
         */
        random_t(const range_type_t &range,
                 const seed_type_t seed = 0u) noexcept
            : base_type_t(range, seed)
        {
        }
    };

}


#include "t_random_generator.cxx"


#endif
