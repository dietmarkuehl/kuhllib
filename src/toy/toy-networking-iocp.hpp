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

#include "toy-networking-common.hpp"
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

#include <mswsock.h>
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

struct io_context;
struct io_scheduler {
    io_context* context;
};

struct socket;

struct io_context
    : winsock
    , starter<io_scheduler> {
    static constexpr bool has_timer = false; //-dk:TODO remove - used while adding timers to contexts
    using scheduler = io_scheduler;
    scheduler get_scheduler() { return { this }; }
    void run() { /*-dk:TODO */}

    void accept(toy::socket& socket);

    HANDLE port;
    io_context()
        : port(CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0))
        , starter<io_scheduler>(get_scheduler()) {
        if (!port) {
            throw std::system_error(WSAGetLastError(),
                                    std::system_category(), //-dk:TODO use custom category
                                    "failed to create completion port");
        }
    }
    io_context(io_context&&) = delete;
    ~io_context() { /*-dk:TODO */ }
};

// ----------------------------------------------------------------------------

struct socket {
    struct data {};
    SOCKET fd;
    std::unique_ptr<data> continuation;
    socket(toy::io_context& io, int domain, int type, int protocol)
        : fd(WSASocketW(domain, type, protocol, nullptr, 0, WSA_FLAG_OVERLAPPED))
        , continuation(new data()) {
        if (fd == INVALID_SOCKET) {
            throw std::system_error(WSAGetLastError(),
                                    std::system_category(), //-dk:TODO use custom category
                                    "failed to create socket");

        }
        if (CreateIoCompletionPort(reinterpret_cast<HANDLE>(fd), io.port, reinterpret_cast<ULONG_PTR>(continuation.get()), 0) != io.port) {
            throw std::system_error(WSAGetLastError(),
                                    std::system_category(), //-dk:TODO use custom category
                                    "failed to set iocp");
        }
    }
    socket(socket&& other)
        : fd(std::exchange(other.fd, INVALID_SOCKET))
        , continuation(std::move(other.continuation)) {}
    ~socket() {
        if (fd != INVALID_SOCKET) {
            closesocket(fd);
        }
    }
};

void io_context::accept(toy::socket& socket) {
        static LPFN_ACCEPTEX accept_ex{};
        static bool const dummy = [&socket]{
            DWORD bytes{};
            GUID  guidAcceptEx = WSAID_ACCEPTEX;
            if (SOCKET_ERROR == WSAIoctl(socket.fd,
                                         SIO_GET_EXTENSION_FUNCTION_POINTER,
                                         &guidAcceptEx, sizeof(guidAcceptEx),
                                         &accept_ex, sizeof(accept_ex),
                                         &bytes, nullptr, nullptr)) {
                throw std::runtime_error("failed to get accept_ex");
            }
            std::cout << "successfully got accept_ex\n";
            return true;
        }();
        std::cout << "accept done\n";
    }
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

struct async_accept {
    using result_t = toy::socket;

    template <typename Receiver>
    struct state {
        Receiver      receiver;
        toy::socket&  socket;
        WSAOVERLAPPED overlapped{};
        friend void start(state& self) {
            toy::io_context& context(*get_scheduler(self.receiver).context);
            (void)context;
        }
    };
    toy::socket& socket;
    async_accept(toy::socket& socket): socket(socket) {}
    template <typename Receiver>
    friend state<Receiver> connect(async_accept const& self, Receiver receiver) {
        return { receiver, self.socket };
    }
};

// ----------------------------------------------------------------------------

struct async_receive {
    using result_t = int;
    struct state {
        friend void start(state&) {}
    };
    async_receive(toy::socket&, auto&&) {}
    friend state connect(async_receive, auto) {
        return {};
    }
};

// ----------------------------------------------------------------------------

struct async_send {
    using result_t = int;
    struct state {
        friend void start(state&) {}
    };
    async_send(toy::socket&, auto&&) {}
    friend state connect(async_send, auto) {
        return {};
    }
};

// ----------------------------------------------------------------------------

struct async_write_some {
    using result_t = int;
    struct state {
        friend void start(state&) {}
    };
    async_write_some(toy::socket&, char const*, std::size_t) {}
    friend state connect(async_write_some, auto) {
        return {};
    }
};

// ----------------------------------------------------------------------------

}


#endif
