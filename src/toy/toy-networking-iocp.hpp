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
#include "toy-networking-sender.hpp"
#include "toy-starter.hpp"
#include "toy-utility.hpp"
#include <chrono>
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

#include <windows.h>
#include <mswsock.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

namespace toy::iocp
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
    }
    winsock_setup(winsock_setup&&) = delete;
    ~winsock_setup() {
        WSACleanup();
    }
};

struct winsock {
    winsock() { static winsock_setup setup; }
};

struct io_base
    : WSAOVERLAPPED
{
    io_base(): WSAOVERLAPPED() {}
    virtual void complete(std::size_t) = 0;
};

// ----------------------------------------------------------------------------

template <typename Stream, typename Receiver, typename Args>
struct io_state
#if 0
    : toy::iocp::io_base
{
    Receiver receiver;
    Stream&  stream;
    Args     args;
    io_state(Receiver receiver, Stream& stream, toy::event_kind event, Args args)
        : receiver(receiver)
        , stream(stream)
        , args(args)
    {
    }
    friend void start(io_state&) {}
    void complete(std::size_t) override {}
}
#endif
;

template <typename Receiver>
struct io_state<toy::socket, Receiver, toy::accept_t::args>
    : toy::iocp::io_base
{
    Receiver                   receiver;
    toy::socket&               socket;
    std::optional<toy::socket> client;
    char                       buffer[2 * (sizeof(sockaddr_storage) + 16)];
    DWORD                      dummy;

    io_state(Receiver receiver, toy::socket& socket, toy::event_kind, toy::accept_t::args)
        : receiver(receiver)
        , socket(socket)
    {
    }
    friend void start(io_state& self) {
        auto scheduler(get_scheduler(self.receiver));
        self.client.emplace(scheduler.base(), PF_INET, SOCK_STREAM, IPPROTO_TCP);
        scheduler.accept(self.socket, *self.client, self.buffer, &self.dummy, &self);
        scheduler.increment();
    }
    void complete(std::size_t) override {
        set_value(this->receiver, std::move(*client));
    }
};

template <typename Receiver>
struct io_state<toy::socket, Receiver, toy::connect_t::args>
    : toy::iocp::io_base
{
    Receiver             receiver;
    toy::socket&         socket;
    toy::connect_t::args args;

    io_state(Receiver receiver, toy::socket& socket, toy::event_kind, toy::connect_t::args args)
        : receiver(receiver)
        , socket(socket)
        , args(args)
    {
    }
    friend void start(io_state& self) {
        auto scheduler(get_scheduler(self.receiver));
        scheduler.connect(self.socket, &self.args.address.as_addr(), self.args.address.size(), &self);
        scheduler.increment();
    }
    void complete(std::size_t) override {
        set_value(this->receiver, 0);
    }
};

template <typename Receiver, typename MBS>
struct io_state<toy::socket, Receiver, toy::receive_t::args<MBS>>
    : toy::iocp::io_base
{
    Receiver                  receiver;
    toy::socket&              socket;
    toy::receive_t::args<MBS> args;

    io_state(Receiver receiver, toy::socket& socket, toy::event_kind, toy::receive_t::args<MBS> args)
        : receiver(receiver)
        , socket(socket)
        , args(args)
    {
    }
    friend void start(io_state& self)
    {
        DWORD bytes{};
        DWORD flags{};
        if (WSARecv(self.socket.fd(), self.args.buffer.data(), DWORD(self.args.buffer.size()), &bytes, &flags, static_cast<toy::iocp::io_base*>(&self), nullptr)) {
            int error = WSAGetLastError();
            if (error != WSA_IO_PENDING) {
                //-dk:TODO deal with error
                return;
            }
        }
        get_scheduler(self.receiver).increment();
    }
    void complete(std::size_t size) override {
        set_value(this->receiver, int(size));
    }
};

