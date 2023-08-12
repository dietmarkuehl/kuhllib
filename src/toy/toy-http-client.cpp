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

#include "toy-networking.hpp"
#include "toy-task.hpp"
#include <iostream>
#include <string.h>

// ----------------------------------------------------------------------------

int main() {
    toy::io_context context;
    
    context.spawn([&context]()->toy::task<toy::io_context::scheduler> {
        toy::socket client(context, PF_INET, SOCK_STREAM, 0);

        if (co_await toy::async_connect(client, toy::address(AF_INET, htons(80), htonl(0x53f33a19))) < 0) {
            std::cout << "ERROR: failed to connect: " << ::strerror(errno) << "\n";
            co_return;
        }
        std::cout << "connected\n";
        char const request[] = {
            "GET /index.html HTTP/1.0\r\n"
            "Host: dietmar-kuehl.de\r\n"
            "\r\n"
        };
        auto const size = ::strlen(request);
        if (co_await toy::async_send(client, toy::buffer(request, size)) < 1u) {
            std::cout << "ERROR: failed to write request: " << toy::strerror(errno) << "\n";
            co_return;
        }
        std::cout << "wrote request\n";
        char buffer[65536];
        auto result = co_await toy::async_receive(client, toy::buffer(buffer));
        if (result < 0) {
            std::cout << "ERROR: failed to read response: " << toy::strerror(errno) << "\n";
            co_return;
        }
        std::cout << "received result\n";
        std::cout.write(buffer, result);
    }());

    context.run();
}
