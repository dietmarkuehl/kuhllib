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

#include "nstd/file/async_io.hpp"
#include "nstd/net/io_context.hpp"
#include "nstd/net/socket.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/get_env.hpp"
#include "nstd/execution/get_scheduler.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/sender_adaptor_closure.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/file/context.hpp"

#include <system_error>

// ----------------------------------------------------------------------------

namespace nstd::hidden_names::async_accept {
    template <typename Acceptor, typename Env>
    struct operation {
        using completion_signature
            = ::nstd::execution::set_value_t(typename Acceptor::socket_type, typename Acceptor::endpoint_type);
        
        struct state {
            ::std::optional<typename Acceptor::protocol_type> d_protocol;
            ::sockaddr_storage d_address;
            ::socklen_t        d_length{sizeof(::sockaddr_storage)};
            auto start(Acceptor& socket, auto&& scheduler, ::nstd::file::io_base* cont) noexcept -> void {
                this->d_protocol = socket.protocol();
                scheduler.accept(socket.native_handle(), reinterpret_cast<sockaddr*>(&this->d_address), &this->d_length, 0, cont);
            }
            template <typename Receiver>
            auto complete(::std::int32_t fd, ::std::uint32_t, Receiver& receiver) noexcept -> void {
                typename Acceptor::endpoint_type endpoint;
                endpoint.set_address(&this->d_address, this->d_length);
                ::nstd::execution::set_value(
                    ::nstd::utility::move(receiver),
                    typename Acceptor::socket_type(*this->d_protocol, fd),
                    endpoint
                );
            }
        };
        static auto connect(Env const&) -> state { return {}; };
    };
}

namespace nstd::net::inline customization_points {
    using async_accept_t =
        ::nstd::hidden_names::async_io::cpo<
            ::nstd::hidden_names::async_accept::operation
        >;
    inline constexpr async_accept_t async_accept_adapter{};

    template <::nstd::net::socket Socket>
    inline auto async_accept(Socket&& socket) {
        return ::nstd::net::async_accept_adapter(::nstd::execution::just(), socket);
    }
}

// ----------------------------------------------------------------------------

#endif
