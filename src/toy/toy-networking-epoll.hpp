// toy-networking-epoll.hpp                                           -*-C++-*-
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

#ifndef INCLUDED_TOY_NETWORKING_EPOLL
#define INCLUDED_TOY_NETWORKING_EPOLL

// ----------------------------------------------------------------------------

#include "toy-starter.hpp"
#include "toy-utility.hpp"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#include <stddef.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>

namespace toy {

// ----------------------------------------------------------------------------

class io_context;
struct io_scheduler {
    io_context* context;
};

struct io {
    int         fd;
    io(int fd): fd(fd) {}
    io(io&& other): fd(std::exchange(other.fd, -1)) {}
    ~io() = default;
    virtual int complete(short int events) = 0;
};

class io_context
    : public starter<toy::io_scheduler> {
    static constexpr std::size_t size{4};

    int          poll;
    std::size_t  outstanding{};

public:
    static constexpr bool has_timer = false; //-dk:TODO remove - used while adding timers to contexts

    using scheduler = toy::io_scheduler;
    scheduler get_scheduler() { return { this }; }

    io_context()
        : starter(get_scheduler())
        , poll(::epoll_create(size)) {
        if (poll < 0) {
            throw std::runtime_error("can't create epoll");
        }
    }
    ~io_context() {
        close(poll);
    }
    void await() { ++outstanding; }
    void modify(io* i, auto op, std::uint32_t events) {
        ::epoll_event ev{events, { static_cast<void*>(i) } };
        if (::epoll_ctl(poll, op, i->fd, &ev) < 0) {
            throw std::runtime_error("epoll_ctl");
        }
    }
    void remove(io* i) {
        close(i->fd);
    }
    void run() {
        ::epoll_event ev[size];
        while (0 < outstanding) {
            int n = ::epoll_wait(poll, ev, size, -1);
            while (0 < n) {
                --n;
                outstanding -= static_cast<io*>(ev[n].data.ptr)->complete(ev[n].events);
            }
        }
    }
};

// ----------------------------------------------------------------------------

struct socket final
    : io
{
    enum class kind { input = 0, output = 1 };
    int events{0};
    io* op[2]{ nullptr, nullptr };
    socket(int fd): io(fd) {
        if (::fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
            throw std::runtime_error("fcntl");
        }
    }
    socket(socket&&) = default;
    ~socket() { if (fd != -1) ::close(fd); }

    void add(io_context& context, io* i, std::uint32_t events) {
        if ((events & this->events) != events) {
            auto op{this->events? EPOLL_CTL_MOD: EPOLL_CTL_ADD};
            this->events |= events;
            context.modify(this, op, this->events);
        }
        if (events & EPOLLIN) {
            assert(nullptr == op[int(kind::input)]);
            op[int(kind::input)] = i;
        }
        if (events & EPOLLOUT) {
            assert(nullptr == op[int(kind::output)]);
            op[int(kind::output)] = i;
        }
    }
    int complete(short int events) override {
        int rc{};
        if (events & EPOLLIN && op[int(kind::input)]) {
            std::exchange(op[int(kind::input)], nullptr)->complete(events);
            ++rc;
        }
        if (events & EPOLLOUT && op[int(kind::output)]) {
            std::exchange(op[int(kind::output)], nullptr)->complete(events);
            ++rc;
        }
        return rc;
    }
};


namespace hidden_io_op {
    template <typename Res, short Flags, typename Op, typename... P>
    struct io_op {
        using result_t = Res;

        socket&          sock;
        std::tuple<P...> p;

        io_op(socket& sock, auto... a): sock(sock), p(a...) {}

        template <typename Receiver>
        struct state: io {
            io_context&      context;
            socket&          sock;
            std::tuple<P...> p;
            Receiver         r;
            Op               op;
            bool             connected{false};

            state(socket& sock, auto p, Receiver r): io(sock.fd), context(*get_scheduler(r).context), sock(sock), p(p), r(r) {}
            state(state&&) = delete;
            friend void start(state& self) {
                if (!self.op(self)) {
                    self.sock.add(self.context, &self, Flags);
                    self.context.await();
                }
            }
            int complete(short int) override final { this->op(*this); return 0; }
        };

        template <typename R>
        friend state<R> connect(io_op const& self, R r) {
            return state<R>(self.sock, self.p, r);
        }
    };
}

using async_accept = hidden_io_op::io_op<socket, EPOLLIN, decltype([](auto& s){
    ::sockaddr  addr{};
    ::socklen_t len{sizeof(addr)};
    auto        fd = ::accept(s.fd, &addr, &len);

    if (0 <= fd) {
        set_value(s.r, fd);
    }
    else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        return false;
    }
    else {
        set_error(s.r, std::make_exception_ptr(std::runtime_error(std::string("accept: ") + strerror(errno))));
    }
    return true;
    })>;

using async_connect = hidden_io_op::io_op<int, EPOLLOUT, decltype([](auto& s){
    if (s.connected) {
        int         rc{};
        ::socklen_t len{sizeof rc};
        if (::getsockopt(s.fd, SOL_SOCKET, SO_ERROR, &rc, &len)) {
            set_error(s.r, std::make_exception_ptr(std::runtime_error(std::string("getsockopt: ") + strerror(errno))));
        }
        else if (rc) {
            set_error(s.r, std::make_exception_ptr(std::runtime_error(std::string("connect: ") + strerror(rc))));
        }
        else {
            set_value(s.r, rc);
        }
        return true;
    }

    s.connected = true;
    auto rc = ::connect(s.fd, std::get<0>(s.p), std::get<1>(s.p));
    if (0 <= rc) {
        set_value(s.r, rc);
    }
    else if (errno == EAGAIN || errno == EINPROGRESS) {
        return false;
    }
    else {
        set_error(s.r, std::make_exception_ptr(std::runtime_error(std::string("connect: ") + strerror(errno))));
    }
    return true;
    }), ::sockaddr const*, ::socklen_t>;

using async_read_some = hidden_io_op::io_op<int, EPOLLIN, decltype([](auto& s){
    auto n = ::read(s.fd, get<0>(s.p), get<1>(s.p));
    if (0 <= n) {
        set_value(s.r, result_t(n));
    }
    else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        return false;
    }
    else {
        set_error(s.r, std::make_exception_ptr(std::runtime_error(std::string("read: ") + strerror(errno))));
    }
    return true;
    }), char*, size_t>;

using async_write_some = hidden_io_op::io_op<int, EPOLLOUT, decltype([](auto& s){
    auto n = ::write(s.fd, get<0>(s.p), get<1>(s.p));
    if (0 <= n) {
        set_value(s.r, result_t(n));
    }
    else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        return false;
    }
    else {
        set_error(s.r, std::make_exception_ptr(std::runtime_error(std::string("write: ") + strerror(errno))));
    }
    return true;
    }), char const*, size_t>;

// ----------------------------------------------------------------------------

namespace hidden_async_sleep_for {
    struct async_sleep_for {
        using result_t = none;

        std::chrono::milliseconds duration;
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
