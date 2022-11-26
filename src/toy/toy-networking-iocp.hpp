// toy-networking-iocp.hpp                                          -*-C++-*-
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

#ifndef INCLUDED_TOY_NETWORKING_IOCP
#define INCLUDED_TOY_NETWORKING_IOCP

#include "toy-starter.hpp"
#include "toy-utility.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <stddef.h>
#include <string.h>

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

namespace toy
{

// ----------------------------------------------------------------------------

struct winsock_setup {
    WSADATA wsaData;
    winsock_setup() {
        if (int error = WSAStartup(MAKEWORD(2, 2), &wsaData)) {
            throw std::runtime_error("failed to initialize winsock`");
        }
        std::cout << "winsock setup\n" << std::flush;
    }
    winsock_setup(winsock_setup&&) = delete;
    ~winsock_setup() {
        WSACleanup();
        std::cout << "winsock torn down\n" << std::flush;
    }
};

struct winsock {
    winsock() { static winsock_setup setup; }
};

struct io_context
    : winsock {
    static constexpr bool has_timer = false; //-dk:TODO remove - used while adding timers to contexts
    HANDLE port;
    io_context()
        : port(CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0)) {
        if (!port) {
            throw std::system_error(WSAGetLastError(),
                                    std::system_category(), //-dk:TODO use custom category
                                    "failed to create completion port");
        }
    }
    io_context(io_context&&) = delete;
};

// ----------------------------------------------------------------------------

struct acceptor {
    SOCKET fd;
    acceptor()
        : fd(WSASocketW(AF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED)) {
        if (fd == INVALID_SOCKET) {
            throw std::system_error(WSAGetLastError(),
                                    std::system_category(), //-dk:TODO use custom category
                                    "failed to create socket");

        }
    }
    acceptor(acceptor&& other): fd(std::exchange(other.fd, INVALID_SOCKET)) {}
    ~acceptor() {
        if (fd != INVALID_SOCKET) {
            closesocket(fd);
        }
    }
};

// ----------------------------------------------------------------------------

struct async_sleep_for {
    using result_t = none;
    struct state {
        friend void start(state&) {}
    };
    friend state connect(async_sleep_for, auto) {
        return {};
    }
};

// ----------------------------------------------------------------------------

}


#endif
