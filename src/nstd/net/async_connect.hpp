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
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/utility/move.hpp"

#include <system_error>

// ----------------------------------------------------------------------------

namespace nstd::net::hidden_names::async_connect {
    template <typename Socket> struct operation;
    struct cpo;
}

// ----------------------------------------------------------------------------

template <typename Socket>
struct nstd::net::hidden_names::async_connect::operation {
    using completion_signature
        = ::nstd::execution::set_value_t();

    typename Socket::native_handle_type      d_handle;
    typename Socket::endpoint_type           d_endpoint;
    ::sockaddr_storage                       d_address{};
    struct state {
    };
    auto start(::nstd::net::io_context::scheduler_type scheduler, state&, ::nstd::file::context::io_base* cont) -> void{
        ::socklen_t length = this->d_endpoint.get_address(&this->d_address);
        scheduler.connect(this->d_handle, reinterpret_cast<::sockaddr*>(&this->d_address), length, cont);
    }
    template <::nstd::execution::receiver Receiver>
    auto complete(int32_t rc, uint32_t, bool cancelled, state&, Receiver& receiver) -> void {
        if (cancelled) {
            ::nstd::execution::set_stopped(::nstd::utility::move(receiver));
        }
        else if (rc < 0) {
            ::nstd::execution::set_error(::nstd::utility::move(receiver), ::std::error_code(-rc, std::system_category()));
        }
        else {
            ::nstd::execution::set_value(::nstd::utility::move(receiver));
        }
    }
};

// ----------------------------------------------------------------------------

struct nstd::net::hidden_names::async_connect::cpo {
    template <typename Socket>
    friend auto tag_invoke(cpo, Socket& socket, typename Socket::endpoint_type const& endpoint) {
        return nstd::file::hidden_names::async_io_sender<::nstd::net::hidden_names::async_connect::operation<Socket>>(
            socket.native_handle(), endpoint
            );
    }
    template <typename Socket>
    auto operator()(Socket& socket, typename Socket::endpoint_type const& endpoint) const {
        return ::nstd::tag_invoke(*this, socket, endpoint);
    }
};

// ----------------------------------------------------------------------------

namespace nstd::net::inline customization_points {
    using async_connect_t = ::nstd::net::hidden_names::async_connect::cpo;
    inline constexpr async_connect_t async_connect;
}

// ----------------------------------------------------------------------------

#endif
