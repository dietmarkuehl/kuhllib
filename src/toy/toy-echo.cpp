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

#include "toy-task.hpp"
#include "toy-networking.hpp"
#include "toy-sender.hpp"
#include <chrono>
#include <functional>
#include <iostream>
#include <cstring>
#include <cstdlib>
#ifdef TOY_HAS_ARPA_INET
#include <arpa/inet.h>
#endif

// ----------------------------------------------------------------------------

int main()
{
    std::cout << std::unitbuf;
    try
    {
    //toy::io_context  io;
    toy::io_context  io("../../ssl/server_cert.pem", "../../ssl/server_keypair.pem");

    unsigned short port(12345);
    toy::socket server(io, PF_INET, SOCK_STREAM, IPPROTO_TCP);
    toy::address addr(AF_INET, htons(port), INADDR_ANY);
    std::cout << "listening on port " << port << "\n";
    if (::bind(server.fd(), &addr.as_addr(), int(addr.size())) < 0
        || ::listen(server.fd(), 1) < 0) {
        std::cout << "can't bind socket: " << toy::strerror(errno) << "\n";
        return EXIT_FAILURE;
    };

    io.spawn([](auto& io, auto& server)->toy::task<toy::io_context::scheduler> {
        for (int i{}; i != 3; ++i) {
            try {
                std::cout << "server.id()=" << server.id() << "\n";
                auto c = co_await toy::async_accept(server);
                std::cout << "accepted a client\n";

                io.spawn(std::invoke([](auto socket)->toy::task<toy::io_context::scheduler> {
                    char   buf[4];
                    while (int n = co_await toy::async_receive(socket, toy::buffer(buf))) {
                        if (n <= 0
                            || co_await toy::async_send(socket, toy::buffer(buf, n)) <= 0) {
                            co_return;
                        }
                    }
                    std::cout << "client done\n";
                }, std::move(c)));
            }
            catch (std::exception const& ex) {
                std::cout << "ERROR: " << ex.what() << "\n";
            }
        }
    }(io, server));

    io.run();
    std::cout << "done\n";
    }
    catch (std::exception const& ex) {
        std::cout << "ERROR: " << ex.what() << "\n";
    }
}
