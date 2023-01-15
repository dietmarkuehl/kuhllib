// nstd/net/async_connect.hpp                                         -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_ASYNC_CONNECT
#define INCLUDED_NSTD_NET_ASYNC_CONNECT

#include "nstd/file/async_io.hpp"
#include "nstd/net/io_context.hpp"
#include "nstd/net/socket.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/utility/move.hpp"

#include <system_error>

// ----------------------------------------------------------------------------

namespace nstd::hidden_names::async_connect {
    template <typename Socket, typename Env>
    struct operation {
        using completion_signature = ::nstd::execution::set_value_t();
        
        struct state {
            ::sockaddr_storage d_address;
            ::socklen_t        d_length{sizeof(::sockaddr_storage)};
            state(typename Socket::endpoint_type const& endpoint) {
                this->d_length = endpoint.get_address(&this->d_address);
            }
            auto start(Socket& socket, auto&& scheduler, ::nstd::file::io_base* cont) noexcept -> void {
                scheduler.connect(socket.native_handle(), reinterpret_cast<sockaddr*>(&this->d_address), this->d_length, cont);
            }
            template <typename Receiver>
            auto complete(::std::int32_t, ::std::uint32_t, Receiver& receiver) noexcept -> void {
                ::nstd::execution::set_value(::nstd::utility::move(receiver));
            }
        };
        static auto connect(Env const&, typename Socket::endpoint_type const& endpoint)
            -> state { return state(endpoint); }
    };
}

namespace nstd::net::inline customization_points {
    using async_connect_t =
        ::nstd::hidden_names::async_io::cpo<
            ::nstd::hidden_names::async_connect::operation
        >;
    inline constexpr async_connect_t async_connect_adapter{};

    template <::nstd::net::socket Socket>
    inline auto async_connect(
        Socket&& socket,
        typename ::nstd::type_traits::remove_cvref_t<Socket>::endpoint_type const& endpoint) {
        return ::nstd::net::async_connect_adapter(::nstd::execution::just(endpoint), socket);
    }
}

// ----------------------------------------------------------------------------

#endif