template <typename Receiver, typename MBS>
struct io_state<toy::socket, Receiver, toy::send_t::args<MBS>>
    : toy::iocp::io_base
{
    Receiver               receiver;
    toy::socket&           socket;
    toy::send_t::args<MBS> args;

    io_state(Receiver receiver, toy::socket& socket, toy::event_kind, toy::send_t::args<MBS> args)
        : receiver(receiver)
        , socket(socket)
        , args(args)
    {
    }
    friend void start(io_state& self)
    {
        DWORD bytes{};
        DWORD flags{};
        if (WSASend(self.socket.fd(), self.args.buffer.data(), DWORD(self.args.buffer.size()), &bytes, flags, static_cast<toy::iocp::io_base*>(&self), nullptr)) {
            int error = WSAGetLastError();
            if (error != WSA_IO_PENDING) {
                //-dk:TODO deal with error
                return;
            }
        }
        get_scheduler(self.receiver).increment();
    }
    void complete(std::size_t size) override
    {
        set_value(this->receiver, int(size));
    }
};

template <typename Receiver, typename MBS>
struct io_state<toy::socket, Receiver, toy::send_to_t::args<MBS>>
    : toy::iocp::io_base
{
    Receiver               receiver;
    toy::socket&           socket;
    toy::send_to_t::args<MBS> args;

    io_state(Receiver receiver, toy::socket& socket, toy::event_kind, toy::send_to_t::args<MBS> args)
        : receiver(receiver)
        , socket(socket)
        , args(args)
    {
    }
    friend void start(io_state& self)
    {
        set_error(std::move(self.receiver), std::make_exception_ptr(std::runtime_error("IOCP send_to is not, yet, implemented")));
    }
    void complete(std::size_t size) override
    {
        set_value(this->receiver, int(size));
    }
};

template <typename Receiver, typename MBS>
struct io_state<toy::socket, Receiver, toy::receive_from_t::args<MBS>>
    : toy::iocp::io_base
{
    Receiver               receiver;
    toy::socket&           socket;
    toy::receive_from_t::args<MBS> args;

    io_state(Receiver receiver, toy::socket& socket, toy::event_kind, toy::receive_from_t::args<MBS> args)
        : receiver(receiver)
        , socket(socket)
        , args(args)
    {
    }
    friend void start(io_state& self)
    {
        set_error(std::move(self.receiver), std::make_exception_ptr(std::runtime_error("IOCP receive_from is not, yet, implemented")));
    }
    void complete(std::size_t size) override
    {
        set_value(this->receiver, int(size));
    }
};

template <typename Receiver>
struct io_state<toy::socket, Receiver, toy::write_some_t::args>
    : toy::iocp::io_base
{
    Receiver                receiver;
    toy::socket&            socket;
    toy::write_some_t::args args;

    io_state(Receiver receiver, toy::socket& socket, toy::event_kind, toy::write_some_t::args args)
        : receiver(receiver)
        , socket(socket)
        , args(args)
    {
    }
    friend void start(io_state& self)
    {
        set_error(std::move(self.receiver), std::make_exception_ptr(std::runtime_error("IOCP write_some is not, yet, implemented")));
    }
    void complete(std::size_t size) override
    {
        set_value(this->receiver, int(size));
    }
};

template <typename Receiver>
struct io_state<toy::socket, Receiver, toy::read_some_t::args>
    : toy::iocp::io_base
{
    Receiver                receiver;
    toy::socket&            socket;
    toy::read_some_t::args args;

    template <typename R>
    io_state(R&& receiver, toy::socket& socket, toy::event_kind, toy::read_some_t::args args)
        : receiver(std::forward<R>(receiver))
        , socket(socket)
        , args(args)
    {
    }
    friend void start(io_state& self)
    {
        set_error(std::move(self.receiver), std::make_exception_ptr(std::runtime_error("IOCP read_some is not, yet, implemented")));
    }
    void complete(std::size_t size) override
    {
        set_value(this->receiver, int(size));
    }
};

