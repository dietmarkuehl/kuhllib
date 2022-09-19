// toy-client.cpp                                                     -*-C++-*-
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
#include <iostream>
#include <string.h>

// ----------------------------------------------------------------------------

int main() {
    toy::io_context context;
    
    context.spawn([](toy::io_context& context)->toy::task {
        toy::socket client{ ::socket(PF_INET, SOCK_STREAM, 0) };

        ::sockaddr_in addr{
            .sin_family = AF_INET,
            .sin_port = htons(80),
            .sin_addr = { .s_addr = htonl(0x53f33a19) }
            };

        if (co_await toy::async_connect(context, client, reinterpret_cast<::sockaddr const*>(&addr), sizeof addr) < 0) {
            std::cout << "ERROR: failed to connect: " << ::strerror(errno) << "\n";
            co_return;
        }
        char const request[] = {
            "GET /index.html HTTP/1.0\r\n"
            "Host: dietmar-kuehl.de\r\n"
            "\r\n"
        };
        int const size = ::strlen(request);
        if (co_await toy::async_writesome(context, client, request, size) < 0) {
            std::cout << "ERROR: failed to write request: " << ::strerror(errno) << "\n";
            co_return;
        }
        char buffer[65536];
        int result = co_await toy::async_readsome(context, client, buffer, sizeof buffer);
        if (result < 0) {
            std::cout << "ERROR: failed to read response: " << ::strerror(errno) << "\n";
            co_return;
        }
        std::cout.write(buffer, result);
    }(context));

    context.run();
}
