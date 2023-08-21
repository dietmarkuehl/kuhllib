// toy-networking-generic.hpp                                         -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2023 Dietmar Kuehl http://www.dietmar-kuehl.de
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

#ifndef INCLUDED_TOY_NETWORKING_GENERIC
#define INCLUDED_TOY_NETWORKING_GENERIC

#include "toy-networking-poll.hpp"
#include "toy-socket.hpp"
#include "toy-stop_token.hpp"
#include "toy-starter.hpp"
#include <span>
#include <stdexcept>
#include <string>
#include <utility>

// ----------------------------------------------------------------------------

namespace toy::generic
{
    template <typename Result> struct io_base;
    template <typename Result> struct receiver;
    template <typename Result, typename Scheduler> struct receiver_scheduler;
    struct inner_state;
    template <typename State> struct concrete_state;
    template <typename Receiver, typename Args> struct io_state;
    template <typename Receiver, typename Operation> struct time_state;
    struct scheduler;

    class context_base;
    template <typename> class concrete_context;
    class context_members;
    class context;

}

// ----------------------------------------------------------------------------

template <typename Result>
struct toy::generic::io_base
{
    toy::in_place_stop_source d_source;

    virtual void do_set_value(Result result) = 0;
    virtual void do_set_error(std::exception_ptr result) = 0;
    virtual void do_set_stopped() = 0;
};

template <typename Result>
struct toy::generic::receiver
{
    io_base<Result>* state;

    friend void set_value(receiver const& self, Result result) { self.state->do_set_value(std::move(result)); }
    friend void set_error(receiver const& self, std::exception_ptr error) { self.state->do_set_error(error); }
    friend void set_stopped(receiver const& self) { self.state->do_set_stopped(); }

    friend toy::in_place_stop_source::stop_token get_stop_token(receiver const& self) { return self.state->d_source.token(); }
};

// ----------------------------------------------------------------------------

struct toy::generic::inner_state
{
    virtual ~inner_state() = default;
    virtual void do_start() = 0;
};

class toy::generic::context_base
    : public toy::io_context_base
{
public:
    using return_t = std::unique_ptr<toy::generic::inner_state>;

    virtual ~context_base() = default;

    virtual void run() = 0;

    virtual toy::starter<toy::generic::scheduler>& starter() = 0;

    virtual return_t start(toy::generic::receiver<toy::poll_t::args::result_t>, toy::socket&, toy::event_kind, toy::poll_t::args) = 0;
    virtual return_t start(toy::generic::receiver<toy::accept_t::args::result_t>, toy::socket&, toy::event_kind, toy::accept_t::args) = 0;
    virtual return_t start(toy::generic::receiver<toy::connect_t::args::result_t>, toy::socket&, toy::event_kind, toy::connect_t::args) = 0;
    virtual return_t start(toy::generic::receiver<toy::read_some_t::args::result_t>, toy::socket&, toy::event_kind, toy::read_some_t::args) = 0;
    virtual return_t start(toy::generic::receiver<toy::write_some_t::args::result_t>, toy::socket&, toy::event_kind, toy::write_some_t::args) = 0;
    virtual return_t start(toy::generic::receiver<toy::receive_t::args<std::span<iovec>>::result_t>, toy::socket&, toy::event_kind, toy::receive_t::args<std::span<iovec>>) = 0;
    virtual return_t start(toy::generic::receiver<toy::receive_from_t::args<std::span<iovec>>::result_t>, toy::socket&, toy::event_kind, toy::receive_from_t::args<std::span<iovec>>) = 0;
    virtual return_t start(toy::generic::receiver<toy::send_t::args<std::span<iovec>>::result_t>, toy::socket&, toy::event_kind, toy::send_t::args<std::span<iovec>>) = 0;
    virtual return_t start(toy::generic::receiver<toy::send_to_t::args<std::span<iovec>>::result_t>, toy::socket&, toy::event_kind, toy::send_to_t::args<std::span<iovec>>) = 0;

    virtual return_t start(toy::generic::receiver<toy::hidden::time_operation::sleep_for_op::result_t>, toy::hidden::time_operation::sleep_for_op) = 0;
    virtual return_t start(toy::generic::receiver<toy::hidden::time_operation::sleep_to_op::result_t>, toy::hidden::time_operation::sleep_to_op) = 0;
};

