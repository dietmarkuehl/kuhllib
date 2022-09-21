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

#include "toy-networking-common.hpp"
#include "toy-starter.hpp"
#include "toy-utility.hpp"

#include <algorithm>
#include <chrono>
#include <optional>
#include <queue>
#include <stdexcept>
#include <system_error>
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
struct io_scheduler {
    io_context* context;
};

struct io
    : immovable {
    io_context& c;
    int         fd;
    short int   events;
    virtual void complete() = 0;
    io(io_context& c, int fd, short int events): c(c), fd(fd), events(events)  {}
};

class io_context
    : public starter<toy::io_scheduler>
{
public:
    using scheduler = toy::io_scheduler;
    scheduler get_scheduler() { return { this }; }

private:
    using time_point_t = std::chrono::system_clock::time_point;
    using timer_t      = std::pair<time_point_t, io*>;
    struct compare_t { bool operator()(auto&& a, auto&& b) { return a.first > b.first; } };
    using queue_t      = std::vector<timer_t>;
    
    std::vector<io*>      ios;
    std::vector<::pollfd> fds;
    queue_t               times;

public:
    static constexpr bool has_timer = true; //-dk:TODO remove - used while adding timers to contexts

    io_context()
        : starter(get_scheduler()) {
    }
    void add(io* i) {
        ios.push_back(i);
        fds.push_back(  pollfd{ .fd = i->fd, .events = i->events });
    }
    void add(time_point_t time, io* op) {
        auto it(std::lower_bound(times.begin(), times.end(), std::make_pair(time, op), compare_t()));
        times.insert(it, std::make_pair(time, op));
    }
    void erase(io* i) {
        auto it = std::find(ios.begin(), ios.end(), i);
        if (it != ios.end()) {
            fds.erase(fds.begin() + (it - ios.begin()));
            ios.erase(it);
        }
    }
    void erase_timer(io* i) {
        auto it = std::find_if(times.begin(), times.end(), [i](auto&& p){ return p.second == i; });
        if (it != times.end()) {
            times.erase(it);
        }
    }
    void run() {
        while (
            (!ios.empty() || not times.empty())) { 
            auto now{std::chrono::system_clock::now()};

            bool timed{false};
            while (!times.empty() && times.front().first <= now) {
                io* op{times.front().second};
                times.erase(times.begin());
                op->complete();
                timed = true;
            }
            if (timed) {
                continue;
            }
            auto time{times.empty()
                     ? -1
                : std::chrono::duration_cast<std::chrono::milliseconds>(times.front().first - now).count()};
            if (0 < ::poll(fds.data(), fds.size(), time)) {
                for (size_t i = fds.size(); i--; )
                    // The check for i < fds.size() is added as complete() may
                    // cause elements to get canceled and be removed from the
                    // list.
                    if (i < fds.size() && fds[i].events & fds[i].revents) {
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

namespace hidden_io_op {
    template <typename Res, short F, typename O, typename... P>
    struct io_op {
        using result_t = Res;

        int              fd;
        std::tuple<P...> p;

        io_op(socket& s, auto... a): fd(s.fd), p(a...) {}

        template <typename R>
        struct state: io {
            struct callback {
                state& s;
                void operator()() {
                    s.c.erase(&s);
                    s.cb.reset();
                    set_stopped(std::move(s.r));
                }
            };
            using stop_token = decltype(get_stop_token(std::declval<R>()));
            using stop_callback = typename stop_token::template callback_type<callback>; 

            std::tuple<P...>             p;
            R                            r;
            std::optional<stop_callback> cb;
            state(int fd, auto p, R r): io(*get_scheduler(r).context, fd, F), p(p), r(r) {}
            friend void start(state& self) {
                self.cb.emplace(get_stop_token(self.r), callback{self});
                self.c.add(&self);
            }
            void complete() override final {
                cb.reset();
                O()(*this);
            }
        };

        template <typename R>
        friend state<R> connect(io_op const& self, R r) {
            return state<R>(self.fd, self.p, r);
        }
    };
}

using async_accept = hidden_io_op::io_op<socket, POLLIN, decltype([](auto& s){
    ::sockaddr  addr{};
    ::socklen_t len{sizeof(addr)};
    auto        fd{::accept(s.fd, &addr, &len)};
    if (0 <= fd) set_value(s.r, fd);
    else set_error(s.r, std::make_exception_ptr(std::runtime_error("accept")));
    })>;

using async_read_some = hidden_io_op::io_op<int, POLLIN, decltype([](auto& s){
    auto n = ::read(s.fd, get<0>(s.p), get<1>(s.p));
    if (0 <= n) set_value(s.r, n);
    else set_error(s.r, std::make_exception_ptr(std::runtime_error("read")));
    }), char*, std::size_t>;

using async_write_some = hidden_io_op::io_op<int, POLLOUT, decltype([](auto& s){
    auto n = ::write(s.fd, get<0>(s.p), get<1>(s.p));
    if (0 <= n) set_value(s.r, n);
    else set_error(s.r, std::make_exception_ptr(std::runtime_error("write")));
    }), char const*, std::size_t>;

namespace hidden_async_connect {
    struct async_connect {
        using result_t = int;

        int                fd;
        ::sockaddr const*  addr;
        ::socklen_t        len;

        async_connect(socket& s, auto addr, auto len): fd(s.fd), addr(addr), len(len) {}

        template <typename R>
        struct state: io {
            ::sockaddr const*  addr;
            ::socklen_t        len;
            R                  r;
            state(int fd, auto addr, auto len, R r): io(*get_scheduler(r).context, fd, POLLOUT), addr(addr), len(len), r(r) {}
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
            return state<R>(self.fd, self.addr, self.len, r);
        }
    };
}
using async_connect = hidden_async_connect::async_connect;

// ----------------------------------------------------------------------------

namespace hidden::send {
    template <typename MBS>
    struct sender {
        using result_t = std::size_t;

        toy::socket&       socket;
        toy::message_flags flags;
        MBS                buffer;

        template <typename R>
        struct state
            : toy::io
        {
            R                  receiver;
            int                fd;
            toy::message_flags flags;
            MBS                buffer;
            state(R                  receiver,
                  int                fd,
                  toy::message_flags flags,
                  MBS                buffer)
                : io(*get_scheduler(receiver).context, fd, POLLOUT)
                , receiver(receiver)
                , flags(flags)
                , buffer(buffer) {
            }

            friend void start(state& self) {
                self.c.add(&self);
            }
            void complete() override final {
                msghdr msg{
                    .msg_name = nullptr,
                    .msg_namelen = 0,
                    .msg_iov = buffer.data(),
                    .msg_iovlen = int(buffer.size()),
                    .msg_control = nullptr,
                    .msg_controllen = 0,
                    .msg_flags = 0
                };
                ::ssize_t n = ::sendmsg(fd, &msg, int(flags));
                if (n < 0)
                    set_error(std::move(receiver), std::make_exception_ptr(std::system_error(errno, std::system_category())));
                else   
                    set_value(std::move(receiver), std::size_t(n));
            }
        };
        template <typename R>
        friend state<R> connect(sender const& self, R receiver) {
            return state<R>(receiver, self.socket.fd, self.flags, self.buffer);
        }
    };
}

template <typename MBS>
hidden::send::sender<MBS> async_send(toy::socket& s, toy::message_flags f, const MBS& b) {
    return hidden::send::sender<MBS>{s, f, b};

}

template <typename MBS>
hidden::send::sender<MBS> async_send(toy::socket& s, const MBS& b) {
    return async_send(s, toy::message_flags{}, b);
}

// ----------------------------------------------------------------------------

namespace hidden::receive {
    template <typename MBS>
    struct sender {
        using result_t = std::size_t;

        toy::socket&       socket;
        toy::message_flags flags;
        MBS                buffer;

        template <typename R>
        struct state
            : toy::io
        {
            R                  receiver;
            int                fd;
            toy::message_flags flags;
            MBS                buffer;
            state(R                  receiver,
                  int                fd,
                  toy::message_flags flags,
                  MBS                buffer)
                : io(*get_scheduler(receiver).context, fd, POLLIN)
                , receiver(receiver)
                , flags(flags)
                , buffer(buffer) {
            }

            friend void start(state& self) {
                self.c.add(&self);
            }
            void complete() override final {
                msghdr msg{
                    .msg_name = nullptr,
                    .msg_namelen = 0,
                    .msg_iov = buffer.data(),
                    .msg_iovlen = int(buffer.size()),
                    .msg_control = nullptr,
                    .msg_controllen = 0,
                    .msg_flags = 0
                };
                ::ssize_t n = ::recvmsg(fd, &msg, int(flags));
                if (n < 0)
                    set_error(std::move(receiver), std::make_exception_ptr(std::system_error(errno, std::system_category())));
                else   
                    set_value(std::move(receiver), std::size_t(n));
            }
        };
        template <typename R>
        friend state<R> connect(sender const& self, R receiver) {
            return state<R>(receiver, self.socket.fd, self.flags, self.buffer);
        }
    };
}

template <typename MBS>
hidden::receive::sender<MBS> async_receive(toy::socket& s, toy::message_flags f, const MBS& b) {
    return hidden::receive::sender<MBS>{s, f, b};

}

template <typename MBS>
hidden::receive::sender<MBS> async_receive(toy::socket& s, const MBS& b) {
    return async_receive(s, toy::message_flags{}, b);
}

// ----------------------------------------------------------------------------

namespace hidden_async_sleep_for {
    struct async_sleep_for {
        using result_t = toy::none;
        using duration_t = std::chrono::milliseconds;

        duration_t  duration;

        template <typename R>
        struct state
            : io {
            struct callback {
                state& s;
                void operator()() {
                    s.c.erase_timer(&s);
                    s.cb.reset();
                    set_stopped(s.receiver);
                }
            };
            using stop_token = decltype(get_stop_token(std::declval<R>()));
            using stop_callback = typename stop_token::template callback_type<callback>; 

            R                            receiver;
            duration_t                   duration;
            std::optional<stop_callback> cb;

            state(R receiver, duration_t duration)
                : io(*get_scheduler(receiver).context, 0, 0)
                , receiver(receiver)
                , duration(duration) {
            }
            friend void start(state& self) {
                self.cb.emplace(get_stop_token(self.receiver), callback{self});
                self.c.add(std::chrono::system_clock::now() + self.duration, &self);;
            }
            void complete() override {
                cb.reset();
                set_value(receiver, result_t{});
            }
        };
        template <typename R>
        friend state<R> connect(async_sleep_for self, R receiver) {
            return state<R>(receiver, self.duration);
        }
    };
}
using async_sleep_for = hidden_async_sleep_for::async_sleep_for;

// ----------------------------------------------------------------------------

}

#endif
