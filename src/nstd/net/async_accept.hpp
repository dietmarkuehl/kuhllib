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

#include "nstd/net/async_io_.hpp"
#include "nstd/net/io_context.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/get_env.hpp"
#include "nstd/execution/get_scheduler.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/sender_adaptor_closure.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/file/context.hpp"

#include <system_error>
#include <functional>
#include <iostream> //-dk:TODO remove

// ----------------------------------------------------------------------------

namespace nstd::net::hidden_names {
    template <typename Operation, ::nstd::execution::receiver Receiver>
    struct async_io_state
        : ::nstd::file::context::io_base {
        ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
        Operation                                     d_operation;
        typename Operation::state                     d_state;

        template <typename O, ::nstd::execution::receiver R>
        async_io_state(O&& o, R&& r)
            : d_receiver(::nstd::utility::forward<R>(r))
            , d_operation(::nstd::utility::forward<O>(o))
            , d_state{} {
        }
        friend void tag_invoke(::nstd::execution::start_t, async_io_state& self) noexcept {
            ::std::cout << "async I/O start\n";
            // register cancellation
            auto env = nstd::execution::get_env(self.d_receiver);
            auto scheduler = ::nstd::execution::get_scheduler(env);
            self.d_operation.start(scheduler, self.d_state, &self);
        }

        auto do_result(int32_t rc, uint32_t flags) -> void override {
            this->d_operation.complete(rc, flags, this->d_state, this->d_receiver);
        }
    };

    template <typename Operation>
    struct async_io_sender
    {
        using completion_signatures
            = ::nstd::execution::completion_signatures<
                typename Operation::completion_signature,
                ::nstd::execution::set_error_t(int), //-dk:TODO
                ::nstd::execution::set_stopped_t()
            >;

        Operation d_operation;
        template <typename... P>
        async_io_sender(P&&... p)
            : d_operation{::nstd::utility::forward<P>(p)...} {
        }

        template <::nstd::execution::receiver Receiver>
        friend auto tag_invoke(::nstd::execution::connect_t, async_io_sender const& self, Receiver&& receiver)
            -> ::nstd::net::hidden_names::async_io_state<Operation, Receiver>
        {
            return { self.d_operation, ::nstd::utility::forward<Receiver>(receiver) };
        }
    };
}

// ----------------------------------------------------------------------------

namespace nstd::net::hidden_names::async_accept {
    template <typename Acceptor>
    struct operation {
        using completion_signature
            = ::nstd::execution::set_value_t(typename Acceptor::socket_type, typename Acceptor::endpoint_type);

        typename Acceptor::protocol_type      d_protocol;
        typename Acceptor::native_handle_type d_handle;
        struct state {
            ::sockaddr_storage               d_addr;
            ::socklen_t                      d_len{sizeof(::sockaddr_storage)};
        };
        auto start(::nstd::net::io_context::scheduler_type scheduler, state& s, ::nstd::file::context::io_base* cont) -> void{
            scheduler.accept(this->d_handle, reinterpret_cast<sockaddr*>(&s.d_addr), &s.d_len, 0, cont);
            ::std::cout << "async_accept start\n";
        }
        template <::nstd::execution::receiver Receiver>
        auto complete(int32_t rc, uint32_t, state& s, Receiver& receiver) -> void {
            ::std::cout << "async_accept complete\n";
            if (rc < 0) {
                ::nstd::execution::set_error(::nstd::utility::move(receiver), -rc);
            }
            else {
                typename Acceptor::endpoint_type endpoint;
                endpoint.set_address(&s.d_addr, s.d_len);
                ::nstd::execution::set_value(::nstd::utility::move(receiver), typename Acceptor::socket_type(this->d_protocol, rc), endpoint);
            }
        }
    };

    struct cpo {
        template <typename Acceptor>
        friend auto tag_invoke(cpo, Acceptor& acceptor) {
            return nstd::net::hidden_names::async_io_sender<::nstd::net::hidden_names::async_accept::operation<Acceptor>>(
                acceptor.protocol(),
                acceptor.native_handle()
                );
        }
        template <typename Acceptor>
        auto operator()(Acceptor& acceptor) const {
            return ::nstd::tag_invoke(*this, acceptor);
        }
    };
}

namespace nstd::net {
    using async_accept_t = ::nstd::net::hidden_names::async_accept::cpo;
    inline constexpr async_accept_t async_accept;
}

// ----------------------------------------------------------------------------

#endif
