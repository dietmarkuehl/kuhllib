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
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/buffer/const_buffer.hpp"
#include "nstd/buffer/sequence.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/net/io_context.hpp"

#include <system_error>

// ----------------------------------------------------------------------------

namespace nstd::net::hidden_names::async_send {
    template <typename Socket, typename CBS> struct operation;
    struct cpo;
}

// ----------------------------------------------------------------------------

template <typename Socket, typename CBS>
struct nstd::net::hidden_names::async_send::operation {
    using completion_signature
        = ::nstd::execution::set_value_t(int);

    typename Socket::native_handle_type      d_handle;
    ::nstd::type_traits::remove_cvref_t<CBS> d_buffers; //-dk:TODO add allocator support?
    ::nstd::hidden_names::message_flags      d_flags;
    ::msghdr                                 d_msgheader{};
    struct state {
    };
    auto start(::nstd::net::io_context::scheduler_type scheduler, state&, ::nstd::file::context::io_base* cont) -> void{
        ::iovec const* iov = reinterpret_cast<::iovec const*>(&*::nstd::net::buffer_sequence_begin(this->d_buffers));
        this->d_msgheader.msg_iov = const_cast<::iovec*>(iov);
        // this->d_msgheader.msg_iovlen = 1u;
        this->d_msgheader.msg_iovlen = ::std::distance(::nstd::net::buffer_sequence_begin(this->d_buffers),
                                                       ::nstd::net::buffer_sequence_end(this->d_buffers));
        scheduler.sendmsg(this->d_handle, &this->d_msgheader, static_cast<int>(this->d_flags), cont);
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
            ::nstd::execution::set_value(::nstd::utility::move(receiver), rc);
        }
    }
};

// ----------------------------------------------------------------------------

struct nstd::net::hidden_names::async_send::cpo {
    template <typename Socket, typename CBS>
    friend auto tag_invoke(cpo, Socket& socket, CBS&& cbs, ::nstd::hidden_names::message_flags flags) {
        return nstd::net::hidden_names::async_io_sender<::nstd::net::hidden_names::async_send::operation<Socket, CBS>>(
            socket.native_handle(), ::nstd::utility::forward<CBS>(cbs), flags
            );
    }
    template <typename Socket, typename CBS>
    auto operator()(Socket& socket, CBS&& cbs, ::nstd::hidden_names::message_flags flags) const {
        return ::nstd::tag_invoke(*this, socket, ::nstd::utility::forward<CBS>(cbs), flags);
    }
    template <typename Socket, typename CBS>
    auto operator()(Socket& socket, CBS&& cbs) const {
        return ::nstd::tag_invoke(*this, socket, ::nstd::utility::forward<CBS>(cbs), ::nstd::hidden_names::message_flags());
    }
};

// ----------------------------------------------------------------------------

namespace nstd::net {
    using async_send_t = ::nstd::net::hidden_names::async_send::cpo;
    inline constexpr async_send_t async_send;
}

// ----------------------------------------------------------------------------

#endif
