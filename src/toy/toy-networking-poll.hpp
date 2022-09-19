// toy-networking-poll.hpp                                            -*-C++-*-
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

#ifndef INCLUDED_TOY_NETWORKING_POLL
#define INCLUDED_TOY_NETWORKING_POLL

#include "toy-starter.hpp"
#include "toy-utility.hpp"

#include <chrono>
#include <queue>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <stddef.h>
#include <string.h>

#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

namespace toy
{

// ----------------------------------------------------------------------------

struct socket
{
    int fd = -1;
    socket(int fd): fd(fd) {
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
    using time_point_t = std::chrono::system_clock::time_point;
    using timer_t      = std::pair<time_point_t, io*>;
    struct compare_t { bool operator()(auto&& a, auto&& b) { return a.first > b.first; } };
    using queue_t      = std::priority_queue<timer_t, std::vector<timer_t>, compare_t>;
    
    std::vector<io*>      ios;
    std::vector<::pollfd> fds;
    queue_t               times;

public:
    static constexpr bool has_timer = true; //-dk:TODO remove - used while adding timers to contexts

    void add(io* i) {
        ios.push_back(i);
        fds.push_back(  pollfd{ .fd = i->fd, .events = i->events });
    }
    void add(time_point_t time, io* op) {
        times.emplace(time, op);
    }
    void run() {
        while (!ios.empty() || not times.empty()) { 
            auto now{std::chrono::system_clock::now()};

            while (!times.empty() && times.top().first < now) {
                io* op{times.top().second};
                times.pop();
                op->complete();
            }
            auto time{times.empty()
                     ? -1
                : std::chrono::duration_cast<std::chrono::milliseconds>(times.top().first - now).count()};
            if (0 < ::poll(fds.data(), fds.size(), time)) {
                for (size_t i = fds.size(); i--; )
                    if (fds[i].events & fds[i].revents) {
                        fds[i] = fds.back();
                        fds.pop_back();
                        auto c = std::exchange(ios[i], ios.back());
                        ios.pop_back();
                        c->complete();
                    }
            }
        }
    }
};

// ----------------------------------------------------------------------------

template <typename Res, short F, typename O, typename... P>
struct io_op {
    using result_t = Res;

    io_context&      c;
    int              fd;
    std::tuple<P...> p;

    io_op(auto& c, socket& s, auto... a): c(c), fd(s.fd), p(a...) {}

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

using async_accept = io_op<socket, POLLIN, decltype([](auto& s){
    ::sockaddr  addr{};
    ::socklen_t len{sizeof(addr)};
    auto        fd{::accept(s.fd, &addr, &len)};
    if (0 <= fd) set_value(s.r, fd);
    else set_error(s.r, std::make_exception_ptr(std::runtime_error("accept")));
    })>;

using async_readsome = io_op<int, POLLIN, decltype([](auto& s){
    auto n = ::read(s.fd, get<0>(s.p), get<1>(s.p));
    if (0 <= n) set_value(s.r, n);
    else set_error(s.r, std::make_exception_ptr(std::runtime_error("read")));
    }), char*, std::size_t>;

using async_writesome = io_op<int, POLLOUT, decltype([](auto& s){
    auto n = ::write(s.fd, get<0>(s.p), get<1>(s.p));
    if (0 <= n) set_value(s.r, n);
    else set_error(s.r, std::make_exception_ptr(std::runtime_error("write")));
    }), char const*, std::size_t>;

struct async_connect {
    using result_t = int;

    io_context&        c;
    int                fd;
    ::sockaddr const*  addr;
    ::socklen_t        len;

    async_connect(auto& c, socket& s, auto addr, auto len): c(c), fd(s.fd), addr(addr), len(len) {}

    template <typename R>
    struct state: io {
        ::sockaddr const*  addr;
        ::socklen_t        len;
        R                  r;
        state(auto& c, int fd, auto addr, auto len, R r): io(c, fd, POLLOUT), addr(addr), len(len), r(r) {}
        friend void start(state& self) {
            if (0 <= ::connect(self.fd, self.addr, self.len))
                set_value(self.r, 0);
            else if (errno == EAGAIN || errno == EINPROGRESS) {
                self.c.add(&self);
            }
            else
                set_error(self.r, std::make_exception_ptr(std::runtime_error(std::string("connect: ") + ::strerror(errno))));
        }
        void complete() override final {
            int         rc{};
            ::socklen_t len{sizeof rc};
            if (::getsockopt(fd, SOL_SOCKET, SO_ERROR, &rc, &len)) {
                set_error(r, std::make_exception_ptr(std::runtime_error(std::string("getsockopt: ") + ::strerror(errno))));
            }
            else if (rc) {
                set_error(r, std::make_exception_ptr(std::runtime_error(std::string("connect: ") + ::strerror(rc))));
            }
            else {
                set_value(r, rc);
            }
        }
    };

    template <typename R>
    friend state<R> connect(async_connect const& self, R r) {
        return state<R>(self.c, self.fd, self.addr, self.len, r);
    }
};

// ----------------------------------------------------------------------------

struct async_sleep_for {
    using result_t = toy::none;
    using duration_t = std::chrono::milliseconds;

    io_context& context;
    duration_t  duration;

    template <typename R>
    struct state
        : io {
        R           receiver;
        duration_t  duration;

        state(R receiver, io_context& context, duration_t duration)
            : io(context, 0, 0)
            , receiver(receiver)
            , duration(duration) {
        }
        friend void start(state& self) {
            self.c.add(std::chrono::system_clock::now() + self.duration,
                       &self);;
        }
        void complete() override {
            set_value(receiver, result_t{});
        }
    };
    template <typename R>
    friend state<R> connect(async_sleep_for self, R receiver) {
        return state<R>(receiver, self.context, self.duration);
    }
};

// ----------------------------------------------------------------------------

}

#endif