// ----------------------------------------------------------------------------

template <typename Receiver, typename Args>
struct toy::generic::io_state
    : toy::generic::io_base<typename Args::result_t>
{
    using stop_token_t = decltype(get_stop_token(std::declval<Receiver const&>()));
    struct callback
    {
        toy::generic::io_base<typename Args::result_t>* state;
        void operator()() const { this->state->d_source.stop(); }
    };
    using stop_callback = stop_token_t::template callback_type<callback>;

    Receiver                     receiver;
    toy::socket&                 socket;
    toy::event_kind              event;
    Args                         args;
    std::unique_ptr<inner_state> state;
    std::optional<stop_callback> d_callback;

    io_state(Receiver receiver, toy::socket& socket, toy::event_kind event, Args args)
        : receiver(receiver)
        , socket(socket)
        , event(event)
        , args(args)
    {
    }
    template <typename A> static A& transform_args(A& a) { return a; }
    template <typename MBS>
    static toy::receive_t::args<std::span<iovec>>
    transform_args(toy::receive_t::args<MBS>& a)
    {
        return { a.flags, std::span<iovec>(a.buffer) };
    }
    template <typename MBS>
    static toy::receive_from_t::args<std::span<iovec>>
    transform_args(toy::receive_from_t::args<MBS>& a)
    {
        return { std::span<iovec>(a.buffer), a.addr, a.flags };
    }
    template <typename MBS>
    static toy::send_t::args<std::span<iovec>>
    transform_args(toy::send_t::args<MBS>& a)
    {
        return { a.flags, std::span<iovec>(a.buffer) };
    }
    template <typename MBS>
    static toy::send_to_t::args<std::span<iovec>>
    transform_args(toy::send_to_t::args<MBS>& a)
    {
        return { std::span<iovec>(a.buffer), a.address, a.flags };
    }

    friend void start(io_state& self)
    {
        self.d_callback.emplace(get_stop_token(self.receiver), callback(&self));
        self.state = get_scheduler(self.receiver).start(toy::generic::receiver<typename Args::result_t>(&self), self.socket, self.event,
            transform_args(self.args));
    }
    void do_set_value(typename Args::result_t value) override
    {
        set_value(std::move(receiver), std::move(value));
    }
    void do_set_error(std::exception_ptr error) override
    {
        set_error(std::move(receiver), error);
    }
    void do_set_stopped() override
    {
        set_stopped(std::move(receiver));
    }
};

template <typename Receiver, typename Operation>
struct toy::generic::time_state
    : toy::generic::io_base<typename Operation::result_t>
{
    Receiver                     receiver;
    Operation                    operation;
    std::unique_ptr<inner_state> state;

    time_state(Receiver receiver, Operation operation)
        : receiver(receiver)
        , operation(operation)
    {
    }

    friend void start(time_state& self)
    {
        self.state = get_scheduler(self.receiver).start(toy::generic::receiver<typename Operation::result_t>(&self), self.operation);
    }
    void do_set_value(typename Operation::result_t value) override
    {
        set_value(std::move(receiver), value);
    }
    void do_set_error(std::exception_ptr error) override
    {
        set_error(std::move(receiver), error);
    }
    void do_set_stopped() override
    {
        set_stopped(std::move(receiver));
    }
};

// ----------------------------------------------------------------------------

struct toy::generic::scheduler
{
    template <typename Receiver, typename Args, typename>
    using io_state = toy::generic::io_state<Receiver, Args>;
    template <typename Receiver, typename Operation>
    using time_state = toy::generic::time_state<Receiver, Operation>;

    toy::generic::context_base* context;

