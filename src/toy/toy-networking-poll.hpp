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
#ifdef TOY_HAS_WINSOCK2
#    include "toy-networking-winsock2.hpp"
#else
#    include "toy-networking-posix.hpp"
#endif
#include "toy-starter.hpp"
#include "toy-utility.hpp"

#include <algorithm>
#include <chrono>
#include <functional>
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
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

namespace toy
{

// ----------------------------------------------------------------------------

class io_context;
struct io_scheduler {
    io_context* context;
    friend std::ostream& operator<< (std::ostream& out, io_scheduler const& s) {
        return out << s.context;
    }
};

struct io
    : immovable {
    io_context& c;
    int         fd;
    short int   events;
    virtual void complete(int) = 0;
    io(io_context& c, int fd, short int events): c(c), fd(fd), events(events)  {}
};

class io_context
    : public starter<toy::io_scheduler>
    , public toy::io_context_base
{
public:
    using scheduler = toy::io_scheduler;
    scheduler get_scheduler() { return { this }; }

private:
    using time_point_t = std::chrono::system_clock::time_point;
    std::vector<io*>      ios;
    std::vector<::pollfd> fds;
    toy::timer_queue<io*> times;

public:
    io_context()
        : starter(scheduler(this)) {
    }
    void add(io* i) {
        ios.push_back(i);
        fds.push_back(  pollfd{ .fd = i->fd, .events = i->events });
    }
    void add(time_point_t time, io* op) {
        times.push(time, op);
    }
    void erase(io* i) {
        auto it = std::find(ios.begin(), ios.end(), i);
        if (it != ios.end()) {
            fds.erase(fds.begin() + (it - ios.begin()));
            ios.erase(it);
        }
    }
    void erase_timer(io* i) {
        times.erase(i);
    }
    void run() {
        while ( (!ios.empty() || not times.empty())) {
            auto now{std::chrono::system_clock::now()};

            bool timed{false};
            while (!times.empty() && times.top().first <= now) {
                io* op{times.top().second};
                times.pop();
                op->complete(0);
                timed = true;
            }
            if (timed) {
                continue;
            }
            auto time{times.empty()
                     ? -1
                : std::chrono::duration_cast<std::chrono::milliseconds>(times.top().first - now).count()};
            if (0 < ::poll(fds.data(), fds.size(), time)) {
                for (size_t i = fds.size(); i--; )
                    // The check for i < fds.size() is added as complete() may
                    // cause elements to get canceled and be removed from the
                    // list.
                    if (i < fds.size() && fds[i].events & fds[i].revents) {
                        int revents(fds[i].revents);
                        fds[i] = fds.back();
                        fds.pop_back();
                        auto c = std::exchange(ios[i], ios.back());
                        ios.pop_back();
                        c->complete(revents);
                    }
            }
        }
    }
};

// ----------------------------------------------------------------------------

namespace hidden_async_connect {
    struct async_connect {
        using result_t = int;

        int                fd;
        ::sockaddr const*  addr;
        ::socklen_t        len;

        async_connect(socket& s, auto addr, auto len): fd(s.fd()), addr(reinterpret_cast<::sockaddr const*>(addr)), len(len) {}

        template <typename R>
        struct state: io {
            ::sockaddr const*  addr;
            ::socklen_t        len;
            R                  receiver;
            hidden::io_operation::stop_callback<state, R> cb;

