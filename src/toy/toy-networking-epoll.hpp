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

#include "toy-networking-common.hpp"
#include "toy-networking-posix.hpp"
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
    int                              fd;
    toy::hidden::io_operation::event_kind event;
    io(int fd, toy::hidden::io_operation::event_kind event): fd(fd), event(event) {}
    io(io&& other): fd(std::exchange(other.fd, -1)), event(other.event) {}
    ~io() = default;
    virtual int complete(short int events) = 0;
};

class io_context
    : public starter<toy::io_scheduler>
    , public toy::io_context_base
{
    static constexpr std::size_t size{4};
    using time_point_t = std::chrono::system_clock::time_point;

    int                   poll;
    std::size_t           outstanding{};
    toy::timer_queue<io*> times;

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
    void increment() { ++outstanding; }
    void decrement() { --outstanding; }
    void modify(io* i, auto op, std::uint32_t events) {
        ::epoll_event ev{events, { static_cast<void*>(i) } };
        if (::epoll_ctl(poll, op, i->fd, &ev) < 0) {
            throw std::system_error(errno, std::system_category());
        }
    }
    void add(time_point_t time, io* op) {
        times.push(time, op);
    }
    void erase_timer(io* i) {
        times.erase(i);
    }
    void remove(io* i) {
        close(i->fd);
    }
    void run() {
        ::epoll_event ev[1];
        while (0 < outstanding || !times.empty()) {
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
            int n = ::epoll_wait(poll, ev, 1, time);
            while (0 < n) {
                --n;
                outstanding -= static_cast<io*>(ev[n].data.ptr)->complete(ev[n].events);
            }
        }
    }
};

// ----------------------------------------------------------------------------

struct epoll_socket final
    : io
{
    enum class kind { input = 0, output = 1 };
    int events{0};
    io_context* c = nullptr;
    io* op[2]{ nullptr, nullptr };
    epoll_socket(int fd): io(fd, {}) {
        if (::fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
            throw std::runtime_error("fcntl");
        }
    }
    epoll_socket(epoll_socket&&) = default;
    ~epoll_socket() { if (fd != -1) ::close(fd); }

    void add(io_context& context, io* i, std::uint32_t events) {
        c = &context;
        if ((events & this->events) != events) {
            auto op{this->events? EPOLL_CTL_MOD: EPOLL_CTL_ADD};
            this->events |= events;
            context.modify(this, op, this->events);
        }
        if (events & EPOLLIN) {
            op[int(kind::input)] = i;
        }
        if (events & EPOLLOUT) {
            op[int(kind::output)] = i;
        }
    }
    void erase(io* i) {
        if (i->event == toy::hidden::io_operation::event_kind::read) {
            op[int(kind::input)] = nullptr;
            c->decrement();
        }
        if (i->event == toy::hidden::io_operation::event_kind::write) {
            op[int(kind::output)] = nullptr;
            c->decrement();
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

namespace hidden::io_operation {
    template <typename Operation>
    struct sender {
        using result_t = typename Operation::result_t;

        socket&    sock;
        Operation  op;

        template <typename Receiver>
        struct state: io {
            socket&                        c;
            Operation                      op;
            Receiver                       receiver;
            //stop_callback<state, Receiver> cb;

            state(socket& sock, auto op, Receiver r): io(sock.fd(), op.event), c(sock), op(op), receiver(r) {}
            friend void start(state& self) {
                (void)self;
#if 0
                if constexpr (requires(state& self, Operation op){ op.start(self); }) {
                    self.try_operation([&]{ return self.op.start(self); });
                }
                else {
                    self.try_operation([&]{ return self.op(self); });
                }
#endif
            }
            template <typename Fun>
            void try_operation(Fun fun) {
                auto rc{fun()};
                auto& context = *get_scheduler(receiver).context;
                if (rc < 0) {
#if 0
                    if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINPROGRESS) {
                        cb.engage(*this);
                        auto& context = *get_scheduler(receiver).context;
                        c.add(context, this, op.event == event_kind::read? EPOLLIN: EPOLLOUT);
                        context.increment();
                    }
                    else {
                        set_error(std::move(receiver), std::make_exception_ptr(std::system_error(errno, std::system_category())));
                    }
#endif
                }
                else {
                    if constexpr (requires(io_context_base& context, decltype(rc) rc){ result_t(context, rc); }) {
                        set_value(std::move(receiver), result_t(context, rc));
                    }
                    else {
                        set_value(std::move(receiver), result_t(rc));
                    }
                }
            }
            int complete(short int) override final {
                //cb.disengage();
                try_operation([&]{ return op(*this); });
                return 0;
            }
        };

        template <typename R>
        friend state<R> connect(sender const& self, R r) {
            return state<R>(self.sock, self.op, r);
        }
    };
}

// ----------------------------------------------------------------------------

hidden::io_operation::sender<hidden::io_operation::connect_op>
async_connect(toy::socket& s, sockaddr const* addr, socklen_t len)
{
    (void)addr;
    (void)len;
#if 0
    return hidden::io_operation::sender<hidden::io_operation::connect_op>{s, {addr, len}};
#else
    return hidden::io_operation::sender<hidden::io_operation::connect_op>{s, toy::address()};
#endif
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

namespace hidden::async_sleep_for {
    struct sender {
        using result_t = toy::none;
        using duration_t = std::chrono::milliseconds;

        duration_t  duration;

        template <typename R>
        struct state
            : io {
            R                            receiver;
            io_context&                  c;
            duration_t                   duration;
            hidden::io_operation::stop_callback<state, R, true> cb;

            state(R receiver, duration_t duration)
                : io(0, {})
                , receiver(receiver)
                , c(*get_scheduler(receiver).context)
                , duration(duration) {
            }
            friend void start(state& self) {
                self.cb.engage(self);
                self.c.add(std::chrono::system_clock::now() + self.duration, &self);;
            }
            int complete(short int) override final {
                cb.disengage();
                set_value(receiver, result_t{});
                return 0;
            }
        };
        template <typename R>
        friend state<R> connect(sender self, R receiver) {
            return state<R>(receiver, self.duration);
        }
    };
}
using async_sleep_for = hidden::async_sleep_for::sender;

// ----------------------------------------------------------------------------

}

#endif
