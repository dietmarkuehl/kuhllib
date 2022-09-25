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
#include <chrono>
#include <iostream> //-dk:TODO remove
#include <optional>
#include <stdexcept>
#include <string>
#include <system_error>
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
struct io_scheduler {
    io_context* context;
};

struct io
    : immovable {
    io_context& c;
    int         fd;
    short int   events;
    virtual void complete() = 0;
    io(io_context& c, int fd, short int events): c(c), fd(fd), events(events)  {
    }
};

class io_context
    : public starter<io_scheduler> {
public:
    using scheduler = toy::io_scheduler;
    scheduler get_scheduler() { return { this }; }

private:
    using time_point_t = std::chrono::system_clock::time_point;
    using kevent_t = struct ::kevent;
    static constexpr std::size_t size{4};

    int                   queue;
    std::size_t           outstanding{};
    std::size_t           changes{};
    kevent_t              change[size];
    kevent_t              result[size];
    toy::timer_queue<io*> times;

public:
    static constexpr bool has_timer = false; //-dk:TODO remove - used while adding timers to contexts
    io_context()
        : starter(get_scheduler())
        , queue(::kqueue()) {
        if (queue < 0) {
            throw std::runtime_error("can't create kqueue");
        }
    }
    ~io_context() {
        ::close(queue);
    }
    void flush_changes() {
        if (changes == size) {
            ::kevent(queue, change, changes, result, 0, nullptr);
            changes = 0u;
        }

    }
    void add(io* i) {
        flush_changes();
        change[changes++] = kevent_t{::uintptr_t(i->fd), ::int16_t(i->events), EV_ADD | EV_ONESHOT| EV_EOF, ::uint32_t(), ::intptr_t(), i};
        ++outstanding;
    }
    void add(time_point_t time, io* op) {
        times.push(time, op);
    }
    void erase(io* i) {
        flush_changes();
        change[changes++] = kevent_t{::uintptr_t(i->fd), ::int16_t(i->events), EV_DELETE, ::uint32_t(), ::intptr_t(), i};
        --outstanding;
    }
    void erase_timer(io* i) {
        times.erase(i);
    }
    void run() {
        while (0 < outstanding || !times.empty()) {
            auto now{std::chrono::system_clock::now()};

            bool timed{false};
            while (!times.empty() && times.top().first <= now) {
                io* op{times.top().second};
                times.pop();
                op->complete();
                timed = true;
            }
            if (timed) {
                continue;
            }

            timespec timeout;
            if (!times.empty()) {
                auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(times.top().first - now).count();
                timeout.tv_sec  = milliseconds / 1000;
                timeout.tv_nsec = 1000000 * (milliseconds % 1000);
            }

            int n = ::kevent(queue, change, changes, result, 1, times.empty()? nullptr: &timeout);
            changes = 0u;
            while (0 < n) {
                static_cast<io*>(result[--n].udata)->complete();
                --outstanding;
            }
        }
    }
};

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
            Operation               op;
            stop_callback<state, R> cb;

            state(R         receiver,
                  int       fd,
                  Operation op)
                : io(*get_scheduler(receiver).context, fd, Operation::event == event_kind::read? EVFILT_READ: EVFILT_WRITE)
                , receiver(receiver)
                , op(op)
                , cb() {
            }

            friend void start(state& self) {
                self.cb.engage(self);
                self.c.add(&self);
            }
            void complete() override final {
                cb.disengage();
                auto res{op(*this)};
                if (0 <= res)
                    set_value(std::move(receiver), typename Operation::result_t(res));
                else if (errno == EAGAIN) {
                    std::cout << "restart op=" << Operation::name << "\n";
                    start(*this);
                }
                else {
                    std::cout << "error: op=" << Operation::name << " res=" << res << " errno=" << errno << "\n";
                    set_error(std::move(receiver), std::make_exception_ptr(std::system_error(errno, std::system_category())));
                }  
            }
        };
        template <typename R>
        friend state<R> connect(sender const& self, R receiver) {
            return state<R>(receiver, self.socket.fd, self.op);
        }
    };
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

namespace hidden_async_connect {
    struct async_connect {
        using result_t = int;

        int               fd;
        ::sockaddr const* addr;
        ::socklen_t       len;

        async_connect(socket& sock, ::sockaddr const* addr, ::socklen_t len): fd(sock.fd), addr(addr), len(len) {}

        template <typename R>
        struct state
            : io {
            ::sockaddr const*       addr;
            ::socklen_t             len;
            R                       receiver;
            hidden::io_operation::stop_callback<state, R> cb;
            state(int fd, ::sockaddr const* addr, ::socklen_t len, R r)
                : io(*get_scheduler(r).context, fd, EVFILT_WRITE), addr(addr), len(len), receiver(r) {
            }
            friend void start(state& self) {
                switch (::connect(self.fd, self.addr, self.len)? errno: 0) {
                default:
                    set_error(self.receiver, std::make_exception_ptr(std::runtime_error(std::string("connect: ") + ::strerror(errno))));
                    break;
                case 0:
                    set_value(self.receiver, 0);
                    break;
                case EAGAIN:
                case EINPROGRESS:
                    self.cb.engage(self);
                    self.c.add(&self);
                    break;
                }
            }
            void complete() override final {
                cb.disengage();
                int         rc{};
                ::socklen_t len{sizeof rc};
                if (::getsockopt(fd, SOL_SOCKET, SO_ERROR, &rc, &len))
                    set_error(receiver, std::make_exception_ptr(std::runtime_error(std::string("getsockopt: ") + ::strerror(errno))));
                else
                    set_value(receiver, 0);
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

namespace hidden::async_sleep_for {
    struct sender {
        using duration_t = std::chrono::milliseconds;
        using result_t = none;
        std::chrono::milliseconds duration;

        template <typename R>
        struct state
            : io {
            R                         receiver;
            std::chrono::milliseconds duration;
            hidden::io_operation::stop_callback<state, R, true> cb;
            state(R receiver, duration_t duration)
                : io(*get_scheduler(receiver).context, 0, 0)
                , receiver(receiver)
                , duration(duration) {
            }
            friend void start(state& self) {
                self.cb.engage(self);
                self.c.add(std::chrono::system_clock::now() + self.duration, &self);
            }
            void complete() override {
                cb.disengage();
                set_value(receiver, result_t{});
            }
        };
        template <typename R>
        friend state<R> connect(sender self, R receiver) {
            return { receiver, self.duration };
        }
    };
}
using async_sleep_for = hidden::async_sleep_for::sender;

// ----------------------------------------------------------------------------

}

#endif
