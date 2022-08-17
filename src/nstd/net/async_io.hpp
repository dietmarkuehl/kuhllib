// nstd/net/async_io.hpp                                              -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_ASYNC_IO
#define INCLUDED_NSTD_NET_ASYNC_IO

#include "nstd/execution/connect.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/get_env.hpp"
#include "nstd/execution/get_stop_token.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/file/context.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"

#include <mutex>


// ----------------------------------------------------------------------------

namespace nstd::net {
    template <::nstd::execution::scheduler, ::nstd::execution::receiver, typename IOOperation> struct async_io_state_base;
    template <::nstd::execution::scheduler, ::nstd::execution::receiver, typename IOOperation> struct async_io_receiver;
    template <::nstd::execution::scheduler, ::nstd::execution::sender, ::nstd::execution::receiver, typename IOOperation> struct async_io_state;
    template <::nstd::execution::scheduler, ::nstd::execution::sender, typename IOOperation> struct async_io_sender;
}

// ----------------------------------------------------------------------------

template <::nstd::execution::scheduler Scheduler, ::nstd::execution::receiver Receiver, typename IOOperation>
struct nstd::net::async_io_state_base
    : IOOperation::operation_type
{
    Scheduler                                        d_scheduler;
    ::nstd::type_traits::remove_cvref_t<Receiver>    d_receiver;
    ::nstd::type_traits::remove_cvref_t<IOOperation> d_submit;

    template <typename Sch, typename R, typename Socket, typename... Parameter>
    async_io_state_base(Sch&& scheduler, R&& r, Socket& socket, Parameter&&... p)
        : IOOperation::operation_type(::nstd::utility::forward<Parameter>(p)...)
        , d_scheduler(::nstd::utility::forward<Sch>(scheduler))
        , d_receiver(::nstd::utility::forward<R>(r))
        , d_submit(socket)
    {
    }
    auto enqueue() -> void { this->d_submit.enqueue(this); }
    auto do_submit() -> void override { this->d_submit.submit(this->d_scheduler, this); }
};

// ----------------------------------------------------------------------------

template <::nstd::execution::scheduler Scheduler, ::nstd::execution::receiver Receiver, typename IOOperation>
struct nstd::net::async_io_receiver
{
    ::nstd::net::async_io_state_base<Scheduler, Receiver, IOOperation>* d_state;

    friend auto tag_invoke(::nstd::execution::get_env_t, async_io_receiver const& self) noexcept {
        return ::nstd::execution::get_env(self.d_state->d_receiver);
    }
    friend auto tag_invoke(::nstd::execution::set_value_t, async_io_receiver&& self, auto&&...) noexcept -> void {
        try {
            self.d_state->enqueue();
        }
        catch (...) {
            ::nstd::execution::set_error(::nstd::utility::move(self.d_state->d_receiver), ::std::current_exception());
        }
    }
    template <typename Error>
    friend auto tag_invoke(::nstd::execution::set_error_t, async_io_receiver&& self, Error&& error) noexcept -> void {
        ::nstd::execution::set_error(::nstd::utility::move(self.d_state->d_receiver), ::nstd::utility::forward<Error>(error));
    }
    friend auto tag_invoke(::nstd::execution::set_stopped_t, async_io_receiver&& self) noexcept -> void {
        ::nstd::execution::set_stopped(::nstd::utility::move(self.d_state->d_receiver));
    }
};

// ----------------------------------------------------------------------------

template <::nstd::execution::scheduler Scheduler, ::nstd::execution::sender Sender, ::nstd::execution::receiver Receiver, typename IOOperation>
struct nstd::net::async_io_state
    : ::nstd::net::async_io_state_base<Scheduler, Receiver, IOOperation>
{
    using base_t    = nstd::net::async_io_state_base<Scheduler, Receiver, IOOperation>;
    using own_receiver_t = ::nstd::net::async_io_receiver<Scheduler, Receiver, IOOperation>;
    using inner_state_t  = decltype(::nstd::execution::connect(::nstd::type_traits::declval<Sender>(),
                                                               ::nstd::type_traits::declval<own_receiver_t>()));
    using StopToken = decltype(::nstd::execution::get_stop_token(::nstd::type_traits::declval<Receiver>()));

    inner_state_t d_inner_state;
    struct cancel_state
        : ::nstd::file::context::io_base
    {
        struct cancel_fun {
            cancel_state* d_state;
            auto operator()() -> void;
        };
        using callback_type = typename StopToken::template callback_type<cancel_fun>;
        ::std::mutex           d_mutex;
        ::std::size_t          d_outstanding{1u};
        Scheduler              d_scheduler;
        base_t*                d_self;
        callback_type          d_callback;
        bool                   d_canceled{false};
        template <::nstd::execution::scheduler Sch, ::nstd::execution::receiver R>
        cancel_state(Sch&& scheduler, base_t* self, R& receiver)
            : d_mutex()
            , d_scheduler(::nstd::utility::forward<Sch>(scheduler))
            , d_self(self)
            , d_callback(::nstd::execution::get_stop_token(receiver), cancel_fun{this}) {
        }
        auto cancel() -> void {
            ::std::lock_guard kerberos(this->d_mutex);
            if (this->d_outstanding == 1u) {
                this->d_canceled = true;
                ++this->d_outstanding;
                this->d_scheduler.cancel(this->d_self, this);
            }
        }
        auto do_result(::std::int32_t, ::std::uint32_t) -> void override {
            ::std::lock_guard kerberos(this->d_mutex);
            if (--this->d_outstanding == 0) {
                ::nstd::execution::set_stopped(::nstd::utility::move(this->d_self->d_receiver));
            }
        }
        auto complete(::std::int32_t rc, ::std::uint32_t flags) -> void {
            ::std::lock_guard kerberos(this->d_mutex);
            if (--this->d_outstanding == 0) {
                if (this->d_canceled) {
                    ::nstd::execution::set_stopped(::nstd::utility::move(this->d_self->d_receiver));
                }
                else {
                    this->d_self->d_submit.complete(rc, flags, this->d_self, this->d_self->d_receiver);
                }
            }
        }
    };
    cancel_state d_cancel_state;

    template <::nstd::execution::sender S,
              ::nstd::execution::receiver R,
              typename Socket,
              typename... P>
    async_io_state(Scheduler const& scheduler,
                   S&& s,
                   R&& r,
                   Socket& socket,
                   P&&... p)
        : base_t{scheduler, ::nstd::utility::forward<R>(r), socket, ::nstd::utility::forward<P>(p)...}
        , d_inner_state(::nstd::execution::connect(::nstd::utility::forward<S>(s),
                                                   own_receiver_t{this}))
        , d_cancel_state(scheduler, this, this->d_receiver)
    {
    }

    friend auto tag_invoke(::nstd::execution::start_t, async_io_state& self) noexcept -> void {
        ::nstd::execution::start(self.d_inner_state);
    }

    auto do_result(::std::int32_t rc, ::std::uint32_t flags) -> void override {
        this->d_cancel_state.complete(rc, flags);
    }
};