template <typename Receiver>
struct io_state<toy::file, Receiver, toy::read_some_t::args>
    : toy::iocp::io_base
{
    Receiver               receiver;
    toy::file&             file;
    toy::read_some_t::args args;
    template <typename R>
    io_state(R&& receiver, toy::file& file, toy::event_kind event, toy::read_some_t::args args)
        : receiver(std::forward<R>(receiver))
        , file(file)
        , args(args)
    {
    }
    friend void start(io_state&) { /*-dk:TODO*/ }
    void complete(std::size_t) override {}
};

template <typename Receiver>
struct io_state<toy::socket, Receiver, toy::poll_t::args>
    : toy::iocp::io_base
{
    Receiver                receiver;
    toy::socket&            socket;
    toy::poll_t::args args;

    io_state(Receiver receiver, toy::socket& socket, toy::event_kind, toy::poll_t::args args)
        : receiver(receiver)
        , socket(socket)
        , args(args)
    {
    }
    friend void start(io_state& self)
    {
        set_error(std::move(self.receiver), std::make_exception_ptr(std::runtime_error("IOCP poll is not implemented (and it is unclearly how to do it)")));
    }
    void complete(std::size_t) override
    {
        set_value(this->receiver, toy::event_kind::none);
    }
};

// ----------------------------------------------------------------------------

template <typename Receiver, typename Operation>
struct time_state
    : toy::iocp::io_base
{
    Receiver  receiver;
    Operation operation;
    time_state(Receiver receiver, Operation operation): receiver(receiver), operation(operation) {}
    friend void start(time_state&) {}
    void complete(std::size_t) override {}
};

struct context;
struct scheduler {
    template <typename Receiver, typename Args, typename Stream>
    using io_state = toy::iocp::io_state<Stream, Receiver, Args>;
    template <typename Receiver, typename Operation>
    using time_state = toy::iocp::time_state< Receiver, Operation>;

    context* context;
    toy::io_context_base& base();

    void increment();
    void accept(toy::socket& server, toy::socket& client, void* buffer, DWORD* dummy, LPOVERLAPPED overlapped);
    void connect(toy::socket& server, ::sockaddr const* name, ::socklen_t namelen, LPOVERLAPPED overlapped);
};


struct file;
struct socket;

struct context
    : winsock
    , toy::io_context_base
    , starter<toy::iocp::scheduler> {
        using io_base = toy::iocp::io_base;
    static std::error_category const& category() { return std::system_category(); } //-dk:TODO use custom category
    using scheduler = toy::iocp::scheduler;

    struct record
    {
        socket_handle          fd{};
        std::unique_ptr<ULONG> continuation;
    };

    HANDLE              port;
    std::vector<record> sockets;
    socket_handle       next_record{};
    std::size_t         outstanding{};

    bool empty() const { return outstanding == 0u; }
    void increment() { ++outstanding; }
    void decrement() { --outstanding; }
    scheduler get_scheduler() { return { this }; }

    socket_handle make_record()
    {
        if (sockets.size() == next_record)
        {
            sockets.resize(std::max(sockets.size(), std::size_t(16u)));
            for (auto i{next_record}; i!= sockets.size(); ++i)
            {
                sockets[i].fd = i + 1;
            }
        }
        return std::exchange(next_record, sockets[next_record].fd);
    }
    socket_handle make_socket(int domain, int type, int protocol) override
    {
        socket_handle index = make_record();
        record& r(sockets[index]);
        r = record{toy::make_socket(domain, type, protocol), std::unique_ptr<ULONG>(new ULONG())};
        if (r.fd == INVALID_SOCKET) {
            throw std::system_error(WSAGetLastError(), std::system_category(),
                                    "failed to create socket");

        }
        if (CreateIoCompletionPort(reinterpret_cast<HANDLE>(r.fd), port, reinterpret_cast<ULONG_PTR>(r.continuation.get()), 0) != port)
        {
            throw std::system_error(WSAGetLastError(), std::system_category(),
                                    "failed to set iocp");
        }
        return index;
    }
    socket_handle fd(socket_handle index) override { return sockets[index].fd; }
    int close(socket_handle index) override
    {
        closesocket(sockets[index].fd);
        next_record = std::exchange(sockets[index].fd, next_record);
        return 0;
    }

    void run()
    {
        std::size_t total{};
        std::size_t count{};
        while (!empty())
        {
            ++total;
            count += run_one();
        }
    }
    std::size_t run_one()
    {
        DWORD       transferred{};
        ULONG_PTR   key{};
        OVERLAPPED* overlapped{};
        if (FALSE == GetQueuedCompletionStatus(port, &transferred, &key, &overlapped, 6000 /* INFINITE /*-dk:TODO use more sensible time*/)) {
            return 0;
        }
        static_cast<context::io_base*>(overlapped)->complete(transferred);
        decrement();
        return 1;
    }

    void accept(toy::socket& server, toy::socket& client, void* buffer, DWORD* dummy, LPOVERLAPPED overlapped);
    void connect(toy::socket& server, ::sockaddr const* name, ::socklen_t namelen, LPOVERLAPPED overlapped);

    context()
        : starter<toy::iocp::scheduler>(get_scheduler()) 
        , port(CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0))
        , sockets()
    {
        if (!port) {
            throw std::system_error(WSAGetLastError(), this->category(),
                                    "failed to create completion port");
        }
    }
    context(context&&) = delete;
    ~context() { /*-dk:TODO */ }
};

