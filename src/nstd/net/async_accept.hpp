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

#include "nstd/net/async_io.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/file/context.hpp"

#include <system_error>
// ----------------------------------------------------------------------------


namespace nstd::net {
    inline constexpr struct async_accept_t {
        template <typename Socket> struct io_operation;

        template <typename Acceptor, ::nstd::execution::sender Sender>
        friend auto tag_invoke(async_accept_t, Acceptor& acceptor, Sender sndr)
            -> ::nstd::net::async_io_sender<Sender, io_operation<typename Acceptor::socket_type>> {
            auto scheduler{::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sndr)};
            return nstd::net::async_io_sender<Sender, io_operation<typename Acceptor::socket_type>>{
                ::nstd::utility::move(sndr),
                scheduler.context()->hidden_context(),
                acceptor.protocol(),
                acceptor.native_handle()
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
    template <template <typename...> class T, template <typename...> class V>
    using value_types = V<T<::std::error_code, Socket>>;
    template <template <typename...> class V>
    using error_types = V<::std::exception_ptr>;

    struct parameters {
        typename Socket::protocol_type       d_protocol;
        typename Socket::native_handle_type  d_fd;
    };

    ::nstd::file::context*                        d_context;
    parameters                                    d_parameters;
    ::sockaddr_storage                            d_address;
    ::socklen_t                                   d_length;

    io_operation(::nstd::file::context*               context,
                 ::nstd::net::async_accept_t::io_operation<Socket>::parameters const& parameters)
        : d_context(context)
        , d_parameters(parameters)
        , d_address{}
        , d_length{}
    {
    }

    auto submit(::nstd::file::context::io_base* cont) -> void {
        this->d_context->accept(this->d_parameters.d_fd, reinterpret_cast<::sockaddr*>(&this->d_address), &this->d_length, 0, cont);
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
                                         Socket(this->d_parameters.d_protocol, rc));
        }
    }
};

// ----------------------------------------------------------------------------

#endif
