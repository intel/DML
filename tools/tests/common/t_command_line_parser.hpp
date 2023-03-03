/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @file
 * @brief
 * @date 4/14/2020
 */
#ifndef DML_TEST_COMMAND_LINE_PARSER
#define DML_TEST_COMMAND_LINE_PARSER


#include <unordered_map>
#include <algorithm>
#include <string>


namespace dml
{

    namespace test
    {

        /**
         * @brief Command line parser for tests
         */
        class command_line_arguments_parser_t
        {

            public:

                /**
                 * @brief Key value for argument
                 */
                using key_type_t   = std::string;

                /**
                 * @brief Value type corresponding to the argument
                 */
                using value_type_t = std::string;

                /**
                 * @brief Main buffer that stores arguments with their values
                 */
                using buffer_t     = std::unordered_map<key_type_t, value_type_t>;



                /**
                 * @brief Empty constructor
                 */
                command_line_arguments_parser_t() noexcept = delete;


                /**
                 * @brief Command line parsing function
                 */
                static auto parse(int count, char **args_pptr) noexcept -> void
                {
                    for (int i = 1; i < count; ++i)
                    {
                        auto argument       = key_type_t(args_pptr[i]);
                        auto equal_sign_pos = std::find(argument.begin(), argument.end(), '=');

                        m_command_line_buffer.insert({key_type_t(argument.begin(), equal_sign_pos),
                                                     (argument.end() == equal_sign_pos) ?
                                                     (value_type_t()) :
                                                     (value_type_t(equal_sign_pos + 1u, argument.end()))});
                    }
                }


                /**
                 * @brief Checker if the argument had been parsed
                 */
                static auto exists(const key_type_t &key) noexcept -> bool
                {
                    return m_command_line_buffer.end() != m_command_line_buffer.find(key);
                }


                /**
                 * @brief Value extractor
                 */
                template<typename T>
                static auto get_value(const key_type_t &key) noexcept -> T
                {
                    if ((exists(key)) && (!m_command_line_buffer.at(key).empty()))
                    {
                        const auto value = std::string(m_command_line_buffer.at(key));
                        m_command_line_buffer.erase(key);
                        return static_cast<T>(std::stold(value));
                    }

                    return T();
                }


                /**
                 * @brief String extractor
                 */
                /**
                 * TODO: Create specialization for get_value<std::string>
                 *       Nowadays it's not supported by GCC Compiler
                 *       Link to the issue: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=85282
                 */
                static auto get_string(const key_type_t &key) noexcept -> std::string
                {
                    if (exists(key) && !m_command_line_buffer.at(key).empty())
                    {
                        const auto value = std::string(m_command_line_buffer.at(key));
                        m_command_line_buffer.erase(key);
                        return value;
                    }

                    return std::string();
                }


                /**
                 * @brief Removes the item
                 */
                static auto remove(const key_type_t &key) noexcept -> void
                {
                    if (exists(key))
                    {
                        m_command_line_buffer.erase(key);
                    }
                }


                /**
                 * @brief Checks inner buffer for emptiness
                 */
                [[nodiscard]]
                static auto empty() noexcept -> bool
                {
                    return m_command_line_buffer.empty();
                }


                /**
                 * @brief Size of the inner buffer
                 */
                [[nodiscard]]
                static auto size() noexcept -> buffer_t::size_type
                {
                    return m_command_line_buffer.size();
                }

            private:

                /**
                 * @brief Arguments buffer
                 */
                static inline buffer_t m_command_line_buffer;

        };

    }

}


#endif // DML_TEST_COMMAND_LINE_PARSER
