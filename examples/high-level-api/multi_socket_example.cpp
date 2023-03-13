/*******************************************************************************
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <dml/dml.hpp>
#include <iostream>
#include <vector>

constexpr auto socket_count = 4u;

// 1 MB per socket
constexpr auto transfer_size = 1024u * 1024 * socket_count;

int main()
{
    std::cout << "Starting example for multi-socket memory move:\n";

    auto src = std::vector<std::uint8_t>(transfer_size, 1);
    auto dst = std::vector<std::uint8_t>(transfer_size, 0);

    auto handlers =
        std::vector<dml::handler<dml::mem_move_operation, std::allocator<std::uint8_t>>>();
    handlers.reserve(socket_count);

    auto results = std::vector<dml::mem_move_result>();

    for (auto i = 0u; i < socket_count; ++i)
    {
        auto execution_interface = dml::default_execution_interface<dml::automatic>();
        handlers.emplace_back(dml::submit<dml::automatic>(dml::mem_move,
                                                          dml::make_view(src),
                                                          dml::make_view(dst),
                                                          execution_interface,
                                                          i));
    }

    for (auto& handler : handlers)
    {
        results.emplace_back(handler.get());
    }

    if (src != dst)
    {
        std::cout << "Fail!\n";
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "Success!\n";
        return EXIT_SUCCESS;
    }
}
