// nstd/net/async_accept.hpp                                          -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_ASYNC_ACCEPT
#define INCLUDED_NSTD_NET_ASYNC_ACCEPT

#include "nstd/execution/connect.hpp"
#include "nstd/execution/operation_state.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/get_stop_token.hpp"
#include "nstd/execution/receiver_of.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_done.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/file/context.hpp"
#include "nstd/stop_token/stoppable_token.hpp"

#include <atomic>
#include <mutex>
#include <new>
#include <optional>
#include <system_error>

// ----------------------------------------------------------------------------

namespace nstd::net {
    inline constexpr struct async_accept_t {
        template <typename Socket> struct io_operation;
        template <::nstd::execution::receiver Receiver, typename Submit> struct state_base;
        template <::nstd::execution::receiver Receiver, typename Submit> struct receiver;
        template <typename Socket, ::nstd::execution::sender , ::nstd::execution::receiver_of<::std::error_code, Socket&&> > struct state;
        template <typename, typename > struct sender;
        template <typename Acceptor, ::nstd::execution::sender Sender>
        friend auto tag_invoke(async_accept_t, Acceptor& acceptor, Sender sndr)
            -> sender<typename Acceptor::socket_type, ::nstd::type_traits::remove_cvref_t<Sender>> {
            auto scheduler{::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sndr)};
            return nstd::net::async_accept_t::sender<typename Acceptor::socket_type, ::nstd::type_traits::remove_cvref_t<Sender>>{
                scheduler.context()->hidden_context(),
                acceptor.protocol(),
                acceptor.native_handle(),
                ::nstd::utility::move(sndr)
                };
        }
        template <typename Acceptor, ::nstd::execution::sender Sender>
        auto operator()(Sender sender, Acceptor& acceptor) const {
            return ::nstd::tag_invoke(*this, acceptor, sender);
        }
        template <typename Acceptor>
        auto operator()(Acceptor& acceptor) const {
            return [&acceptor, this](::nstd::execution::sender auto sender){
                return ::nstd::tag_invoke(*this, acceptor, sender);
                };
        }
    } async_accept;
}

// ----------------------------------------------------------------------------

template <typename Socket>
struct nstd::net::async_accept_t::io_operation
{
    ::nstd::file::context*                        d_context;
    typename Socket::protocol_type                d_protocol;
    typename Socket::native_handle_type           d_fd;
    ::sockaddr_storage                            d_address;
    ::socklen_t                                   d_length;

    io_operation(::nstd::file::context*               context,
                 typename Socket::protocol_type       protocol,
                 typename Socket::native_handle_type  fd)
        : d_context(context)
        , d_protocol(protocol)
        , d_fd(fd)
        , d_address{}
        , d_length{}
    {
    }

    auto submit(::nstd::file::context::io_base* cont) -> void {
        this->d_context->accept(this->d_fd, reinterpret_cast<::sockaddr*>(&this->d_address), &this->d_length, 0, cont);
    }
    template <typename Receiver>
    auto complete(::std::int32_t rc, std::uint32_t, Receiver& receiver) {
        if (rc < 0) {
            ::nstd::execution::set_value(::nstd::utility::move(receiver),
                                         ::std::error_code(-rc, ::std::system_category()),
                                         Socket());
        }
        else {
            ::nstd::execution::set_value(::nstd::utility::move(receiver),
                                         ::std::error_code(),
                                         Socket(this->d_protocol, rc));
        }
    }
};

// ----------------------------------------------------------------------------

template <::nstd::execution::receiver Receiver, typename Submit>
struct nstd::net::async_accept_t::state_base
    : ::nstd::file::context::io_base
{
    ::nstd::type_traits::remove_cvref_t<Receiver>  d_receiver;
    ::nstd::type_traits::remove_cvref_t<Submit>    d_submit;
    template <typename R, typename S>
    state_base(R&& r, S&& s)
        : d_receiver(::nstd::utility::forward<R>(r))
        , d_submit(::nstd::utility::forward<S>(s))
    {
    }
    auto submit() -> void { this->d_submit.submit(this); }
};

// ----------------------------------------------------------------------------

template <::nstd::execution::receiver Receiver, typename Submit>
struct nstd::net::async_accept_t::receiver
{
    ::nstd::net::async_accept_t::state_base<Receiver, Submit>* d_state;

    friend auto tag_invoke(::nstd::execution::set_value_t, receiver&& self, auto&&...) noexcept -> void {
        try {
            self.d_state->submit();
        }
        catch (...) {
            ::nstd::execution::set_error(::nstd::utility::move(self.d_state->d_receiver), ::std::current_exception());
        }
    }
    template <typename Error>
    friend auto tag_invoke(::nstd::execution::set_error_t, receiver&& self, Error&& error) noexcept -> void {
        ::nstd::execution::set_error(::nstd::utility::move(self.d_state->d_receiver), ::nstd::utility::forward<Error>(error));
    }
    friend auto tag_invoke(::nstd::execution::set_done_t, receiver&& self) noexcept -> void {
        ::nstd::execution::set_done(::nstd::utility::move(self.d_state->d_receiver));
    }
};

