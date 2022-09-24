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
#include <iostream> //-dk:TODO remove

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
    friend std::ostream& operator<< (std::ostream& out, io_scheduler const& s) {
        return out << s.context;
    }
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

namespace hidden::io_operation {
    struct accept_op {
        using result_t = toy::socket;
        static constexpr int event = POLLIN;
        static constexpr char const* name = "accept";

        int operator()(auto& state) {
            ::sockaddr  addr{};
            ::socklen_t len{sizeof(addr)};
            return ::accept(state.fd, &addr, &len);
        }
    };

    struct read_some_op {
        using result_t = int;
        static constexpr int event = POLLIN;
        static constexpr char const* name = "read_some";

        char*       buffer;
        std::size_t len;

        int operator()(auto& state) {
            return ::read(state.fd, buffer, len);
        }
    };

    struct write_some_op {
        using result_t = int;
        static constexpr int event = POLLOUT;
        static constexpr char const* name = "write_some";

        char const* buffer;
        std::size_t len;

        int operator()(auto& state) {
            return ::write(state.fd, buffer, len);
        }
    };

    template <typename MBS>
    struct receive_op {
        using result_t = std::size_t;
        static constexpr int event = POLLIN;
        static constexpr char const* name = "receive";

        toy::message_flags flags;
        MBS                buffer;
        ::ssize_t operator()(auto& state) {
            msghdr msg{
                .msg_name = nullptr,
                .msg_namelen = 0,
                .msg_iov = buffer.data(),
                .msg_iovlen = decltype(std::declval<msghdr>().msg_iovlen)(buffer.size()),
                .msg_control = nullptr,
                .msg_controllen = 0,
                .msg_flags = 0
            };
            return recvmsg(state.fd, &msg, int(flags));
        }
    };

    template <typename MBS>
    struct receive_from_op {
        using result_t = std::size_t;
        static constexpr int event = POLLIN;
        static constexpr char const* name = "receive_from";

        MBS                buffer;
        toy::address&      addr;
        toy::message_flags flags;
        ::ssize_t operator()(auto& state) {
            msghdr msg{
                .msg_name = &addr.as_addr(),
                .msg_namelen = addr.capacity(),
                .msg_iov = buffer.data(),
                .msg_iovlen = decltype(std::declval<msghdr>().msg_iovlen)(buffer.size()),
                .msg_control = nullptr,
                .msg_controllen = 0,
                .msg_flags = 0
            };
            ::ssize_t rc{recvmsg(state.fd, &msg, int(flags))};
            if (0 <= rc) {
                addr.resize(msg.msg_namelen);
            }
            return rc;
        }
    };

    template <typename MBS>
    struct send_op {
        using result_t = std::size_t;
        static constexpr int event = POLLOUT;
        static constexpr char const* name = "send";

        toy::message_flags flags;
        MBS                buffer;
        ::ssize_t operator()(auto& state) {
            msghdr msg{
                .msg_name = nullptr,
                .msg_namelen = 0,
                .msg_iov = buffer.data(),
                .msg_iovlen = decltype(std::declval<msghdr>().msg_iovlen)(buffer.size()),
                .msg_control = nullptr,
                .msg_controllen = 0,
                .msg_flags = 0
            };
            return sendmsg(state.fd, &msg, int(flags));
        }
    };

    template <typename MBS>
    struct send_to_op {
        using result_t = std::size_t;
        static constexpr int event = POLLOUT;
        static constexpr char const* name = "send_to";

        MBS                buffer;
        toy::address       address;
        toy::message_flags flags;
        ::ssize_t operator()(auto& state) {
            msghdr msg{
                .msg_name = &address.as_addr(),
                .msg_namelen = address.size(),
                .msg_iov = buffer.data(),
                .msg_iovlen = decltype(std::declval<msghdr>().msg_iovlen)(buffer.size()),
                .msg_control = nullptr,
                .msg_controllen = 0,
                .msg_flags = 0
            };
            return sendmsg(state.fd, &msg, int(flags));
        }
    };

    template <typename Operation>
    struct sender {
        using result_t = typename Operation::result_t;

        toy::socket&  socket;
        Operation     op;

        template <typename R>
        struct state
            : toy::io
        {
            struct callback {
                state& s;
                void operator()() {
                    s.c.erase(&s);
                    s.cb.reset();
                    set_stopped(std::move(s.receiver));
                }
            };
            using stop_token = decltype(get_stop_token(std::declval<R>()));
            using stop_callback = typename stop_token::template callback_type<callback>; 

            R                            receiver;
            Operation                    op;
            std::optional<stop_callback> cb;

            state(R         receiver,
                  int       fd,
                  Operation op)
                : io(*get_scheduler(receiver).context, fd, Operation::event)
                , receiver(receiver)
                , op(op)
                , cb() {
            }

            friend void start(state& self) {
                self.cb.emplace(get_stop_token(self.receiver), callback{self});
                self.c.add(&self);
            }
            void complete() override final {
                cb.reset();
                auto res{op(*this)};
                if (0 <= res)
                    set_value(std::move(receiver), typename Operation::result_t(res));
                else {
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
