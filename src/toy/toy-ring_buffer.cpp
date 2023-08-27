// toy-ring_buffer.cpp                                                -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2023 Dietmar Kuehl http://www.dietmar-kuehl.de
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

#include "toy-ring_buffer.hpp"
#include "toy-networking-sender.hpp"
#include "toy-sender.hpp"
#include "toy-networking.hpp"
#include "toy-task.hpp"
#include <iostream>
#include <functional>

// ----------------------------------------------------------------------------

int main()
{
    std::cout << std::unitbuf;
    toy::poll::context io;

    unsigned short port(12345);
    toy::socket server(io, PF_INET, SOCK_STREAM, IPPROTO_TCP);
    toy::address addr(AF_INET, htons(port), INADDR_ANY);
    std::cout << "listening on port " << port << "\n";
    if (::bind(server.fd(), &addr.as_addr(), int(addr.size())) < 0
        || ::listen(server.fd(), 1) < 0) {
            throw std::runtime_error("can't bind socket");
    };

    io.spawn(std::invoke([](auto& io, toy::socket& server)->toy::task<toy::poll::context::scheduler>{
        while (true)
        {
            toy::socket client = co_await toy::async_accept(server);
            std::cout << "accepted a client\n";
            io.spawn(std::invoke([](toy::socket client)->toy::task<toy::poll::context::scheduler>{
                toy::ring_buffer<16> ring;
                co_await toy::when_all(
                    std::invoke([](toy::socket& client, auto& ring)->toy::task<toy::poll::context::scheduler>{
                        for (std::string_view data; 0u < (data = co_await ring.produce(toy::async_receive(client))).size(); )
                        {
                            std::cout << "received=" << data << "\n";
                        }
                    }, client, ring),
                    std::invoke([](toy::socket& client, auto& ring)->toy::task<toy::poll::context::scheduler>{
                        for (std::string_view data; 0u < (data = co_await ring.consume(toy::async_send(client))).size(); )
                        {
                            std::cout << "sent=" << data << "\n";
                        }
                    }, client, ring)
                );
            }, std::move(client)));
        }
        std::cout << "exiting server\n";
    }, io, server));

    io.run();
}