#if 0
    : public ::nstd::file::cancelable_receiver<decltype(::nstd::execution::get_stop_token(::nstd::type_traits::declval<Receiver>()))>
{
    using stop_token = decltype(::nstd::execution::get_stop_token(::nstd::type_traits::declval<Receiver>()));
    struct cancel_callback {
        receiver* d_self;
        auto operator()() -> void;
    };
    using callback_type = typename stop_token::template callback_type<cancel_callback>;

    ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
    ::nstd::file::context*                        d_context;
    typename Socket::protocol_type                d_protocol;
    typename Socket::native_handle_type           d_fd;
    ::sockaddr_storage                            d_address;
    ::socklen_t                                   d_length;

    template <typename T>
    struct holder
    {
        bool d_engaged{false};
        union data {
            data() {}
            data(holder&&) {}
            ~data() {}
            T d_value;
        } d_data;
        holder() {}
        holder(holder&&) {}
        ~holder() {
            if (this->d_engaged) {
                this->d_data.d_value.~T();
            }
        }
        template <typename... Args>
        auto construct(Args&&... args) -> void {
            new(&this->d_data.d_value) T(::nstd::utility::forward<Args>(args)...);
            this->d_engaged = true;
        }
        auto get() -> T& { return this->d_data.d_value; }
    };
    holder<::std::mutex>  d_mutex;
    holder<callback_type> d_callback;

    template <::nstd::execution::receiver R>
    receiver(R&&                                    receiver,
             ::nstd::file::context*                 context,
             typename Socket::protocol_type const&  protocol,
             typename Socket::native_handle_type    fd)
        : d_receiver(::nstd::utility::forward<R>(receiver))
        , d_context(context)
        , d_protocol(protocol)
        , d_fd(fd)
        , d_length(sizeof(this->d_address))
    {
    }

    friend auto tag_invoke(::nstd::execution::set_value_t, receiver&& self)
        noexcept -> void {
        self.d_mutex.construct();
        ::std::lock_guard kerberos(self.d_mutex.get());
        self.d_callback.construct(::nstd::execution::get_stop_token(self.d_receiver), cancel_callback{&self});
        self.d_context->accept(self.d_fd, reinterpret_cast<::sockaddr*>(&self.d_address), &self.d_length, 0, &self);
    }
    template <typename E>
    friend auto tag_invoke(::nstd::execution::set_error_t, receiver&& self, E&& error)
        noexcept -> void {
        ::nstd::execution::set_error(::nstd::utility::move(self.d_receiver),
                                     ::nstd::utility::forward<E>(error));
    }
    friend auto tag_invoke(::nstd::execution::set_done_t, receiver&& self)
        noexcept -> void {
        ::nstd::execution::set_done(::nstd::utility::move(self.d_receiver));
    }
    auto do_final_result(::std::int32_t rc, ::std::uint32_t)
        -> void override
    {
        if (rc < 0) {
            ::nstd::execution::set_value(::nstd::utility::move(this->d_receiver),
                                         ::std::error_code(-rc, ::std::system_category()),
                                         Socket());
        }
        else {
            ::nstd::execution::set_value(::nstd::utility::move(this->d_receiver),
                                         ::std::error_code(),
                                         Socket(this->d_protocol, rc));

        }
    }
};

template <typename Socket, ::nstd::execution::receiver_of<::std::error_code, Socket&&> Receiver>
auto nstd::net::async_accept_t::receiver<Socket, Receiver>::cancel_callback::operator()() -> void
{
    ::std::lock_guard kerberos(this->d_self->d_mutex.get());
    this->d_self->d_context->cancel(this->d_self, nullptr);
}
#endif

// ----------------------------------------------------------------------------

template <typename Socket,
          ::nstd::execution::sender Sender,
          ::nstd::execution::receiver_of<::std::error_code, Socket&&> Receiver>
