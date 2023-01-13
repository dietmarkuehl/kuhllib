// nstd/net/async_read_some.hpp                                       -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_ASYNC_READ_SOME
#define INCLUDED_NSTD_NET_ASYNC_READ_SOME

#include "nstd/file/async_io.hpp"
#include "nstd/net/get_iovec.hpp"
#include "nstd/net/socket.hpp"
#include "nstd/net/io_context.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/buffer/mutable_buffer.hpp"
#include "nstd/utility/move.hpp"

#include <system_error>

// ----------------------------------------------------------------------------

namespace nstd::net::hidden_names::async_read_some {
    template <typename Socket, typename MBS> struct socket_operation;
    template <typename Socket, typename MBS> struct stream_operation;
    struct cpo;
}

// ----------------------------------------------------------------------------

template <typename Socket, typename MBS>
struct nstd::net::hidden_names::async_read_some::socket_operation {
    using completion_signature = ::nstd::execution::set_value_t(int);
    using buffer_sequence = ::nstd::type_traits::remove_cvref_t<MBS>;
    template <typename Env>
    using iovec = decltype(::nstd::net::get_iovec(::nstd::type_traits::declval<Env>(), ::nstd::type_traits::declval<buffer_sequence>()));

    typename Socket::native_handle_type  d_handle;
    buffer_sequence                      d_buffer;
    template <typename Env>
    struct state {
        iovec<Env> d_iovec;
        ::msghdr   d_msg{};
        state(Env const& env, buffer_sequence const& buffer)
            : d_iovec(::nstd::net::get_iovec(env, buffer)) {
            this->d_msg.msg_iov    = this->d_iovec.data();
            this->d_msg.msg_iovlen = this->d_iovec.size();
        }
    };
    template <typename Env>
    auto connect(Env const& env) -> state<Env> {
        return state<Env>(env, this->d_buffer);
    }
    template <typename Env>
    auto start(::nstd::net::io_context::scheduler_type scheduler, state<Env>& s, ::nstd::file::context::io_base* cont) -> void{
        scheduler.recvmsg(this->d_handle, &s.d_msg, int(), cont);
    }
    template <::nstd::execution::receiver Receiver, typename Env>
    auto complete(int32_t rc, uint32_t, bool cancelled, state<Env>&, Receiver& receiver) -> void {
        if (cancelled) {
            ::nstd::execution::set_stopped(::nstd::utility::move(receiver));
        }
        else if (rc < 0) {
            ::nstd::execution::set_error(::nstd::utility::move(receiver), ::std::error_code(-rc, std::system_category()));
        }
        else {
            ::nstd::execution::set_value(::nstd::utility::move(receiver), rc);
        }
    }
};

// ----------------------------------------------------------------------------

template <typename Stream, typename MBS>
struct nstd::net::hidden_names::async_read_some::stream_operation {
    using completion_signature = ::nstd::execution::set_value_t(int);
    using buffer_sequence = ::nstd::type_traits::remove_cvref_t<MBS>;
    template <typename Env>
    using iovec = decltype(::nstd::net::get_iovec(::nstd::type_traits::declval<Env>(), ::nstd::type_traits::declval<buffer_sequence>()));

    typename Stream::native_handle_type d_handle;
    buffer_sequence                     d_buffer;

    template <typename Env>
    struct state {
        iovec<Env> d_iovec;
    };
    template <typename Env>
    auto connect(Env const& env) -> state<Env> {
        return { ::nstd::net::get_iovec(env, this->d_buffer) };
    }
    template <typename Env>
    auto start(::nstd::net::io_context::scheduler_type scheduler, state<Env>& s, ::nstd::file::context::io_base* cont) -> void{
        scheduler.read(this->d_handle, s.d_iovec.data(), s.d_iovec.size(), cont);
    }
    template <::nstd::execution::receiver Receiver, typename Env>
    auto complete(int32_t rc, uint32_t, bool cancelled, state<Env>&, Receiver& receiver) -> void {
        if (cancelled) {
            ::nstd::execution::set_stopped(::nstd::utility::move(receiver));
        }
        else if (rc < 0) {
            ::nstd::execution::set_error(::nstd::utility::move(receiver), ::std::error_code(-rc, std::system_category()));
        }
        else {
            ::nstd::execution::set_value(::nstd::utility::move(receiver), rc);
        }
    }
};

// ----------------------------------------------------------------------------

struct nstd::net::hidden_names::async_read_some::cpo {
    template <::nstd::net::socket Socket, typename MBS>
    friend auto tag_invoke(cpo, Socket& socket, MBS const& buffer) {
        return nstd::file::hidden_names::async_io_sender<::nstd::net::hidden_names::async_read_some::socket_operation<Socket, MBS>>(
            socket.native_handle(), buffer
            );
    }
    template <typename Stream, typename MBS>
    friend auto tag_invoke(cpo, Stream& socket, MBS const& buffer) {
        return nstd::file::hidden_names::async_io_sender<::nstd::net::hidden_names::async_read_some::stream_operation<Stream, MBS>>(
            socket.native_handle(), buffer
            );
    }
    template <typename Socket, typename MBS>
    auto operator()(Socket& socket, MBS const& buffer) const {
        return ::nstd::tag_invoke(*this, socket, buffer);
    }
};

// ----------------------------------------------------------------------------

namespace nstd::net::inline customization_points {
    using async_read_some_t = ::nstd::net::hidden_names::async_read_some::cpo;
    inline constexpr async_read_some_t async_read_some;
}

// ----------------------------------------------------------------------------

#endif