    explicit scheduler(toy::generic::context_base* context): context(context) {}

    toy::io_context_base& base() { return *this->context; }


    template <typename Args>
    std::unique_ptr<toy::generic::inner_state>
    start(toy::generic::receiver<typename Args::result_t> receiver, toy::socket& socket, toy::event_kind event, Args args)
    {
        return context->start(receiver, socket, event, args);
    }

    template <typename Operation>
    std::unique_ptr<toy::generic::inner_state>
    start(toy::generic::receiver<typename Operation::result_t> receiver, Operation operation)
    {
        return context->start(receiver, operation);
    }

    template <typename Sender>
    void spawn(Sender&& sender) { this->context->starter().spawn(std::forward<Sender>(sender)); }
};

// ----------------------------------------------------------------------------

template <typename Result, typename Scheduler>
struct toy::generic::receiver_scheduler
    : toy::generic::receiver<Result>
{
    Scheduler                      d_scheduler;
    friend Scheduler get_scheduler(receiver_scheduler const& self) { return self.d_scheduler; }
};

template <typename State>
struct toy::generic::concrete_state
    : toy::generic::inner_state
{
    State state;
    template <typename Receiver, typename Args>
    concrete_state(Receiver receiver, toy::socket& socket, toy::event_kind event, Args args)
        : state(receiver, socket, event, args)
    {
    }
    template <typename Receiver, typename Operation>
    concrete_state(Receiver receiver, Operation operation)
        : state(receiver, operation)
    {
    }
    void do_start() override { start(state); }
};

template <typename Context>
class toy::generic::concrete_context
    : public toy::generic::context_base
{
private:
    Context& d_context;
    toy::starter<toy::generic::scheduler>& d_starter;

protected:
    void run() override { return this->d_context.run(); }

    socket_handle make_socket(int domain, int type, int protocol) override { return this->d_context.make_socket(domain, type, protocol); }
    socket_handle make_fd(socket_handle fd) override { return this->d_context.make_fd(fd); }
    socket_handle fd(socket_handle id) override { return this->d_context.fd(id); }
    int close(socket_handle fd) override { return this->d_context.close(fd); }
    void set_non_blocking(socket_handle fd) override { this->d_context.set_non_blocking(fd); }

public:
    concrete_context(Context& context, toy::starter<toy::generic::scheduler>& starter): d_context(context), d_starter(starter) {}

    toy::starter<toy::generic::scheduler>& starter() override
    {
        return this->d_starter;
    }

    template <typename Receiver, typename Args>
    return_t common_start(Receiver receiver, toy::socket& socket, toy::event_kind events, Args args)
    {
        using scheduler_t = decltype(this->d_context.get_scheduler());
        using receiver_t  = toy::generic::receiver_scheduler<typename Args::result_t, scheduler_t>;
        using state_t     = typename scheduler_t::template io_state<receiver_t, Args, toy::socket>;

        return_t rc(new toy::generic::concrete_state<state_t>(receiver_t{ {receiver}, this->d_context.get_scheduler()}, socket, events, args));
        rc->do_start();
        return rc;
    }
    return_t start(toy::generic::receiver<toy::poll_t::args::result_t> receiver, toy::socket& socket, toy::event_kind events, toy::poll_t::args args) override
    {
        return common_start(receiver, socket, events, args);
    }
    return_t start(toy::generic::receiver<toy::accept_t::args::result_t> receiver, toy::socket& socket, toy::event_kind events, toy::accept_t::args args) override
    {
        return common_start(receiver, socket, events, args);
    }
    return_t start(toy::generic::receiver<toy::connect_t::args::result_t> receiver, toy::socket& socket, toy::event_kind events, toy::connect_t::args args) override
    {
        return common_start(receiver, socket, events, args);
    }
    return_t start(toy::generic::receiver<toy::read_some_t::args::result_t> receiver, toy::socket& socket, toy::event_kind events, toy::read_some_t::args args) override
    {
        return common_start(receiver, socket, events, args);
    }
    return_t start(toy::generic::receiver<toy::write_some_t::args::result_t> receiver, toy::socket& socket, toy::event_kind events, toy::write_some_t::args args) override
    {
        return common_start(receiver, socket, events, args);
    }
    return_t start(toy::generic::receiver<toy::receive_t::args<std::span<iovec>>::result_t> receiver, toy::socket& socket, toy::event_kind events, toy::receive_t::args<std::span<iovec>> args) override
    {
        return common_start(receiver, socket, events, args);
    }
    return_t start(toy::generic::receiver<toy::receive_from_t::args<std::span<iovec>>::result_t> receiver, toy::socket& socket, toy::event_kind events, toy::receive_from_t::args<std::span<iovec>> args) override
    {
        return common_start(receiver, socket, events, args);
    }
    return_t start(toy::generic::receiver<toy::send_t::args<std::span<iovec>>::result_t> receiver, toy::socket& socket, toy::event_kind events, toy::send_t::args<std::span<iovec>> args) override
    {
        return common_start(receiver, socket, events, args);
    }
    return_t start(toy::generic::receiver<toy::send_to_t::args<std::span<iovec>>::result_t> receiver, toy::socket& socket, toy::event_kind events, toy::send_to_t::args<std::span<iovec>> args) override
    {
        return common_start(receiver, socket, events, args);
    }

    template <typename Receiver, typename Operation>
    return_t common_start(Receiver receiver, Operation operation)
    {
        using scheduler_t = decltype(this->d_context.get_scheduler());
        using receiver_t  = toy::generic::receiver_scheduler<typename Operation::result_t, scheduler_t>;
        using state_t     = typename scheduler_t::template time_state<receiver_t, Operation>;

        return_t rc(new toy::generic::concrete_state<state_t>(receiver_t{ {receiver}, this->d_context.get_scheduler()}, operation));
        rc->do_start();
        return rc;
    }
    return_t start(toy::generic::receiver<toy::hidden::time_operation::sleep_for_op::result_t> receiver, toy::hidden::time_operation::sleep_for_op operation) override
    {
        return common_start(receiver, operation);
    }
    return_t start(toy::generic::receiver<toy::hidden::time_operation::sleep_to_op::result_t> receiver, toy::hidden::time_operation::sleep_to_op operation) override
    {
        return common_start(receiver, operation);
    }
};