inline void context::accept(toy::socket& server, toy::socket& client, void* buffer, DWORD* word, LPOVERLAPPED overlapped) {
    static LPFN_ACCEPTEX accept_ex{};
    static bool const dummy = std::invoke([&server]{
        DWORD bytes{};
        GUID  guidAcceptEx = WSAID_ACCEPTEX;
        if (SOCKET_ERROR == WSAIoctl(server.fd(),
                                     SIO_GET_EXTENSION_FUNCTION_POINTER,
                                     &guidAcceptEx, sizeof(guidAcceptEx),
                                     &accept_ex, sizeof(accept_ex),
                                     &bytes, nullptr, nullptr)) {
            throw std::runtime_error("failed to get accept_ex");
        }
        return true;
    });

    if (not accept_ex(server.fd(), client.fd(), buffer, 0, sizeof(sockaddr_storage) + 16, sizeof(sockaddr_storage) + 16, word, overlapped)) {
        int err = WSAGetLastError();
        if (err != ERROR_IO_PENDING) {
            throw std::system_error(err, this->category(), "failed to accept_ex");
        }
    }
}

inline void context::connect(toy::socket& client, ::sockaddr const* name, ::socklen_t namelen, LPOVERLAPPED overlapped)
{
    static LPFN_CONNECTEX connect_ex{};
    static bool const dummy = std::invoke([&client]{
        DWORD bytes{};
        GUID  guidAcceptEx = WSAID_CONNECTEX;
        if (SOCKET_ERROR == WSAIoctl(client.fd(),
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
    if (bind(client.fd(), reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr))) {
        throw std::system_error(WSAGetLastError(), this->category(), "failed to bind before connect_ex");
    }

    if (not connect_ex(client.fd(), name, namelen, nullptr, 0, nullptr, overlapped)) {
        int err = WSAGetLastError();
        if (err != ERROR_IO_PENDING) {
            throw std::system_error(err, this->category(), "failed to connect_ex");
        }
    }
}

inline toy::io_context_base& scheduler::base() { return *this->context; }
inline void scheduler::increment() { context->increment(); }
inline void scheduler::accept(toy::socket& server, toy::socket& client, void* buffer, DWORD* dummy, LPOVERLAPPED overlapped)
{
    context->accept(server, client, buffer, dummy, overlapped);
}
inline void scheduler::connect(toy::socket& server, ::sockaddr const* name, ::socklen_t namelen, LPOVERLAPPED overlapped)
{
    context->connect(server, name, namelen, overlapped);
}

// ----------------------------------------------------------------------------

struct file {
    struct data {};
    HANDLE handle;
    std::unique_ptr<data> continuation;
    file(HANDLE handle)
        : handle(handle)
        , continuation(new data)
    {
    }
};

// ----------------------------------------------------------------------------

}

#endif
