// toy-networking-kqueue.hpp                                          -*-C++-*-
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

#ifndef INCLUDED_TOY_NETWORKING_KQUEUE
#define INCLUDED_TOY_NETWORKING_KQUEUE

#include "toy-networking-common.hpp"
#include "toy-starter.hpp"
#include "toy-utility.hpp"
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <stddef.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/fcntl.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

namespace toy
{

// ----------------------------------------------------------------------------

struct socket
{
    int fd = -1;
    socket(int fd)
        : fd(fd) {
        if (::fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
            throw std::runtime_error("fcntl");
        }
    }
    socket(socket&& other): fd(std::exchange(other.fd, -1)) {}
    ~socket() { if (fd != -1) ::close(fd); }
};

class io_context;
struct io: immovable {
    io_context& c;
    int         fd;
    short int   events;
    virtual void complete() = 0;
    io(io_context& c, int fd, short int events): c(c), fd(fd), events(events)  {}
};

class io_context: public starter {
    using kevent_t = struct ::kevent;
    static constexpr std::size_t size{4};

    int          queue;
    std::size_t  outstanding{};
    std::size_t  changes{};
    kevent_t     change[size];
    kevent_t     result[size];

public:
    static constexpr bool has_timer = false; //-dk:TODO remove - used while adding timers to contexts
    io_context()
        : queue(::kqueue()) {
        if (queue < 0) {
            throw std::runtime_error("can't create kqueue");
        }
    }
    ~io_context() {
        ::close(queue);
    }
    void add(io* i) {
        if (changes == size) {
            ::kevent(queue, change, changes, result, 0, nullptr);
            changes = 0u;
        }
        change[changes++] = kevent_t{::uintptr_t(i->fd), ::int16_t(i->events), EV_ADD | EV_ONESHOT| EV_EOF, ::uint32_t(), ::intptr_t(), i};
        ++outstanding;
    }
    void run() {
        while (0 < outstanding) {
            int n = ::kevent(queue, change, changes, result, size, nullptr);
            changes = 0u;
            while (0 < n) {
                static_cast<io*>(result[--n].udata)->complete();
                --outstanding;
            }
        }
    }
};

// ----------------------------------------------------------------------------

namespace hidden_io_op {
    template <typename Res, short F, typename O, typename... P>
    struct io_op {
        using result_t = Res;

        io_context&      c;
        int              fd;
        std::tuple<P...> p;

        io_op(auto& c, socket& sock, auto... a): c(c), fd(sock.fd), p(a...) {}

        template <typename R>
        struct state: io {
            std::tuple<P...> p;
            R                r;
            state(auto& c, int fd, auto p, R r): io(c, fd, F), p(p), r(r) {}
            friend void start(state& self) { self.c.add(&self); }
            void complete() override final { O()(*this); }
        };

        template <typename R>
        friend state<R> connect(io_op const& self, R r) {
            return state<R>(self.c, self.fd, self.p, r);
        }
    };
}

using async_accept = hidden_io_op::io_op<socket, EVFILT_READ, decltype([](auto& s){
    ::sockaddr  addr{};
    ::socklen_t len{sizeof(addr)};
    auto fd = ::accept(s.fd, &addr, &len);
    if (0 <= fd) set_value(s.r, fd);
    else set_error(s.r, std::make_exception_ptr(std::runtime_error("accept")));
    })>;

using async_read_some = hidden_io_op::io_op<int, EVFILT_READ, decltype([](auto& s){
    auto n = ::read(s.fd, get<0>(s.p), get<1>(s.p));
    if (0 <= n) set_value(s.r, n);
    else set_error(s.r, std::make_exception_ptr(std::runtime_error("read")));
    }), char*, std::size_t>;

using async_write_some = hidden_io_op::io_op<int, EVFILT_WRITE, decltype([](auto& s){
    auto n = ::write(s.fd, get<0>(s.p), get<1>(s.p));
    if (0 <= n) set_value(s.r, n);
    else set_error(s.r, std::make_exception_ptr(std::runtime_error("write")));
    }), char const*, std::size_t>;

namespace hidden_async_connect {
    struct async_connect {
        using result_t = int;

        io_context&       c;
        int               fd;
        ::sockaddr const* addr;
        ::socklen_t       len;

        async_connect(auto& c, socket& sock, ::sockaddr const* addr, ::socklen_t len): c(c), fd(sock.fd), addr(addr), len(len) {}

        template <typename R>
        struct state: io {
            ::sockaddr const* addr;
            ::socklen_t       len;
            R                 r;
            state(auto& c, int fd, ::sockaddr const* addr, ::socklen_t len, R r): io(c, fd, EVFILT_WRITE), addr(addr), len(len), r(r) {}
            friend void start(state& self) {
                switch (::connect(self.fd, self.addr, self.len)? errno: 0) {
                default:
                    set_error(self.r, std::make_exception_ptr(std::runtime_error(std::string("connect: ") + ::strerror(errno))));
                    break;
                case 0:
                    set_value(self.r, 0);
                    break;
                case EAGAIN:
                    self.c.add(&self);
                    break;
                case EINPROGRESS:
                    self.c.add(&self);
                    break;
                }
            }
            void complete() override final {
                int         rc{};
                ::socklen_t len{sizeof rc};
                if (::getsockopt(fd, SOL_SOCKET, SO_ERROR, &rc, &len))
                    set_error(r, std::make_exception_ptr(std::runtime_error(std::string("getsockopt: ") + ::strerror(errno))));
                else
                    set_value(r, 0);
            }
        };

        template <typename R>
        friend state<R> connect(async_connect const& self, R r) {
            return state<R>(self.c, self.fd, self.addr, self.len, r);
        }
    };
}
using async_connect = hidden_async_connect::async_connect;

// ----------------------------------------------------------------------------

namespace hidden_async_sleep_for {
    struct async_sleep_for {
        using result_t = none;
        struct state {
            friend void start(state&) {}
        };
        friend state connect(async_sleep_for, auto) {
            return {};
        }
    };
}
using async_sleep_for = hidden_async_sleep_for::async_sleep_for;

// ----------------------------------------------------------------------------

}

#endif
