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
#include "toy-networking-posix.hpp"
#include "toy-starter.hpp"
#include "toy-utility.hpp"
#include <chrono>
#include <functional>
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
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

namespace toy::kqueue
{

// ----------------------------------------------------------------------------

constexpr toy::event_kind to_event_kind(int events)
{
    switch (events & (EVFILT_READ | EVFILT_WRITE)) {
        default: return toy::event_kind::none;
        case EVFILT_READ: return toy::event_kind::read;
        case EVFILT_WRITE: return toy::event_kind::write;
        //case EVFILT_READ | EVFILT_WRITE: return toy::event_kind::both;
    }
}

constexpr short int to_kqueue(toy::event_kind events) {
    switch (events) {
        default:
        case toy::event_kind::none: return 0;
        case toy::event_kind::read: return EVFILT_READ;
        case toy::event_kind::write: return EVFILT_WRITE;
        case toy::event_kind::both: return EVFILT_READ | EVFILT_WRITE;
    }
}

// ----------------------------------------------------------------------------

class context;

struct io_base
    : immovable
{
    int         d_fd;
    short int   events;
    virtual void complete(toy::event_kind) = 0;
    io_base(int fd, short int events)
        : d_fd(fd)
        , events(events)
    {
    }
    int fd() const { return d_fd; }
};

template <typename Receiver, typename Args>
struct io_state
    : toy::kqueue::io_base
{
    using scheduler_t = decltype(get_scheduler(std::declval<Receiver const&>()));

    Receiver     receiver;
    toy::socket& socket;
    Args         args;
    toy::hidden::io_operation::stop_callback<io_state, Receiver> cb;
    io_state(Receiver receiver, toy::socket& s, toy::event_kind events, Args args)
        : io_base(s.fd(), toy::kqueue::to_kqueue(events))
        , receiver(receiver)
        , socket(s)
        , args(args)
    {
    }

    scheduler_t scheduler() { return get_scheduler(receiver); }

    friend void start(io_state& self)
    {
        self.cb.engage(self);
        if constexpr (requires(toy::socket& s, Args args){ toy::hidden::io_operation::start(s, args); })
        {
            toy::hidden::io_operation::start(self.socket, self.args);
        }
        get_scheduler(self.receiver).await(&self);
    }

    void complete(toy::event_kind event)
    {
        using result_t = typename Args::result_t;
        cb.disengage();
        auto res{toy::hidden::io_operation::operation(socket, event, args)};
        if constexpr (std::same_as<toy::event_kind, decltype(res)>) {
            set_value(std::move(receiver), result_t(res));
        }
        else if (0 <= res) {
            if constexpr (requires(io_context_base& context, decltype(res) res){ result_t(context, res); }) {
                toy::io_context_base& context(get_scheduler(receiver).base());
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

template <typename Receiver, typename Operation>
struct time_state
    : toy::kqueue::io_base
{
    using scheduler_t = decltype(get_scheduler(std::declval<Receiver const&>()));

    Receiver  receiver;
    Operation operation;
    toy::hidden::io_operation::stop_callback<time_state, Receiver, true> cb;
    time_state(Receiver receiver, Operation operation)
        : io_base(0, 0)
        , receiver(receiver)
        , operation(operation)
    {
    }

    scheduler_t scheduler() { return get_scheduler(receiver); }

    friend void start(time_state& self)
    {
        self.cb.engage(self);
        get_scheduler(self.receiver).await(self.operation.get_timepoint(), &self);
    }

    void complete(toy::event_kind)
    {
        using result_t = typename Operation::result_t;
        cb.disengage();
        set_value(std::move(receiver), result_t());
    }
};


struct scheduler {
    toy::kqueue::context* context;
    using io_base = toy::kqueue::io_base;
    using time_point_t = std::chrono::system_clock::time_point;

    template <typename Receiver, typename Args>
    using io_state = toy::kqueue::io_state<Receiver, Args>;
    template <typename Receiver, typename Operation>
    using time_state = toy::kqueue::time_state<Receiver, Operation>;

    toy::io_context_base& base() const;

    void await(toy::kqueue::io_base*);
    void await(time_point_t time, toy::kqueue::io_base* op);
    void erase(toy::kqueue::io_base*);
    void erase_timer(toy::kqueue::io_base*);

    template <typename Sender>
    void spawn(Sender&& sender);
};

class context
    : public starter<toy::kqueue::scheduler>
    , public toy::io_context_base
{
public:
    using scheduler = toy::kqueue::scheduler;
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
    toy::timer_queue<io_base*> times;

public:
    context()
        : starter(get_scheduler())
        , queue(::kqueue()) {
        if (queue < 0) {
            throw std::runtime_error("can't create kqueue");
        }
    }
    ~context() {
        ::close(queue);
    }
    void flush_changes() {
        if (changes == size) {
            ::kevent(queue, change, changes, result, 0, nullptr);
            changes = 0u;
        }

    }
    void add(io_base* i) {
        flush_changes();
        change[changes++] = kevent_t{::uintptr_t(i->fd()), ::int16_t(i->events), EV_ADD | EV_ONESHOT| EV_EOF, ::uint32_t(), ::intptr_t(), i};
        ++outstanding;
    }
    void add(time_point_t time, io_base* op) {
        times.push(time, op);
    }
    void erase(io_base* i) {
        flush_changes();
        change[changes++] = kevent_t{::uintptr_t(i->fd()), ::int16_t(i->events), EV_DELETE, ::uint32_t(), ::intptr_t(), i};
        --outstanding;
    }
    void erase_timer(io_base* i) {
        times.erase(i);
    }
    void run() {
        while (0 < outstanding || !times.empty()) {
            auto now{std::chrono::system_clock::now()};

            bool timed{false};
            while (!times.empty() && times.top().first <= now) {
                io_base* op{times.top().second};
                times.pop();
                op->complete(toy::event_kind::none);
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
                --n;
                using event_kind = toy::event_kind;
                event_kind event(std::invoke([filter = result[n].filter]{
                    switch (filter) {
                        default: return event_kind::none;
                        case EVFILT_READ: return event_kind::read;
                        case EVFILT_WRITE: return event_kind::write;
                    }
                }));
                static_cast<io_base*>(result[n].udata)->complete(event);
                --outstanding;
            }
        }
    }
};

inline void scheduler::await(toy::kqueue::io_base* op) { context->add(op); }
inline void scheduler::await(toy::kqueue::scheduler::time_point_t time, toy::kqueue::io_base* op) { context->add(time, op); }
inline void scheduler::erase(io_base* op) { context->erase(op); }
inline void scheduler::erase_timer(io_base* op) { context->erase_timer(op); }
inline toy::io_context_base& scheduler::base() const { return *context; }

template <typename Sender>
inline void scheduler::spawn(Sender&& sender) { context->spawn(std::forward<Sender>(sender)); }

// ----------------------------------------------------------------------------

}

#endif
