// toy-ops.cpp                                                        -*-C++-*-
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

#include "toy-buffer.hpp"
#include "toy-networking-poll.hpp"
#include "toy-task.hpp"

// ----------------------------------------------------------------------------

int main() {
    toy::io_context context;
    toy::socket     socket{ ::socket(PF_INET, SOCK_STREAM, 0) };

    context.spawn([](toy::socket& socket)->toy::task<toy::io_context::scheduler>{
        ::sockaddr_in addr{
            .sin_family = AF_INET,
            .sin_port = htons(80),
            .sin_addr = { .s_addr = htonl(0x53f33a19) }
            };

        if (co_await toy::async_connect(socket, reinterpret_cast<::sockaddr const*>(&addr), sizeof addr) < 0) {
            std::cout << "ERROR: failed to connect: " << ::strerror(errno) << "\n";
            co_return;
        }
        char buffer[1024];
        std::size_t ns = co_await toy::async_send(socket, toy::buffer(buffer));
        (void)ns;
        std::size_t nr = co_await toy::async_receive(socket, toy::buffer(buffer));
        (void)nr;
    }(socket));

    //context.run();
}