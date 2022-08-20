// nstd/net/async_send.hpp                                            -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_ASYNC_SEND
#define INCLUDED_NSTD_NET_ASYNC_SEND

#include "nstd/net/async_io.hpp"
#include "nstd/file/operation.hpp"
#include "nstd/hidden_names/message_flags.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/sender_adaptor_closure.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/buffer/const_buffer.hpp"
#include "nstd/utility/move.hpp"

#include <system_error>
#include <tuple>

// ----------------------------------------------------------------------------

namespace nstd::net::inline customization_points {
    inline constexpr struct async_send_t {
        template <typename Socket, typename CBS> struct io_operation;

        template <typename Socket, typename CBS, ::nstd::execution::sender Sender>
        friend auto tag_invoke(async_send_t, Socket& socket, CBS const& cbs, ::nstd::hidden_names::message_flags flags, Sender sndr) {
            auto scheduler{::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sndr)};
            return nstd::net::async_io_sender<decltype(scheduler), Sender, io_operation<Socket, CBS>>{
                scheduler,
                ::nstd::utility::move(sndr),
                socket,
                cbs,
                flags
                };
        }
        template <::nstd::execution::sender Sender, typename Socket, typename CBS>
        auto operator()(Sender sender, Socket& socket, CBS const& cbs, ::nstd::hidden_names::message_flags flags) const {
            return ::nstd::tag_invoke(*this, socket, cbs, flags, sender);
        }
        template <typename Socket, typename CBS>
        auto operator()(Socket& socket, CBS const& cbs, ::nstd::hidden_names::message_flags flags) const {
            return ::nstd::execution::sender_adaptor_closure<async_send_t>()(
                ::std::ref(socket), cbs, flags
                );
        }
        template <::nstd::execution::sender Sender, typename Socket, typename CBS>
        auto operator()(Sender sender, Socket& socket, CBS const& cbs) const {
            return ::nstd::tag_invoke(*this, socket, cbs, ::nstd::hidden_names::message_flags(), sender);
        }
        template <typename Socket, typename CBS>
        auto operator()(Socket& socket, CBS const& cbs) const {
            return ::nstd::execution::sender_adaptor_closure<async_send_t>()(
                ::std::ref(socket), cbs, ::nstd::hidden_names::message_flags()
                );
        }
    } async_send;
}

// ----------------------------------------------------------------------------

template <typename Socket, typename CBS>
struct nstd::net::customization_points::async_send_t::io_operation
{
    using completion_signatures
        = ::nstd::execution::completion_signatures<
            ::nstd::execution::set_value_t(int)
            >;
    template <template <typename...> class T, template <typename...> class V>
    using value_types = V<T<int>>;
    template <template <typename...> class V>
    using error_types = V<::std::exception_ptr>;

    using socket_type    = Socket;
    using operation_type = ::nstd::file::operation_send<CBS>;
    using parameter_type = ::std::tuple<CBS, ::nstd::hidden_names::message_flags>;

    socket_type& d_socket;

    io_operation(socket_type& socket): d_socket(socket) {}

    auto enqueue(operation_type* cont) -> void {
        this->d_socket.enqueue(*cont);
    }
    template <::nstd::execution::scheduler Scheduler>
    auto submit(Scheduler&& scheduler, operation_type* cont) -> void {
        iovec const* iov = reinterpret_cast<::iovec const*>(&*::nstd::net::buffer_sequence_begin(cont->d_buffers));
        cont->d_msgheader.msg_iov = const_cast<::iovec*>(iov);
        cont->d_msgheader.msg_iovlen = 1u;
        scheduler.sendmsg(this->d_socket.native_handle(), &cont->d_msgheader, static_cast<int>(cont->d_flags), cont);
    }

    template <typename Receiver>
    auto complete(::std::int32_t rc, std::uint32_t, operation_type* cont, Receiver& receiver) {
        this->d_socket.complete(*cont);
        ::nstd::execution::set_value(::nstd::utility::move(receiver), rc);
    }
};

// ----------------------------------------------------------------------------

#endif
