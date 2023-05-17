// nstd/net/io_context.hpp                                            -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2021 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_NET_IO_CONTEXT
#define INCLUDED_NSTD_NET_IO_CONTEXT

#include "nstd/net/netfwd.hpp"
#include "nstd/execution/operation_state.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/schedule.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/execution/receiver.hpp"
//#include "nstd/executor/execution_context.hpp"
#include "nstd/file/context.hpp"
#include "nstd/file/ring_context.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include <exception>

// ----------------------------------------------------------------------------

namespace nstd::net {
    class io_context;
}

// ----------------------------------------------------------------------------

class nstd::net::io_context
    //-dk:TODO transfer interface? : public ::nstd::net::execution_context
{
private:
    ::std::unique_ptr<::nstd::file::context> d_manager;
    ::nstd::file::context*                   d_context;

public:
    using count_type = ::nstd::file::context::count_type;
    class executor_type {};
    class scheduler_type;
    template <::nstd::execution::receiver> struct state;
    struct sender;

    io_context();
    explicit io_context(::nstd::file::context& ctxt); // ctxt isn't owned
    explicit io_context(::nstd::file::context* ctxt); // ctxt isn't owned
    explicit io_context(::std::unique_ptr<::nstd::file::context> ctxt);
    explicit io_context(::nstd::file::ring_context::queue_size);
    explicit io_context(::std::size_t);
    io_context(io_context const&) = delete;
    io_context(io_context&&) = delete;
    auto operator=(io_context const&) -> io_context& = delete;
    auto operator=(io_context&&) -> io_context& = delete;

    auto hidden_context() -> ::nstd::file::context* { return this->d_context; }

    auto get_executor() noexcept -> executor_type;
    auto scheduler() noexcept -> ::nstd::net::io_context::scheduler_type;

    auto run_one() -> count_type;
    auto run() -> count_type;

    template <typename Rep, typename Period>
        auto run_for(::std::chrono::duration<Rep, Period> const&)
            -> ::nstd::file::context::count_type;
    template <typename Clock, typename Duration>
        auto run_until(::std::chrono::time_point<Clock, Duration> const&)
            -> ::nstd::file::context::count_type;
    template <typename Rep, typename Period>
        auto run_one_for(::std::chrono::duration<Rep, Period> const&)
            -> ::nstd::file::context::count_type;
    template <typename Clock, typename Duration>
        auto run_one_until(::std::chrono::time_point<Clock, Duration> const&)
            -> ::nstd::file::context::count_type;

    auto poll() -> ::nstd::file::context::count_type;
    auto poll_one() -> ::nstd::file::context::count_type;
    auto stop() -> void;
    auto stopped() const noexcept -> bool;
    auto restart() -> void;

};

// ----------------------------------------------------------------------------

namespace nstd::net {
    inline auto tag_invoke(::nstd::execution::schedule_t, nstd::net::io_context::scheduler_type const&)
        -> nstd::net::io_context::sender;
}

