// toy-socket.hpp                                                     -*-C++-*-
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

#ifndef INCLUDED_TOY_SOCKET
#define INCLUDED_TOY_SOCKET

#include "toy-networking-common.hpp"
#ifdef TOY_HAS_SYS_FCNTL
#    include <sys/fcntl.h>
#endif
#ifdef TOY_HAS_SYS_SOCKET
#    include <sys/socket.h>
#endif
#ifdef TOY_HAS_WINSOCK2
#    include <winsock2.h>
#endif
#ifdef TOY_HAS_UNISTD
#    include <unistd.h>
#endif
#include <ostream>
#include <stdexcept>

// ----------------------------------------------------------------------------

namespace toy
{
    struct socket;

#ifdef TOY_HAS_WINSOCK2
    struct file
    {
    };
    toy::file std_in(auto&&) { return toy::file(); }

    using socket_handle = SOCKET;

    inline socket_handle make_socket(int domain, int type, int protocol)
    {
        return WSASocketW(domain, type, protocol, nullptr, 0, WSA_FLAG_OVERLAPPED);
    }
    inline int close(socket_handle) { return 0; }
    inline int fcntl(socket_handle fd, int cmd, int flag) { return 0; }
    inline void set_non_blocking(socket_handle )
    {
    }
#else
    struct file: toy::socket { using toy::socket::socket; };
    using socket_handle = int;

    inline socket_handle make_socket(int domain, int type, int protocol) { return ::socket(domain, type, protocol); }
    inline int close(socket_handle fd) { return ::close(fd); }
    inline int fcntl(socket_handle fd, int cmd, int flag) { return ::fcntl(fd, cmd, flag); }
    inline void set_non_blocking(socket_handle fd)
    {
        if (toy::fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
            throw std::runtime_error("fcntl");
        }
    }
#endif


struct io_context_base {
    io_context_base() = default;
    io_context_base(io_context_base&&) = delete;
    ~io_context_base() = default;
    virtual socket_handle make_socket(int domain, int type, int protocol)
    {
        return toy::make_socket(domain, type, protocol);
    }
    virtual socket_handle make_fd(socket_handle fd) { return fd; }
    virtual socket_handle fd(socket_handle id) { return id; }

    virtual int close(socket_handle fd)   { return toy::close(fd); }
    virtual void set_non_blocking(socket_handle fd)
    {
        toy::set_non_blocking(fd);
    }
};

// ----------------------------------------------------------------------------

struct socket
{
private:
    enum class tag { normal, special };
    io_context_base* d_context;
    socket_handle    d_id;
    tag              d_kind;

    socket(io_context_base& context, socket_handle id, tag kind)
        : d_context(&context)
        , d_id(id)
        , d_kind(kind)
    {
        context.set_non_blocking(fd());
    }
public:
    socket()
        : d_context(nullptr)
        , d_id(-1)
        , d_kind(tag::special)
    {
    }
    socket(io_context_base& context, int domain, int type, int protocol)
        : socket(context, context.make_socket(domain, type, protocol), tag::normal)
    {
    }
    socket(io_context_base& context, socket_handle fd)
        : socket(context, context.make_fd(fd), tag::special)
    {
    }
    socket(socket&& other)
        : d_context(std::exchange(other.d_context, nullptr))
        , d_id(std::exchange(other.d_id, -1))
        , d_kind(std::exchange(other.d_kind, tag::special))
    {
    }
    ~socket()
    {
        if (d_kind != tag::special) {
            d_context->close(d_id);
        }
    }
    socket_handle         fd() const { return d_context->fd(d_id); }
    socket_handle         id() const { return d_id; }
    toy::io_context_base* context() const { return d_context; }

    friend std::ostream& operator<< (std::ostream& out, socket const& s) {
        return out << s.fd();
    }
};

}

// ----------------------------------------------------------------------------

#endif
