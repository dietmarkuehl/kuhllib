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

#include "nstd/net/async_io.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/utility/move.hpp"

#include <system_error>

// ----------------------------------------------------------------------------

namespace nstd::net {
    inline constexpr struct async_connect_t {
        template <typename Socket> struct io_operation;

        template <typename Socket, ::nstd::execution::sender Sender>
        friend auto tag_invoke(async_connect_t, Socket& socket, typename Socket::endpoint_type const& endpoint, Sender sndr) {
            auto scheduler{::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sndr)};
            return nstd::net::async_io_sender<decltype(scheduler), Sender, io_operation<Socket>>{
                scheduler,
                ::nstd::utility::move(sndr),
                socket.native_handle(),
                endpoint
                };
        }
        template <::nstd::execution::sender Sender, typename Socket>
        auto operator()(Sender sender, Socket& socket, typename Socket::endpoint_type const& endpoint) const {
            return ::nstd::tag_invoke(*this, socket, endpoint, sender);
        }
        template <typename Socket>
        auto operator()(Socket& socket, typename Socket::endpoint_type const& endpoint) const {
            return [&socket, endpoint, this](::nstd::execution::sender auto sender){
                return ::nstd::tag_invoke(*this, socket, endpoint, sender);
                };
        }
    } async_connect;
}

// ----------------------------------------------------------------------------

template <typename Socket>
struct nstd::net::async_connect_t::io_operation
{
    template <template <typename...> class T, template <typename...> class V>
    using value_types = V<T<::std::error_code>>;
    template <template <typename...> class V>
    using error_types = V<::std::exception_ptr>;

    struct parameters {
        typename Socket::native_handle_type  d_fd;
        typename Socket::endpoint_type       d_endpoint;
    };

    parameters                                    d_parameters;
    ::sockaddr_storage                            d_address;
    ::socklen_t                                   d_length;

    io_operation(::nstd::net::async_connect_t::io_operation<Socket>::parameters const& parameters)
        : d_parameters(parameters)
        , d_address{}
        , d_length{}
    {
    }

    template <::nstd::execution::scheduler Scheduler>
    auto submit(Scheduler&& scheduler, ::nstd::file::context::io_base* cont) -> void {
        this->d_length = this->d_parameters.d_endpoint.get_address(&this->d_address);
        scheduler.connect(this->d_parameters.d_fd, reinterpret_cast<::sockaddr*>(&this->d_address), this->d_length, cont);
    }
    template <typename Receiver>
    auto complete(::std::int32_t rc, std::uint32_t, Receiver& receiver) {
        ::nstd::execution::set_value(::nstd::utility::move(receiver),
                                     rc < 0
                                     ? ::std::error_code(-rc, ::std::system_category())
                                     : ::std::error_code());
    }
};

// ----------------------------------------------------------------------------

#endif
