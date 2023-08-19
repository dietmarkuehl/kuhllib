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
#include <functional>
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

namespace toy::epoll
{

// ----------------------------------------------------------------------------

class context;

struct io_base
{
    int id;
    toy::event_kind event; io_base(int id, toy::event_kind event): id(id), event(event) {}
    io_base(io_base&& other): id(std::exchange(other.id, -1)), event(other.event) {}
    ~io_base() = default;
    virtual int complete(short int events) = 0;
};

template <typename Receiver, typename Args>
struct io_state
    : toy::epoll::io_base
{
    using result_t = Args::result_t;
    using scheduler_t = decltype(get_scheduler(std::declval<Receiver const&>()));

    Receiver     receiver;
    Args         args;
    toy::socket& socket;
    toy::hidden::io_operation::stop_callback<io_state, Receiver> cb;

    scheduler_t scheduler() { return get_scheduler(receiver); }
    int fd() const { return socket.fd(); }

    io_state(Receiver receiver, toy::socket& socket, toy::event_kind events, Args args)
        : toy::epoll::io_base(socket.id(), events)
        , receiver(receiver)
        , args(args)
        , socket(socket)
    {
    }
    friend void start(io_state& self)
    {
        if constexpr (requires(toy::socket& s, Args args){ toy::hidden::io_operation::start(s, args); })
        {
            self.try_operation([&]{ return toy::hidden::io_operation::start(self.socket, self.args); });
        }
        else
        {
            self.try_operation([&]{ return toy::hidden::io_operation::operation(self.socket, event_kind::none, self.args); });
        }
    }
    template <typename Fun>
    void try_operation(Fun fun)
    {
        errno = 0;
        auto rc{fun()};
        int err = errno;
        if (std::invoke([rc]()->bool{
            if constexpr (std::same_as<result_t, event_kind>) {
                return rc == event_kind::none;
            }
            else {
                return rc < 0;
            }
            })) {
            if (err == EAGAIN || err == EWOULDBLOCK || err == EINPROGRESS) {
                cb.engage(*this);
                if (bool(args.event & event_kind::read)) {
                    scheduler().await(this, EPOLLIN);
                }
                if (bool(args.event & event_kind::write)) {
                    scheduler().await(this,  EPOLLOUT);
                }
            }
            else {
                set_error(std::move(receiver), std::make_exception_ptr(std::system_error(errno, std::system_category())));
            }
        }
        else {
            if constexpr (requires(io_context_base& context, decltype(rc) rc){ result_t(context, rc); }) {
                set_value(std::move(receiver), result_t(scheduler().base(), rc));
            }
            else {
                set_value(std::move(receiver), result_t(rc));
            }
        }
    }
    int complete(short int flags) override final {
        cb.disengage();
        event_kind event(std::invoke([flags]{
            switch (flags & (EPOLLIN | EPOLLOUT)) {
                default: return event_kind::none;
                case EPOLLIN: return event_kind::read;
                case EPOLLOUT: return event_kind::write;
                case EPOLLIN | EPOLLOUT: return event_kind::both;
            }
        }));
        try_operation([&]{ return toy::hidden::io_operation::operation(socket, event, args); });
        return 0;
    }
};

template <typename Receiver, typename Operation>
struct time_state
    : toy::epoll::io_base
{
    using result_t = Operation::result_t;
    using scheduler_t = decltype(get_scheduler(std::declval<Receiver const&>()));

    Receiver  receiver;
    Operation operation;
    toy::hidden::io_operation::stop_callback<time_state, Receiver, true> cb;

    scheduler_t scheduler() const { return get_scheduler(receiver); }

    time_state(Receiver receiver, Operation operation)
        : io_base(0, {})
        , receiver(receiver)
        , operation(operation)
    {
    }
    friend void start(time_state& self) {
        self.cb.engage(self);
        self.scheduler().await(&self, self.operation.get_timepoint());
    }
    int complete(short int) override final {
        cb.disengage();
        set_value(std::move(receiver), result_t{});
        return 0;
    }
};

class scheduler
{
private:
    friend class toy::epoll::context;
    toy::epoll::context* context;

    explicit scheduler(toy::epoll::context* context): context(context) {}

public:
    template <typename Receiver, typename Operation, typename = toy::socket>
    using io_state = toy::epoll::io_state<Receiver, Operation>;
    template <typename Receiver, typename Operation>
    using time_state = toy::epoll::time_state<Receiver, Operation>;

    toy::io_context_base& base() const;
    void await(toy::epoll::io_base*, unsigned short events);
    void erase(toy::epoll::io_base*);
    void await(toy::epoll::io_base*, std::chrono::system_clock::time_point);
    void erase_timer(toy::epoll::io_base*);

