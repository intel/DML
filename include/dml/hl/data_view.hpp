/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_DATA_VIEW_HPP
#define DML_DATA_VIEW_HPP

/**
 * @date 11/25/2020
 * @brief Contains definition of @ref data_view class
 */

#include <cstdint>
#include <dml/hl/types.hpp>
#include <iterator>
#include <type_traits>

namespace dml
{
    class const_data_view;

    /**
     * @ingroup dmlhl dmlhl_aux
     * @brief Represents arbitrary memory region (data) in a uniform way.
     *
     * Use @ref dml::make_view to create a view to your data.
    */
    class data_view
    {
    public:
        /**
         * @brief Constructor from a pointer and a size
         *
         * @param[in] data_ptr  Pointer to the data for the view
         * @param[in] size      Byte size of the data for the view
         */
        data_view(byte_t *const data_ptr, const size_t size) noexcept: data_ptr_(data_ptr), size_(size)
        {
        }

        /**
         * @brief Explicitly deleted conversion constructor from @ref const_data_view
         *
         * It is forbidden make immutable data mutable.
         */
        data_view(const_data_view) = delete;

        /**
         * @brief Default copy constructor
         */
        data_view(const data_view &) noexcept = default;

        /**
         * @brief Default move constructor
         */
        data_view(data_view &&) noexcept = default;

        /**
         * @brief Deleted copy assignment
         *
         * @return Self
         */
        data_view &operator=(const data_view &) = delete;

        /**
         * @brief Deleted move assignment
         *
         * @return Self
         */
        data_view &operator=(data_view &&) = delete;

        /**
         * @brief Getter for the pointer to the viewed data
         *
         * @return Pointer to the viewed data
         */
        [[nodiscard]] auto data() noexcept
        {
            return data_ptr_;
        }

        /**
         * @brief Getter for the size of the viewed data
         *
         * @return Size of the viewed data
         */
        [[nodiscard]] auto size() const noexcept
        {
            return size_;
        }

    private:
        byte_t *const  data_ptr_; /**< Pointer to the viewed data */
        const uint32_t size_;     /**< Size of the viewed data */
    };

    /**
     * @ingroup dmlhl dmlhl_aux
     * @brief Represents arbitrary immutable memory region (data) in a uniform way.
     *
     * Use @ref dml::make_view to create a view to your data.
    */
    class const_data_view
    {
    public:
        /**
         * @brief Constructor from a pointer and a size
         *
         * @param[in] data_ptr  Pointer to the data for the view
         * @param[in] size      Byte size of the data for the view
         */
        const_data_view(const byte_t *const data_ptr, const size_t size) noexcept: data_ptr_(data_ptr), size_(size)
        {
        }

        /**
         * @brief Constructor @ref data_view
         *
         * Intentionally implicit, because there is no harm in making data const.
         *
         * @param[in] other Instance of @ref data_view
         */
        const_data_view(data_view other) noexcept: data_ptr_(other.data()), size_(other.size())
        {
        }

        /**
         * @brief Default copy constructor
         */
        const_data_view(const const_data_view &) noexcept = default;

        /**
         * @brief Default move constructor
         */
        const_data_view(const_data_view &&) noexcept = default;

        /**
         * @brief Deleted copy assignment
         *
         * @return Self
         */
        data_view &operator=(const data_view &) = delete;

        /**
         * @brief Deleted move assignment
         *
         * @return Self
         */
        data_view &operator=(data_view &&) = delete;

        /**
         * @brief Getter for the pointer to the viewed data
         *
         * @return Pointer to the viewed data
         */
        [[nodiscard]] auto data() const noexcept
        {
            return data_ptr_;
        }

        /**
         * @brief Getter for the size of the viewed data
         *
         * @return Size of the viewed data
         */
        [[nodiscard]] auto size() const noexcept
        {
            return size_;
        }

    private:
        const byte_t *const data_ptr_; /**< Pointer to the viewed immutable data */
        const uint32_t      size_;     /**< Size of the viewed data */
    };

    /**
     * @ingroup dmlhl dmlhl_aux
     * @brief Factory function for Data View construction from raw pointer and length
     *
     * For mutable data constructs @ref data_view. For immutable data constructs @ref const_data_view
     *
     * @tparam    data_type  Underlying data type
     * @param[in] data_ptr   Pointer to the data for the view
     * @param[in] length     Length of the data for the view
     *
     * Usage:
     * @code
     * auto data = new int[size];
     * auto view = dml::make_view(data, size);
     * @endcode
     *
     * @return Constructed @ref data_view or @ref const_data_view
     */
    template <typename data_type>
    inline auto make_view(data_type *const data_ptr, std::size_t length) noexcept
    {
        if constexpr (std::is_const_v<data_type>)
        {
            const auto byte_size = static_cast<dml::size_t>(length * sizeof(data_type));
            return const_data_view(reinterpret_cast<const byte_t *>(data_ptr), byte_size);
        }
        else
        {
            const auto byte_size = static_cast<dml::size_t>(length * sizeof(data_type));
            return data_view(reinterpret_cast<byte_t *>(data_ptr), byte_size);
        }
    }

    /**
     * @ingroup dmlhl dmlhl_aux
     * @brief Factory function for @ref data_view construction from a pair of two iterators
     *
     * @tparam    iterator_t  Iterators type
     * @param[in] begin       Begin iterator for a data range for the view
     * @param[in] end         End iterator for a data range for the view
     *
     * Usage:
     * @code
     * auto data = std::vector<uint8_t>(size);
     * auto view = dml::make_view(data.begin(), data.end());
     * @endcode
     *
     * @return Constructed @ref data_view
     */
    template <typename iterator_t>
    inline auto make_view(iterator_t begin, iterator_t end) noexcept(noexcept(&*begin) &&noexcept(std::distance(begin, end)))
    {
        using iterator_category = typename std::iterator_traits<iterator_t>::iterator_category;
        static_assert(std::is_same_v<std::random_access_iterator_tag, iterator_category>, "Only random access iterators are supported.");
        return make_view(&*begin, std::distance(begin, end));
    }

    /**
     * @ingroup dmlhl dmlhl_aux
     * @brief Factory function for @ref data_view construction from a range (const)
     *
     * @tparam    range_t Range type
     * @param[in] range   Range for the view
     *
     * Usage:
     * @code
     * const auto data = std::vector<uint8_t>(size);
     * auto view = dml::make_view(data);
     * @endcode
     *
     * @return Constructed @ref data_view
     */
    template <typename range_t>
    inline auto make_view(const range_t &range) noexcept(noexcept(std::cbegin(range)) &&noexcept(std::cend(range)))
    {
        return make_view(std::cbegin(range), std::cend(range));
    }

    /**
     * @ingroup dmlhl dmlhl_aux
     * @brief Factory function for @ref data_view construction from a range
     *
     * @tparam    range_t Range type
     * @param[in] range   Range for the view
     *
     * Usage:
     * @code
     * auto data = std::vector<uint8_t>(size);
     * auto view = dml::make_view(data);
     * @endcode
     *
     * @return Constructed @ref data_view
     */
    template <typename range_t>
    inline auto make_view(range_t &range) noexcept(noexcept(std::begin(range)) &&noexcept(std::end(range)))
    {
        return make_view(std::begin(range), std::end(range));
    }

}  // namespace dml

#endif  //DML_DATA_VIEW_HPP
