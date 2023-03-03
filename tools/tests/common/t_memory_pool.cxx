/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/


namespace dml::test
{

    template<typename T>
    inline auto dml::test::memory_pool<T>::allocate(const size_type_t size)
        noexcept -> inner_buffer_t &
    {
        auto lock_guard = std::lock_guard<std::mutex>(m_allocation_mutex);

        for (auto i = m_last_free_buffer; i < capacity(); ++i)
        {
            if (size <= m_buffer[i].capacity())
            {
                std::swap(m_buffer[i], m_buffer[m_last_free_buffer]);
                m_buffer[m_last_free_buffer].resize(size);
                std::fill(m_buffer[m_last_free_buffer].begin(), m_buffer[m_last_free_buffer].end(), 0u);
                return m_buffer[m_last_free_buffer++];
            }
        }

        if (m_buffer.size() <= m_last_free_buffer)
        {
            m_buffer.push_back(std::move(inner_buffer_t(size)));
        }
        else
        {
            m_buffer[m_last_free_buffer] = std::move(inner_buffer_t(size));
        }

        std::fill(m_buffer[m_last_free_buffer].begin(), m_buffer[m_last_free_buffer].end(), 0u);

        return m_buffer[m_last_free_buffer++];
    }


    template<typename T>
    template<class input_iterator>
    inline auto dml::test::memory_pool<T>::allocate(const input_iterator &first,
                                                    const input_iterator &last) 
        noexcept -> inner_buffer_t &
    {
        auto &memory_buffer = allocate(std::distance(first, last));
        std::copy(first, last, memory_buffer.begin());

        return memory_buffer;
    }


    template<typename T>
    inline auto dml::test::memory_pool<T>::allocate(const size_type_t size,
                                                    const value_type_t value) 
        noexcept -> inner_buffer_t &
    {
        auto &memory_buffer = allocate(size);
        std::fill(memory_buffer.begin(), memory_buffer.end(), value);
        return memory_buffer;
    }


    template<typename T>
    inline auto dml::test::memory_pool<T>::allocate_ptr(const size_type_t size)
        noexcept -> pointer_t
    {
        return allocate(size).data();
    }


    template<typename T>
    template<class input_iterator>
    inline auto dml::test::memory_pool<T>::allocate_ptr(const input_iterator &first,
                                                        const input_iterator &last) 
            noexcept -> pointer_t
    {
        return allocate(first, last).data();
    }


    template<typename T>
    inline auto dml::test::memory_pool<T>::allocate_ptr(const size_type_t size,
                                                        const value_type_t value) 
        noexcept -> pointer_t
    {
        return allocate(size, value).data();
    }


    template<typename T>
    inline constexpr auto dml::test::memory_pool<T>::deallocate_all() noexcept -> void
    {
        m_last_free_buffer = size_type_t(0); 
    }


    template<typename T>
    inline auto dml::test::memory_pool<T>::shrink_to_fit() noexcept -> void
    {
        m_buffer.resize(m_last_free_buffer);
        m_buffer.shrink_to_fit();
    }


    template<typename T>
    inline constexpr auto dml::test::memory_pool<T>::size()
        noexcept -> size_type_t
    {
        return m_last_free_buffer;
    }


    template<typename T>
    inline constexpr auto dml::test::memory_pool<T>::capacity()
        noexcept -> size_type_t
    {
        return m_buffer.size();
    }


    template<typename T>
    inline auto dml::test::memory_pool<T>::reserve(const size_type_t size)
        noexcept -> void
    {
        m_buffer.reserve(size);
    }

}
