// toy-echo.cpp                                                       -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2022 Dietmar Kuehl http://www.dietmar-kuehl.de
//
//  Permission is hereby granted, free of charge, to any person
//  obtaining a copy of this software and associated documentation
//  files (the "Software"), to deal in the Software without restriction,
//  including without limitation the rights to use, copy, modify,
//  merge, publish, distribute, sublicense, and/or sell copies of
//  the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be
//  included in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//  OTHER DEALINGS IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#if !defined(TOY_NETWORKING_HPP)
#   define TOY_NETWORKING_HPP "toy-networking-poll.hpp"
#endif

#include "toy-task.hpp"
#include TOY_NETWORKING_HPP
#include <chrono>
#include <iostream>
#include <arpa/inet.h>

// ----------------------------------------------------------------------------

int main()
{
    toy::socket server{ ::socket(PF_INET, SOCK_STREAM, 0) };

    ::sockaddr_in addr{
        .sin_family = AF_INET,
        .sin_port = htons(12345),
        .sin_addr = { .s_addr = INADDR_ANY }
        };
    ::bind(server.fd, (::sockaddr*)&addr, sizeof(addr));
    ::listen(server.fd, 1);

    toy::io_context  io;

    if constexpr (io.has_timer) {
        io.spawn([](auto& io)->toy::task {
            while (true) {
                using namespace std::chrono_literals;
                co_await toy::async_sleep_for{io, 1s};
                std::cout << "waited 1s\n" << std::flush;
            }
            
        }(io));
        io.spawn([](auto& io)->toy::task {
            while (true) {
                using namespace std::chrono_literals;
                co_await toy::async_sleep_for{io, 3s};
                std::cout << "waited 3s\n" << std::flush;
            }
            
        }(io));
    }

    io.spawn([](auto& io, auto& server)->toy::task {
        for (int i{}; i != 2; ++i) {
            auto c = co_await toy::async_accept(io, server);

            io.spawn([](auto& io, auto socket)->toy::task {
                char   buf[4];
                while (std::size_t n = co_await toy::async_read_some{io, socket, buf, sizeof buf}) {
                    for (std::size_t o{}, w(1); o != n && w; o += w) {
                        w = co_await toy::async_write_some(io, socket, buf + o, n - o);
                    }
                }
            }(io, std::move(c)));
        }
    }(io, server));

    io.run();
}