            state(int fd, auto addr, auto len, R r)
                : io(*get_scheduler(r).context, fd, POLLOUT), addr(addr), len(len), receiver(r) {
            }
            friend void start(state& self) {
                if (0 <= ::connect(self.fd, self.addr, self.len))
                    set_value(self.receiver, 0);
                else if (errno == EAGAIN || errno == EINPROGRESS) {
                    self.cb.engage(self);
                    self.c.add(&self);
                }
                else
                    set_error(self.receiver, std::make_exception_ptr(std::runtime_error(std::string("connect: ") + ::strerror(errno))));
            }
            void complete(int) override final {
                cb.disengage();
                int         rc{};
                ::socklen_t len{sizeof rc};
                if (::getsockopt(fd, SOL_SOCKET, SO_ERROR, &rc, &len)) {
                    set_error(receiver, std::make_exception_ptr(std::runtime_error(std::string("getsockopt: ") + ::strerror(errno))));
                }
                else if (rc) {
                    set_error(receiver, std::make_exception_ptr(std::runtime_error(std::string("connect: ") + ::strerror(rc))));
                }
                else {
                    set_value(receiver, rc);
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

namespace hidden::io_operation {
    template <typename Operation>
    struct sender {
        using result_t = typename Operation::result_t;

        toy::socket&  socket;
        Operation     op;

        template <typename R>
        struct state
            : toy::io
        {
            R                       receiver;
            Operation               d_op;
            toy::io_context_base*   context;
            stop_callback<state, R> cb;

            state(R                receiver,
                  int              fd,
                  Operation        op)
                : io(*get_scheduler(receiver).context, fd, op.event == event_kind::read? POLLIN: POLLOUT)
                , receiver(receiver)
                , d_op(op)
                , cb() {
            }

            friend void start(state& self) {
                self.cb.engage(self);
                self.c.add(&self);
            }
            void complete(int revents) override final {
                using result_t = typename Operation::result_t;
                cb.disengage();
                event_kind event(std::invoke([revents]{
                    switch (revents & (POLLIN | POLLOUT)) {
                        default: return event_kind::none;
                        case POLLIN: return event_kind::read;
                        case POLLOUT: return event_kind::write;
                        case POLLIN | POLLOUT: return event_kind::both;
                    }
                }));
                auto res{d_op(*this, event)};
                if constexpr (std::same_as<event_kind, decltype(res)>) {
                    set_value(std::move(receiver), result_t(res));
                }
                else if (0 <= res) {
                    if constexpr (requires(io_context_base& context, decltype(res) res){ result_t(context, res); }) {
                        auto& context(*get_scheduler(receiver).context);
                        set_value(std::move(receiver), result_t(context, res));
                    }
                    else {
                        set_value(std::move(receiver), result_t(res));
                    }
                }
                else {
                    set_error(std::move(receiver), std::make_exception_ptr(std::system_error(errno, std::system_category())));
                }
            }
        };
        template <typename R>
        friend state<R> connect(sender const& self, R receiver) {
            return state<R>(receiver, self.socket.fd(), self.op);
        }
    };
}

hidden::io_operation::sender<hidden::io_operation::poll_op>
async_poll(toy::socket& s, toy::hidden::io_operation::event_kind events) {
    return {s, { events }};
}

hidden::io_operation::sender<hidden::io_operation::accept_op>
async_accept(toy::socket& s) {
    return {s, {}};
}

hidden::io_operation::sender<hidden::io_operation::read_some_op>
async_read_some(toy::socket& s, char* buffer, std::size_t len) {
    return {s, {buffer, len}};
}

hidden::io_operation::sender<hidden::io_operation::write_some_op>
async_write_some(toy::socket& s, char const* buffer, std::size_t len) {
    return {s, {buffer, len}};
}

template <typename MBS>
hidden::io_operation::sender<hidden::io_operation::receive_op<MBS>>
async_receive(toy::socket& s, toy::message_flags f, const MBS& b) {
    return {s, {f, b}};
}
template <typename MBS>
hidden::io_operation::sender<hidden::io_operation::receive_op<MBS>>
async_receive(toy::socket& s, const MBS& b) {
    return {s, {toy::message_flags{}, b}};
}

template <typename MBS>
hidden::io_operation::sender<hidden::io_operation::receive_from_op<MBS>>
async_receive_from(toy::socket& s, const MBS& b, toy::address& addr, toy::message_flags f) {
    return {s, {b, addr, f}};
}
template <typename MBS>
hidden::io_operation::sender<hidden::io_operation::receive_from_op<MBS>>
async_receive_from(toy::socket& s, const MBS& b, toy::address& addr) {
    return {s, {b, addr, toy::message_flags{}}};
}

template <typename MBS>
hidden::io_operation::sender<hidden::io_operation::send_op<MBS>>
async_send(toy::socket& s, toy::message_flags f, const MBS& b) {
    return {s, {f, b}};
}
template <typename MBS>
hidden::io_operation::sender<hidden::io_operation::send_op<MBS>>
async_send(toy::socket& s, const MBS& b) {
    return {s, {toy::message_flags{}, b}};
}

template <typename MBS>
hidden::io_operation::sender<hidden::io_operation::send_to_op<MBS>>
async_send_to(toy::socket& s, const MBS& b, toy::address addr, toy::message_flags f) {
    return {s, {b, addr, f}};
}
template <typename MBS>
hidden::io_operation::sender<hidden::io_operation::send_to_op<MBS>>
async_send_to(toy::socket& s, const MBS& b, toy::address addr) {
    return {s, {b, addr, toy::message_flags{}}};
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
            R                            receiver;
            duration_t                   duration;
            hidden::io_operation::stop_callback<state, R, true> cb;

            state(R receiver, duration_t duration)
                : io(*get_scheduler(receiver).context, 0, 0)
                , receiver(receiver)
                , duration(duration) {
            }
            friend void start(state& self) {
                self.cb.engage(self);
                self.c.add(std::chrono::system_clock::now() + self.duration, &self);;
            }
            void complete(int) override {
                cb.disengage();
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
