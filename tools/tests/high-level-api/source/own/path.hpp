/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TYPED_TESTS_SOURCE_OWN_PATH_HPP
#define DML_TYPED_TESTS_SOURCE_OWN_PATH_HPP

#include "gtest/gtest.h"

#include <dml/dml.hpp>

#include <t_common.hpp>

struct submit {
};
struct execute {
};

#if defined(linux)
using execution_paths = testing::Types<std::pair<dml::software, submit>,
                                       std::pair<dml::software, execute>,
                                       std::pair<dml::hardware, submit>,
                                       std::pair<dml::hardware, execute>>;
using mt_execution_paths = ::testing::Types<std::pair<dml::software, submit>,
                                            std::pair<dml::hardware, submit>>;
#define SKIP_IF_WRONG_PATH(execution_path)                            \
    if (dml::test::variables_t::path == DML_PATH_SW)                  \
    {                                                                 \
        if constexpr (!std::is_same_v<execution_path, dml::software>) \
            return;                                                   \
    }                                                                 \
    else if (dml::test::variables_t::path == DML_PATH_HW)             \
    {                                                                 \
        if constexpr (!std::is_same_v<execution_path, dml::hardware>) \
            return;                                                   \
    }                                                                 \
    else                                                              \
        return

#else
using execution_paths = ::testing::Types<std::pair<dml::software, submit>, std::pair<dml::software, execute>>;
using mt_execution_paths = ::testing::Types<std::pair<dml::software, submit>>;

#define SKIP_IF_WRONG_PATH(execution_path)                            \
    if (dml::test::variables_t::path == DML_PATH_SW)                  \
    {                                                                 \
        if constexpr (!std::is_same_v<execution_path, dml::software>) \
            return;                                                   \
    }                                                                 \
    else                                                              \
        return

#endif

#define DML_TESTING_HL_PARAMETRIZE(name)                                                 \
    template <typename T>                                                                \
    class name: public ::testing::Test                                                   \
    {                                                                                    \
    public:                                                                              \
        using execution_path = typename T::first_type;                                   \
        using execution_type = typename T::second_type;                                  \
                                                                                         \
        template <typename... args_t>                                                    \
        auto run(args_t &&...args) const                                                 \
        {                                                                                \
            if constexpr (std::is_same_v<execution_type, submit>)                        \
            {                                                                            \
                return dml::submit<execution_path>(std::forward<args_t>(args)...).get(); \
            }                                                                            \
            else if (std::is_same_v<execution_type, execute>)                            \
            {                                                                            \
                return dml::execute<execution_path>(std::forward<args_t>(args)...);      \
            }                                                                            \
        }                                                                                \
    };                                                                                   \
                                                                                         \
    TYPED_TEST_SUITE(name, execution_paths)

#define DML_MT_TESTING_HL_PARAMETRIZE(name)                                               \
    template<class value_t>                                                               \
    class name : public ::testing::Test {                                                 \
    public:                                                                               \
        using execution_path = typename value_t::first_type;                              \
        using execution_type = typename value_t::second_type;                             \
        static constexpr uint32_t threads[] = {2, 4, 8};                                  \
        template<class callable_t>                                                        \
        auto run(callable_t callable) {                                                   \
            for (auto thread_count : threads) {                                           \
                callable(thread_count);                                                   \
            }                                                                             \
        }                                                                                 \
    };                                                                                    \
                                                                                          \
    TYPED_TEST_SUITE(name, mt_execution_paths)

/**
 * @brief Allocator that always throws at allocation
 *
 * @tparam T Allocated type
 */
template<typename T>
class exception_allocator {
public:
    /**
     * @brief Alias for allocated type
     */
    using value_type = T;

    /**
     * @brief Default constructor
     */
    exception_allocator() = default;

    /**
     * @brief Copy constructor from other allocator type
     *
     * @tparam U Other allocated type
     */
    template<class U>
    constexpr exception_allocator(const exception_allocator<U> &) noexcept {
    }

    /**
     * @brief Always throws
     *
     * @param size Not used
     * @return Returns nothing
     */
    T *allocate(std::size_t size) const {
        throw std::runtime_error("Thrown by testing allocator.");
        return nullptr;
    }

    /**
     * @brief Do nothing
     *
     * @param p Pointer "returned" from allocate
     * @param size Allocated size
     */
    void deallocate(T *p, std::size_t size) const noexcept { return; }
};

#endif  //_DML_EXECUTION_PATH_HPP_