class nstd::net::io_context::scheduler_type
    : public ::nstd::execution::sender_tag
{
private:
    friend class ::nstd::net::io_context;
    ::nstd::net::io_context* d_context;
    explicit scheduler_type(::nstd::net::io_context* context) noexcept: d_context(context) {}

public:
    auto context() const noexcept -> ::nstd::net::io_context* { return this->d_context; }
    auto operator== (scheduler_type const& other) const -> bool = default;

    auto cancel(::nstd::file::context::io_base* to_cancel, ::nstd::file::context::io_base* cont) -> void {
        this->d_context->hidden_context()->cancel(to_cancel, cont);
    }
    auto nop(::nstd::file::context::io_base* cont) -> void {
        this->d_context->hidden_context()->nop(cont);
    }
    auto accept(::nstd::file::context::native_handle_type fd, ::sockaddr* addr, ::socklen_t* len, int flags, ::nstd::file::context::io_base* cont) -> void {
        this->d_context->hidden_context()->accept(fd, addr, len, flags, cont);
    }
    auto connect(::nstd::file::context::native_handle_type fd, ::sockaddr const* addr, ::socklen_t len, ::nstd::file::context::io_base* cont) -> void {
        this->d_context->hidden_context()->connect(fd, addr, len, cont);
    }
    auto recvmsg(::nstd::file::context::native_handle_type fd, ::msghdr* msg, int flags, ::nstd::file::context::io_base* cont) -> void {
        this->d_context->hidden_context()->recvmsg(fd, msg, flags, cont);
    }
    auto read(::nstd::file::context::native_handle_type fd, ::iovec* vec, ::std::size_t length, ::nstd::file::context::io_base* cont) -> void {
        this->d_context->hidden_context()->read(fd, vec, length, cont);
    }
    auto write(::nstd::file::context::native_handle_type fd, ::iovec* vec, ::std::size_t length, ::nstd::file::context::io_base* cont) -> void {
        this->d_context->hidden_context()->write(fd, vec, length, cont);
    }
    auto sendmsg(::nstd::file::context::native_handle_type fd, ::msghdr const* msg, int flags, ::nstd::file::context::io_base* cont) -> void {
        this->d_context->hidden_context()->sendmsg(fd, msg, flags, cont);
    }

    friend auto tag_invoke(::nstd::execution::schedule_t, scheduler_type const& scheduler) -> sender;
};

// ----------------------------------------------------------------------------

template <::nstd::execution::receiver Receiver>
struct nstd::net::io_context::state
    : ::nstd::file::context::io_base
{
    ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
    nstd::net::io_context::scheduler_type         d_scheduler;
    template <::nstd::execution::receiver R>
    state(R&& receiver, ::nstd::net::io_context::scheduler_type scheduler)
        : d_receiver(::nstd::utility::forward<R>(receiver))
        , d_scheduler(scheduler) {
    }
    friend auto tag_invoke(::nstd::execution::start_t, state& self) noexcept -> void {
        self.d_scheduler.nop(&self);
    }
    auto do_result(::std::int32_t, ::std::uint32_t) -> void override {
        ::nstd::execution::set_value(::nstd::utility::move(this->d_receiver));
    }
};

// ----------------------------------------------------------------------------

struct nstd::net::io_context::sender
{
    using completion_signatures
        = ::nstd::execution::completion_signatures<
            ::nstd::execution::set_value_t()
            //, ::nstd::execution::set_value_t(::std::int32_t, ::std::uint32_t)
            //, ::nstd::execution::set_stopped_t()
            >;

private:
    ::nstd::net::io_context::scheduler_type d_scheduler;

public:
    sender(::nstd::net::io_context::scheduler_type scheduler)
        : d_scheduler(scheduler) {
    }

    friend auto tag_invoke(::nstd::execution::get_completion_scheduler_t<::nstd::execution::set_value_t>, sender const& self) noexcept -> scheduler_type {
        return self.d_scheduler;
    }

    template <::nstd::execution::receiver Receiver>
    friend auto tag_invoke(::nstd::execution::connect_t, sender const& self, Receiver&& receiver) noexcept
        -> state<Receiver> {
        return state<Receiver>(::nstd::utility::forward<Receiver>(receiver), self.d_scheduler);
    }
};

// ----------------------------------------------------------------------------

inline auto nstd::net::io_context::scheduler() noexcept
    -> ::nstd::net::io_context::scheduler_type
{
    return ::nstd::net::io_context::scheduler_type(this);
}

inline auto nstd::net::tag_invoke(::nstd::execution::schedule_t, ::nstd::net::io_context::scheduler_type const& self)
    -> nstd::net::io_context::sender {
    return ::nstd::net::io_context::sender(self);
}

// ----------------------------------------------------------------------------

static_assert(::nstd::execution::operation_state<::nstd::net::io_context::state<::nstd::execution::hidden_names::test_receiver>>);
static_assert(::nstd::execution::sender<::nstd::net::io_context::sender>);
static_assert(::nstd::execution::scheduler<::nstd::net::io_context::scheduler_type>);

// ----------------------------------------------------------------------------

#endif