struct nstd::net::async_accept_t::state
    : ::nstd::net::async_accept_t::state_base<Receiver, ::nstd::net::async_accept_t::io_operation<Socket>>
{
    using io_op_t = ::nstd::net::async_accept_t::io_operation<Socket>;
    using base_t    = nstd::net::async_accept_t::state_base<Receiver, io_op_t>;
    using own_receiver_t = ::nstd::net::async_accept_t::receiver<Receiver, io_op_t>;
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
        ::nstd::file::context* d_context;
        base_t*                d_self;
        callback_type          d_callback;
        template <::nstd::execution::receiver R>
        cancel_state(::nstd::file::context* context, base_t* self, R& receiver)
            : d_mutex()
            , d_context(context)
            , d_self(self)
            , d_callback(::nstd::execution::get_stop_token(receiver), cancel_fun{this})
            {

        }
        auto cancel() -> void {
            ::std::lock_guard kerberos(this->d_mutex);
            if (this->d_outstanding == 1u) {
                ++this->d_outstanding;
                this->d_context->cancel(this->d_self, this);
            }
        }
        auto do_result(::std::int32_t, ::std::uint32_t) -> void override {
            ::std::lock_guard kerberos(this->d_mutex);
            if (--this->d_outstanding == 0) {
                ::nstd::execution::set_done(::nstd::utility::move(this->d_self->d_receiver));
            }
        }
        auto complete(::std::int32_t rc, ::std::uint32_t flags) -> void {
            ::std::lock_guard kerberos(this->d_mutex);
            if (--this->d_outstanding == 0) {
                this->d_self->d_submit.complete(rc, flags, this->d_self->d_receiver);
            }
        }
    };
    cancel_state d_cancel_state;

    template <::nstd::execution::sender S,
              ::nstd::execution::receiver_of<::std::error_code, Socket&&> R>
    state(S&& s,
          R&& r,
          ::nstd::file::context* context,
          typename Socket::protocol_type const& protocol,
          typename Socket::native_handle_type fd)
        : base_t{::nstd::utility::forward<R>(r),
                      io_op_t(context, protocol, fd)}
        , d_inner_state(::nstd::execution::connect(::nstd::utility::forward<S>(s),
                                                   own_receiver_t{this}))
        , d_cancel_state(context, this, this->d_receiver)
    {
    }

    friend auto tag_invoke(::nstd::execution::start_t, state& self) noexcept -> void {
        ::nstd::execution::start(self.d_inner_state);
    }

    auto do_result(::std::int32_t rc, ::std::uint32_t flags) -> void override {
        this->d_cancel_state.complete(rc, flags);
    }
};

template <typename Socket,
          ::nstd::execution::sender Sender,
          ::nstd::execution::receiver_of<::std::error_code, Socket&&> Receiver>
auto nstd::net::async_accept_t::state<Socket, Sender, Receiver>::cancel_state::cancel_fun::operator()() -> void {
    this->d_state->cancel();
}

// ----------------------------------------------------------------------------
//-dk:TODO add a static asserts confirming async_accept's properties
// static_assert(::nstd::execution::operation_state<
//                   ::nstd::net::async_accept_t::state<
//                       ::nstd::net::basic_stream_socket<::nstd::net::ip::tcp>,
//                       ::nstd::execution::test_receiver_of<::std::error_code,
//                                                           ::nstd::net::basic_stream_socket<::nstd::net::ip::tcp>&&>>>);

template <typename Socket, typename Sender>
struct nstd::net::async_accept_t::sender
{
    template <template <typename...> class T, template <typename...> class V>
    using value_types = V<T<::std::error_code, Socket>>;
    template <template <typename...> class V>
    using error_types = V<::std::exception_ptr>;
    static constexpr bool sends_done = true;

    ::nstd::file::context*                 d_context;
    typename Socket::protocol_type const&  d_protocol;
    typename Socket::native_handle_type    d_fd;
    Sender                                 d_sender;

    template <::nstd::execution::receiver_of<::std::error_code, Socket&&> Receiver>
    friend auto tag_invoke(::nstd::execution::connect_t, sender&& sndr, Receiver&& receiver)
        noexcept
        -> ::nstd::net::async_accept_t::state<Socket, Sender, Receiver>
    {
        return ::nstd::net::async_accept_t::state<Socket, Sender, Receiver>(
                ::nstd::utility::move(sndr.d_sender),
                ::nstd::utility::forward<Receiver>(receiver),
                sndr.d_context,
                sndr.d_protocol,
                sndr.d_fd);
    }
    template <::nstd::execution::receiver_of<::std::error_code, Socket&&> Receiver>
    friend auto tag_invoke(::nstd::execution::connect_t, sender const& sndr, Receiver&& receiver)
        noexcept
        -> ::nstd::net::async_accept_t::state<Socket, Sender, Receiver>
    {
        return ::nstd::net::async_accept_t::state<Socket, Sender, Receiver>(
                sndr.d_sender,
                ::nstd::utility::forward<Receiver>(receiver),
                sndr.d_context,
                sndr.d_protocol,
                sndr.d_fd);
    }
};

//-dk:TODO add a static asserts confirming async_accept's properties
// static_assert(::nstd::execution::sender<
//                  ::nstd::net::async_accept_t::sender<
//                      ::nstd::net::basic_stream_socket<::nstd::net::ip::tcp>>>);

// ----------------------------------------------------------------------------

#endif