    template <typename Sender>
    void spawn(Sender&& sender);
};

class context
    : public starter<toy::epoll::scheduler>
    , public toy::io_context_base
{
    using time_point_t = std::chrono::system_clock::time_point;

    struct filedesc {
        enum class kind { input = 0, output = 1 };
        int fd{};
        int events{0};
        io_base* op[2]{ nullptr, nullptr };
        bool used{false};
    };

    std::vector<filedesc> files;
    std::size_t           free_filedesc{};
    int                   poll;
    std::size_t           outstanding{};
    toy::timer_queue<io_base*> times;

    void resize()
    {
        if (files.size() == free_filedesc) {
            files.resize(files.empty()? 16u: files.size() * 2u);
            for (std::size_t i{free_filedesc}; i != files.size(); ++i) {
                files[i].fd = i + 1;
            }
        }
    }
protected:
    int make_socket(int domain, int type, int protocol) override
    {
        return make_fd(::socket(domain, type, protocol));
    }
    int make_fd(int fd) override {
        resize();
        int id = free_filedesc;
        free_filedesc = files[id].fd;
        files[id] = filedesc{fd};
        return id;
    }
    int close(int id) override {
        int rc{::close(files[id].fd)};
        files[id].fd = std::exchange(free_filedesc, id);
        return rc;
    }
    int fd(int id) override { return files[id].fd; }

public:
    void add(io_base* i, std::uint32_t events) {
        filedesc& desc{files[i->id]};
        auto op{desc.used? EPOLL_CTL_MOD: EPOLL_CTL_ADD};
        desc.used = true;
        desc.events |= events;
        modify(i, op, desc.events);
        if (events & EPOLLIN) {
            desc.op[int(filedesc::kind::input)] = i;
        }
        if (events & EPOLLOUT) {
            desc.op[int(filedesc::kind::output)] = i;
        }
        increment();
    }
    void erase(io_base* i) {
        filedesc& desc{files[i->id]};
        if (i->event == toy::event_kind::read) {
            desc.op[int(filedesc::kind::input)] = nullptr;
            decrement();
        }
        if (i->event == toy::event_kind::write) {
            desc.op[int(filedesc::kind::output)] = nullptr;
            decrement();
        }
    }
    int complete(io_base* i, short int events) {
        filedesc  desc{files[i->id]};
        files[i->id].events &= ~(events & (EPOLLIN | EPOLLOUT));
        this->erase(i);
        int rc{};
        if (events & EPOLLIN && desc.op[int(filedesc::kind::input)]) {
            desc.op[int(filedesc::kind::input)]->complete(events);
            ++rc;
        }
        if (events & EPOLLOUT && desc.op[int(filedesc::kind::output)]) {
            desc.op[int(filedesc::kind::output)]->complete(events);
            ++rc;
        }
        return rc;
    }

    void modify(io_base* i, auto op, std::uint32_t events) {
        ::epoll_event ev{events | EPOLLONESHOT, { static_cast<void*>(i) } };
        if (::epoll_ctl(poll, op, files[i->id].fd, &ev) < 0) {
            throw std::system_error(errno, std::system_category());
        }
    }
    void add(io_base* op, time_point_t time) {
        times.push(time, op);
    }
    void remove(io_base* i) {
        close(files[i->id].fd);
    }
public:
    using scheduler = toy::epoll::scheduler;
    scheduler get_scheduler() { return toy::epoll::scheduler(this); }

    context()
        : starter(scheduler(this))
        , poll(::epoll_create(1)) {
        if (poll < 0) {
            throw std::runtime_error("can't create epoll");
        }
    }
    ~context() {
        ::close(poll);
    }
    void increment() { ++outstanding; }
    void decrement() { --outstanding; }
    void erase_timer(io_base* i) {
        times.erase(i);
    }
    void run() {
        constexpr std::size_t max(16);
        ::epoll_event ev[max];
        while (0 < outstanding || !times.empty()) {
            auto now{std::chrono::system_clock::now()};

            bool timed{false};
            while (!times.empty() && times.top().first <= now) {
                io_base* op{times.top().second};
                times.pop();
                //complete(op, 0);
                op->complete(0);
                timed = true;
            }
            if (timed) {
                continue;
            }
            auto time{times.empty()
                     ? -1
                : std::chrono::duration_cast<std::chrono::milliseconds>(times.top().first - now).count()};
            int n = ::epoll_wait(poll, ev, max, time);
            while (0 < n) {
                --n;
                this->complete(static_cast<io_base*>(ev[n].data.ptr), ev[n].events);
            }
        }
    }
};

inline toy::io_context_base& scheduler::base() const
{
    return *context;
}

inline void scheduler::await(toy::epoll::io_base* io, unsigned short events)
{
    return context->add(io, events);
}

inline void scheduler::erase(toy::epoll::io_base* io)
{
    return context->erase(io);
}

inline void scheduler::await(toy::epoll::io_base* io, std::chrono::system_clock::time_point time)
{
    context->add(io, time);
}

inline void scheduler::erase_timer(toy::epoll::io_base* io)
{
    context->erase_timer(io);
}

template <typename Sender>
inline void scheduler::spawn(Sender&& sender)
{
    context->spawn(std::forward<Sender>(sender));
}

// ----------------------------------------------------------------------------

}

#endif