// ----------------------------------------------------------------------------

class toy::generic::context
    : public toy::io_context_base
{
private:
    std::unique_ptr<toy::poll::context>         d_default;
    std::unique_ptr<toy::generic::context_base> d_object;
    toy::starter<toy::generic::scheduler>       d_starter;

    socket_handle make_socket(int domain, int type, int protocol) override { return this->d_object->make_socket(domain, type, protocol); }
    socket_handle make_fd(socket_handle fd) override { return this->d_object->make_fd(fd); }
    socket_handle fd(socket_handle id) override { return this->d_object->fd(id); }
    int close(socket_handle fd) override { return this->d_object->close(fd); }
    void set_non_blocking(socket_handle fd) override { this->d_object->set_non_blocking(fd); }

public:
    using scheduler = toy::generic::scheduler;

    context()
        : toy::io_context_base()
        , d_default(new toy::poll::context)
        , d_object(new toy::generic::concrete_context<toy::poll::context>(*this->d_default, this->d_starter))
        , d_starter(toy::generic::scheduler(this->d_object.get()))
    {
    }
    template <typename Context>
    context(Context& context)
        : toy::io_context_base()
        , d_default()
        , d_object(new toy::generic::concrete_context<Context>(context), this->d_starter)
        , d_starter(toy::generic::scheduler(this->d_object.get()))
    {
    }

    void run() { this->d_object->run(); }
    template <typename Sender>
    void spawn(Sender&& sender) { this->d_starter.spawn(std::forward<Sender>(sender)); }
};

// ----------------------------------------------------------------------------

#endif
