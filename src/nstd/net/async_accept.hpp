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
#include "nstd/execution/receiver_of.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_done.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/file/context.hpp"
#include <system_error>

// ----------------------------------------------------------------------------

namespace nstd::net {
    inline constexpr struct async_accept_t {
        template <typename Socket, ::nstd::execution::receiver_of<::std::error_code, Socket&&>> struct receiver;
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

template <typename Socket, ::nstd::execution::receiver_of<::std::error_code, Socket&&> Receiver>
struct nstd::net::async_accept_t::receiver
    : public ::nstd::file::context::io_base
{
    ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
    ::nstd::file::context*                        d_context;
    typename Socket::protocol_type                d_protocol;
    typename Socket::native_handle_type           d_fd;
    ::sockaddr_storage                            d_address;
    ::socklen_t                                   d_length;

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
    auto do_result(::std::int32_t rc, ::std::uint32_t)
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
    template <template <typename...> class V, template <typename...> class T>
    using value_types = V<T<::std::error_code, Socket>>;
    template <template <typename...> class V>
    using error_types = V<::std::exception_ptr>;
    static constexpr bool sends_done = true;

    ::nstd::file::context*                 d_context;
    typename Socket::protocol_type const&  d_protocol;
    typename Socket::native_handle_type    d_fd;
    Sender                                 d_sender;

    template <::nstd::execution::receiver_of<::std::error_code, Socket&&> Receiver>
    friend auto tag_invoke(::nstd::execution::connect_t, sender const& sndr, Receiver&& receiver)
        noexcept
    {
        return ::nstd::execution::connect(::std::move(sndr.d_sender),
                                          ::nstd::net::async_accept_t::receiver<Socket, Receiver>{
                                              ::nstd::utility::forward<Receiver>(receiver),
                                              sndr.d_context,
                                              sndr.d_protocol,
                                              sndr.d_fd
                                          });
    }
};

//-dk:TODO add a static asserts confirming async_accept's properties
// static_assert(::nstd::execution::sender<
//                  ::nstd::net::async_accept_t::sender<
//                      ::nstd::net::basic_stream_socket<::nstd::net::ip::tcp>>>);

// ----------------------------------------------------------------------------

#endif
