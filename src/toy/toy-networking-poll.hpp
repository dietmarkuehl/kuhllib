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

namespace toy::poll
{

// ----------------------------------------------------------------------------

constexpr toy::event_kind to_event_kind(int events)
{
    switch (events & (POLLIN | POLLOUT)) {
        default: return toy::event_kind::none;
        case POLLIN: return toy::event_kind::read;
        case POLLOUT: return toy::event_kind::write;
        case POLLIN | POLLOUT: return toy::event_kind::both;
    }
}

constexpr short int to_poll(toy::event_kind events) {
    switch (events) {
        default:
        case toy::event_kind::none: return 0;
        case toy::event_kind::read: return POLLIN;
        case toy::event_kind::write: return POLLOUT;
        case toy::event_kind::both: return POLLIN | POLLOUT;
    }
}

class context;
class scheduler;

class io_base
    : public toy::immovable
{
private:
    int d_fd;

public:
    toy::event_kind events;
    virtual void    complete(int) = 0;
    io_base(int fd, toy::event_kind events): d_fd(fd), events(events) {}
    virtual ~io_base() = default;

    int fd() const { return d_fd; }
};

template <typename Receiver, typename Args>
struct io_state
    : toy::poll::io_base
{
    using scheduler_t = decltype(get_scheduler(std::declval<Receiver const&>()));

    Receiver     receiver;
    toy::socket& socket;
    Args args;
    toy::hidden::io_operation::stop_callback<io_state, Receiver> cb;
    io_state(Receiver receiver, toy::socket& s, toy::event_kind events, Args args)
        : io_base(s.fd(), events)
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

    void complete(int event)
    {
        using result_t = typename Args::result_t;
        cb.disengage();
        auto res{toy::hidden::io_operation::operation(socket, toy::poll::to_event_kind(event), args)};
        if constexpr (std::same_as<toy::event_kind, decltype(res)>) {
            set_value(std::move(receiver), result_t(res));
        }
        else if (0 <= res) {
            if constexpr (requires(io_context_base& context, decltype(res) res){ result_t(context, res); }) {
                auto& context(get_scheduler(receiver).base());
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
    : toy::poll::io_base
{
    using scheduler_t = decltype(get_scheduler(std::declval<Receiver const&>()));

    Receiver  receiver;
    Operation operation;
    toy::hidden::io_operation::stop_callback<time_state, Receiver, true> cb;
    time_state(Receiver receiver, Operation operation)
        : io_base(0, toy::event_kind::none)
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

    void complete(int)
    {
        using result_t = typename Operation::result_t;
        cb.disengage();
        set_value(std::move(receiver), result_t());
    }
};

class scheduler {
private:
    friend class toy::poll::context;
    friend class toy::poll::io_base;

    toy::poll::context* context;
    scheduler(toy::poll::context* context): context(context) {}

    friend std::ostream& operator<< (std::ostream& out, scheduler const& s) {
        return out << "poll(" << s.context << ")";
    }

    toy::poll::context& get_context() const { return *context; }

public:
    template <typename Receiver, typename Args, typename Stream = toy::socket>
    using io_state = toy::poll::io_state<Receiver, Args>;
    template <typename Receiver, typename Operation>
    using time_state = toy::poll::time_state<Receiver, Operation>;

    using time_point_t = std::chrono::system_clock::time_point;

    toy::io_context_base& base() const;

    void await(time_point_t time, toy::poll::io_base* op);
    void await(io_base* i);
    void erase(io_base* i);
    void erase_timer(io_base* i);

    template <typename Sender>
    void spawn(Sender&& sender);
};

class context
    : public starter<toy::poll::scheduler>
    , public toy::io_context_base
{
public:
    using scheduler = toy::poll::scheduler;
    scheduler get_scheduler() { return toy::poll::scheduler(this); }

private:
    using time_point_t = std::chrono::system_clock::time_point;
    std::vector<toy::poll::io_base*>      ios;
    std::vector<::pollfd> fds;
    toy::timer_queue<toy::poll::io_base*> times;

public:
    context()
        : starter(scheduler(this)) {
    }
    void add(toy::poll::io_base* i) {
        ios.push_back(i);
        fds.push_back(  pollfd{ .fd = i->fd(), .events = to_poll(i->events) });
    }
    void add(time_point_t time, toy::poll::io_base* op) {
        times.push(time, op);
    }
    void erase(toy::poll::io_base* i) {
        auto it = std::find(ios.begin(), ios.end(), i);
        if (it != ios.end()) {
            fds.erase(fds.begin() + (it - ios.begin()));
            ios.erase(it);
        }
    }
    void erase_timer(toy::poll::io_base* i) {
        times.erase(i);
    }
    void run() {
        while ( (!ios.empty() || not times.empty())) {
            auto now{std::chrono::system_clock::now()};

            bool timed{false};
            while (!times.empty() && times.top().first <= now) {
                toy::poll::io_base* op{times.top().second};
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
                        c->complete(int(to_event_kind(revents)));
                    }
            }
        }
    }
};

inline toy::io_context_base& scheduler::base() const { return *context; }

inline void scheduler::await(time_point_t time, toy::poll::io_base* op) { context->add(time, op); }
inline void scheduler::await(io_base* i) { context->add(i); }
inline void scheduler::erase(io_base* i) { context->erase(i); }
inline void scheduler::erase_timer(io_base* i) { context->erase(i); }

template <typename Sender>
inline void scheduler::spawn(Sender&& sender) { context->spawn(std::forward<Sender>(sender)); }

}

// ----------------------------------------------------------------------------

#endif
