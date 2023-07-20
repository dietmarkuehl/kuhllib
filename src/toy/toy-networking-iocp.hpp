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
#include "toy-networking-winsock2.hpp"
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

std::string wsa_to_string(int error) {
    WCHAR source[] = L"%1";
    static constexpr int size{1024};
    char buffer[size];
    DWORD_PTR args[] = { (DWORD_PTR)0 };
    if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       nullptr, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&buffer, size, (va_list*)args)) {
        return "failed to format message";
    }
    return std::string(buffer);
}

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
    struct io_base
        : WSAOVERLAPPED {
        io_base(): WSAOVERLAPPED() {}
        virtual void complete(std::size_t) = 0;
    };
    static constexpr bool has_timer = false; //-dk:TODO remove - used while adding timers to contexts
    static std::error_category const& category() { return std::system_category(); } //-dk:TODO use custom category
    using scheduler = io_scheduler;
    scheduler get_scheduler() { return { this }; }
    void run() {
        std::size_t total{};
        std::size_t count{};
        while (true /*-dk:TODO !empty()*/) {
            ++total;
            count += run_one();
            std::cout << "run(): " << count << "/" << total << "\n" << std::flush;
        }
    }
    std::size_t run_one() {
        std::cout << "run_one() start\n" << std::flush;
        DWORD       transferred{};
        ULONG_PTR   key{};
        OVERLAPPED* overlapped{};
        if (FALSE == GetQueuedCompletionStatus(port, &transferred, &key, &overlapped, 60000 /* INFINITE /*-dk:TODO use more sensible time*/)) {
            std::cout << "run_one(): failure(" << GetLastError() << "): " << toy::wsa_to_string(GetLastError()) << "\n" << std::flush;
            return 0;
        }
        std::cout << "run_one() success\n";
        static_cast<io_context::io_base*>(overlapped)->complete(transferred);
        return 1;
    }

    void accept(toy::socket& server, toy::socket& client, void* buffer, DWORD* dummy, LPOVERLAPPED overlapped);
    void connect(toy::socket& server, ::sockaddr* name, ::socklen_t namelen, LPOVERLAPPED overlapped);

    HANDLE port;
    io_context()
        : port(CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0))
        , starter<io_scheduler>(get_scheduler()) {
        if (!port) {
            throw std::system_error(WSAGetLastError(), this->category(),
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
            throw std::system_error(WSAGetLastError(), io.category(),
                                    "failed to create socket");

        }
        if (CreateIoCompletionPort(reinterpret_cast<HANDLE>(fd), io.port, reinterpret_cast<ULONG_PTR>(continuation.get()), 0) != io.port) {
            throw std::system_error(WSAGetLastError(), io.category(),
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

void io_context::accept(toy::socket& server, toy::socket& client, void* buffer, DWORD* word, LPOVERLAPPED overlapped) {
    static LPFN_ACCEPTEX accept_ex{};
    static bool const dummy = std::invoke([&server]{
        DWORD bytes{};
        GUID  guidAcceptEx = WSAID_ACCEPTEX;
        if (SOCKET_ERROR == WSAIoctl(server.fd,
                                     SIO_GET_EXTENSION_FUNCTION_POINTER,
                                     &guidAcceptEx, sizeof(guidAcceptEx),
                                     &accept_ex, sizeof(accept_ex),
                                     &bytes, nullptr, nullptr)) {
            throw std::runtime_error("failed to get accept_ex");
        }
        return true;
    });

    if (not accept_ex(server.fd, client.fd, buffer, 0, sizeof(sockaddr_storage) + 16, sizeof(sockaddr_storage) + 16, word, overlapped)) {
        int err = WSAGetLastError();
        if (err != ERROR_IO_PENDING) {
            std::cout << "accept error: err=" << err << "(" << wsa_to_string(err) << ")\n";
            throw std::system_error(err, this->category(), "failed to accept_ex");
        }
    }
    std::cout << "accept done\n";
}

void io_context::connect(toy::socket& client, ::sockaddr* name, ::socklen_t namelen, LPOVERLAPPED overlapped)
{
    static LPFN_CONNECTEX connect_ex{};
    static bool const dummy = std::invoke([&client]{
        DWORD bytes{};
        GUID  guidAcceptEx = WSAID_CONNECTEX;
        if (SOCKET_ERROR == WSAIoctl(client.fd,
                                     SIO_GET_EXTENSION_FUNCTION_POINTER,
                                     &guidAcceptEx, sizeof(guidAcceptEx),
                                     &connect_ex, sizeof(connect_ex),
                                     &bytes, nullptr, nullptr)) {
            throw std::runtime_error("failed to get connect_ex");
        }
        return true;
    });

    ::sockaddr_in addr{
        .sin_family = AF_INET,
        .sin_port = htons(0)
        };
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(client.fd, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr))) {
        throw std::system_error(WSAGetLastError(), this->category(), "failed to bind before connect_ex");
    }

    if (not connect_ex(client.fd, name, namelen, nullptr, 0, nullptr, overlapped)) {
        int err = WSAGetLastError();
        if (err != ERROR_IO_PENDING) {
            std::cout << "connect error: err=" << err << "(" << wsa_to_string(err) << ")\n";
            throw std::system_error(err, this->category(), "failed to connect_ex");
        }
    }
    std::cout << "connect done\n";
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
    struct state
        : io_context::io_base
    {
        Receiver      receiver;
        toy::socket&  socket;
        std::optional<toy::socket> client;
        char buffer[2 * (sizeof(sockaddr_storage) + 16)];
        DWORD dummy;

        state(Receiver receiver, toy::socket& socket)
            : receiver(receiver)
            , socket(socket)
        {
        }
        friend void start(state& self) {
            toy::io_context& context(*get_scheduler(self.receiver).context);
            self.client.emplace(context, PF_INET, SOCK_STREAM, IPPROTO_TCP);
            context.accept(self.socket, *self.client, self.buffer, &self.dummy, &self);
        }
        void complete(std::size_t) override {
            std::cout << "async_accept completed\n";
            set_value(this->receiver, std::move(*client));
        }
    };
    toy::socket& socket;
    async_accept(toy::socket& socket): socket(socket) {}
    template <typename Receiver>
    friend state<Receiver> connect(async_accept const& self, Receiver receiver) {
        return state<Receiver>(receiver, self.socket);
    }
};

// ----------------------------------------------------------------------------

struct async_connect {
    using result_t = int;

    template <typename Receiver>
    struct state
        : io_context::io_base
    {
        Receiver      receiver;
        toy::socket&  socket;
        ::sockaddr*   name;
        ::socklen_t   namelen;

        state(Receiver receiver, toy::socket& socket, ::sockaddr* name, ::socklen_t namelen)
            : receiver(receiver)
            , socket(socket)
            , name(name)
            , namelen(namelen)
        {
        }
        friend void start(state& self) {
            toy::io_context& context(*get_scheduler(self.receiver).context);
            context.connect(self.socket, self.name, self.namelen, &self);
        }
        void complete(std::size_t) override {
            std::cout << "async_connect completed\n";
            set_value(this->receiver, 0);
        }
    };
    toy::socket& socket;
    ::sockaddr*   name;
    ::socklen_t  namelen;
    async_connect(toy::socket& socket, void* name, ::socklen_t namelen): socket(socket), name(reinterpret_cast<::sockaddr*>(name)), namelen(namelen) {}
    template <typename Receiver>
    friend state<Receiver> connect(async_connect const& self, Receiver receiver) {
        return state<Receiver>(receiver, self.socket, self.name, self.namelen);
    }
};

// ----------------------------------------------------------------------------

template <typename Buffer>
struct async_receive {
    using result_t = int;
    template <typename Receiver>
    struct state
        : io_context::io_base
    {
        Receiver     receiver;
        toy::socket& socket;
        Buffer       buffer;
        state(Receiver receiver, toy::socket& socket, Buffer buffer)
            : receiver(receiver)
            , socket(socket)
            , buffer(buffer)
        {
        }
        friend void start(state& self) {
            std::cout << "starting receive\n" << std::flush;
            DWORD bytes{};
            DWORD flags{};
            if (WSARecv(self.socket.fd, self.buffer.data(), DWORD(self.buffer.size()), &bytes, &flags, &self, nullptr)) {
                int error = WSAGetLastError();
                if (error != WSA_IO_PENDING) {
                    //-dk:TODO deal with error
                }
                else {
                    std::cout << "receive started\n" << "\n";
                }
            }
            else {
                std::cout << "receive immediately completed\n" << std::flush;
            }
        }
        void complete(std::size_t size) override {
            std::cout << "async_receive done: " << size << "\n" << std::flush;
            set_value(this->receiver, int(size));
        }
    };
    toy::socket& socket;
    Buffer       buffer;
    async_receive(toy::socket& socket, Buffer buffer): socket(socket), buffer(buffer) {}
    template <typename Receiver>
    friend state<Receiver> connect(async_receive const& self, Receiver receiver) {
        return { receiver, self.socket, self.buffer };
    }
};

// ----------------------------------------------------------------------------

template <typename Buffer>
struct async_send {
    using result_t = int;
    template <typename Receiver>
    struct state
        : io_context::io_base
    {
        Receiver     receiver;
        toy::socket& socket;
        Buffer       buffer;
        state(Receiver receiver, toy::socket& socket, Buffer buffer)
            : receiver(receiver)
            , socket(socket)
            , buffer(buffer)
        {
        }
        friend void start(state& self) {
            std::cout << "starting send\n" << std::flush;
            DWORD bytes{};
            DWORD flags{};
            if (WSASend(self.socket.fd, self.buffer.data(), DWORD(self.buffer.size()), &bytes, flags, &self, nullptr)) {
                int error = WSAGetLastError();
                if (error != WSA_IO_PENDING) {
                    //-dk:TODO deal with error
                }
                else {
                    std::cout << "send started\n" << "\n";
                }
            }
            else {
                std::cout << "send immediately completed\n" << std::flush;
            }
        }
        void complete(std::size_t size) override {
            std::cout << "async_send done: " << size << "\n" << std::flush;
            set_value(this->receiver, int(size));
        }
    };
    toy::socket& socket;
    Buffer       buffer;
    async_send(toy::socket& socket, Buffer buffer)
        : socket(socket)
        , buffer(buffer)
    {
    }
    template <typename Receiver>
    friend state<Receiver> connect(async_send const& self, Receiver receiver) {
        return state<Receiver>(receiver, self.socket, self.buffer);
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
