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
                toy::ring_buffer<8> ring;
                int prod_n = 1;
                int cons_n = 1;
                co_await toy::when_any(
                    toy::repeat_effect_until(
                        toy::let_value(ring.produce(), [&client, &prod_n](auto&& buffer){
                            return toy::then(toy::async_receive(client, toy::buffer(buffer.buffer)),
                                [buffer, &prod_n](int r) mutable {
                                    std::cout << "received(" << r << ")='" << std::string_view(buffer.buffer.data(), r) << "'\n";
                                    buffer.advance(r);
                                    prod_n = r;
                                });
                        }),
                    [&prod_n]{ return prod_n == 0; })
                    | toy::then([](auto&&){std::cout << "producer done\n"; })
                    , toy::repeat_effect_until(
                        toy::let_value(ring.consume(), [&client, &cons_n](auto&& buffer){
                            return toy::then(toy::async_send(client, toy::buffer(buffer.buffer)),
                                [buffer, &cons_n](int r) mutable {
                                    std::cout << "sent(" << r << ")='" << std::string_view(buffer.buffer.data(), r) << "'\n";
                                    buffer.advance(r);
                                    cons_n = r;
                                });
                        }),
                    [&cons_n]{ return cons_n == 0; })
                    | toy::then([](auto&&){std::cout << "consumer done\n"; })
                );
                std::cout << "client done\n";
            }, std::move(client)));
        }
        std::cout << "exiting server\n";
    }, io, server));

    io.run();
}