/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_T_TEST_CASE_GENERATOR_HPP
#define DML_T_TEST_CASE_GENERATOR_HPP

#include "gtest/gtest.h"
#include "t_random_parameters.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

namespace dml::test {
    /**
     * @brief Provides test case interface to tests
     */
    template <class test_case_t>
    class test_case_generator {
    public:
        /**
         * @brief Constructor, accepts function which is capable of generating vector of test cases
         */
        template <typename generator_function_t,
                  class ...args_t>
        test_case_generator(generator_function_t generator_function, args_t ...generator_args) {
            test_case_list = generator_function(generator_args...);
        }

        /**
         * @brief Method to support the range based for loop
         * If testid parameter was not specified, returns iterator pointing to the first element in test case vector,
         * returns iterator pointing to the required test case otherwise
         */
        auto begin() {
            if (test_system::get_test_id() == -1) {
                return test_case_list.begin();
            } else {
                if (test_system::get_test_id() > 0 &&
                    test_system::get_test_id() < test_case_list.size()) {
                    return test_case_list.begin() + test_system::get_test_id();
                } else {
                    std::cout << "Incorrect test id was specified\n";
                    std::cout << "There are " << test_case_list.size() << " test cases, " <<
                              "test case number " << test_system::get_test_id() << "was specified.\n";
                    std::cout << "Ignoring testid parameter for this test.\n";

                    return test_case_list.begin();
                }
            }
        }

        /**
         * @brief Method to support the range based for loop
         * If testid parameter was not specified, returns iterator pointing to the end in test case vector,
         * returns iterator pointing to the required test case + 1 otherwise
         */
        auto end() {
            if (test_system::get_test_id() == -1) {
                return test_case_list.end();
            } else {
                if (test_system::get_test_id() > 0 &&
                    test_system::get_test_id() < test_case_list.size()) {
                    return test_case_list.begin() + test_system::get_test_id() + 1;
                } else {
                    return test_case_list.end();
                }
            }
        }

        /**
         * @brief Searches the passed test case in vector of test cases and provides information about it
         */
        auto info(test_case_t &test_case) -> std::string {
            // Find the test case in the list of test cases, and get it's index
            auto found_value = std::find(test_case_list.begin(), test_case_list.end(), test_case);
            auto test_case_index = static_cast<uint32_t>(std::distance(test_case_list.begin(), found_value));

            if (test_case_index < test_case_list.size()) {
                std::ostringstream ostring_stream;
                ostring_stream << test_case;
                std::string current_test_case_info = ostring_stream.str();
                std::string message = "Current test case id = " + std::to_string(test_case_index) + ".\n";
                message += current_test_case_info + "\n";
                message += "To reproduce this test case, rerun test with the following options:\n";
                message += "--test_id=" + std::to_string(test_case_index) + " --seed=" + std::to_string(test_system::get_seed()) + "\n";
                return message;
            } else {
                std::string message = "testid=" + std::to_string(test_case_index) + " was specified\n";
                message += "but this tests has only " + std::to_string(test_case_list.size()) + " test cases\n";
                return message;
            }
        }

    private:
        /**
         * @brief Vector which keeps all the test cases
         */
        std::vector<test_case_t> test_case_list;
    };
}


#endif //DML_T_TEST_CASE_GENERATOR_HPP