template <::nstd::execution::scheduler Scheduler, ::nstd::execution::sender Sender, ::nstd::execution::receiver Receiver, typename IOOperation>
auto nstd::net::async_io_state<Scheduler, Sender, Receiver, IOOperation>::cancel_state::cancel_fun::operator()() -> void {
    this->d_state->cancel();
}

// ----------------------------------------------------------------------------

template <::nstd::execution::scheduler Scheduler, ::nstd::execution::sender Sender, typename IOOperation>
struct nstd::net::async_io_sender
{
    using completion_signatures = typename IOOperation::completion_signatures;
    template <template <typename...> class T, template <typename...> class V>
    using value_types = typename IOOperation::template value_types<T, V>;
    template <template <typename...> class V>
    using error_types = typename IOOperation::template error_types<V>;
    static constexpr bool sends_done = true;
    using scheduler_type = ::nstd::type_traits::remove_cvref_t<Scheduler>;
    using sender_type = ::nstd::type_traits::remove_cvref_t<Sender>;

    scheduler_type                       d_scheduler;
    sender_type                          d_sender;
    typename IOOperation::socket_type&   d_socket;
    typename IOOperation::parameter_type d_parameters;

    template <::nstd::execution::scheduler Sch, ::nstd::execution::sender S, typename Socket, typename... Args>
    async_io_sender(Sch&& scheduler, S&& sender, Socket& socket, Args&&... args)
        : d_scheduler(::nstd::utility::forward<Sch>(scheduler))
        , d_sender(::nstd::utility::forward<S>(sender))
        , d_socket(socket)
        , d_parameters(::nstd::utility::forward<Args>(args)...)
    {
    }

    friend auto tag_invoke(::nstd::execution::get_completion_scheduler_t<::nstd::execution::set_value_t>, async_io_sender const& sndr)
        noexcept
    {
        return ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sndr.d_sender);
    }
    template <::nstd::execution::receiver Receiver>
    friend auto tag_invoke(::nstd::execution::connect_t, async_io_sender&& sndr, Receiver&& receiver)
        noexcept
        -> ::nstd::net::async_io_state<scheduler_type, sender_type, Receiver, IOOperation>
    {
        return ::std::apply([&](auto&&...p){
                return ::nstd::net::async_io_state<scheduler_type, sender_type, Receiver, IOOperation>(
                    sndr.d_scheduler,
                    ::nstd::utility::move(sndr.d_sender),
                    ::nstd::utility::forward<Receiver>(receiver),
                    sndr.d_socket,
                    ::nstd::utility::forward<decltype(p)>(p)...);
            }, ::nstd::utility::move(sndr.d_parameters));
    }
    template <::nstd::execution::receiver Receiver>
    friend auto tag_invoke(::nstd::execution::connect_t, async_io_sender const& sndr, Receiver&& receiver)
        noexcept
        -> ::nstd::net::async_io_state<scheduler_type, sender_type, Receiver, IOOperation>
    {
        return ::std::apply([&](auto&&...p){
                return ::nstd::net::async_io_state<scheduler_type, sender_type, Receiver, IOOperation>(
                    sndr.d_scheduler,
                    ::nstd::utility::move(sndr.d_sender),
                    ::nstd::utility::forward<Receiver>(receiver),
                    sndr.d_socket,
                    ::nstd::utility::forward<decltype(p)>(p)...);
            }, sndr.d_parameters);
    }
};

// ----------------------------------------------------------------------------

#endif
