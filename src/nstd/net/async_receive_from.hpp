// nstd/net/async_receive_from.hpp                                    -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2022 Dietmar Kuehl http://www.dietmar-kuehl.de
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

#ifndef INCLUDED_NSTD_NET_ASYNC_RECEIVE_FROM
#define INCLUDED_NSTD_NET_ASYNC_RECEIVE_FROM

#include "nstd/net/async_io.hpp"
#include "nstd/file/operation.hpp"
#include "nstd/hidden_names/message_flags.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/buffer/mutable_buffer.hpp"
#include "nstd/buffer/sequence.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/net/io_context.hpp"

#include <system_error>

// ----------------------------------------------------------------------------

namespace nstd::net::hidden_names::async_receive_from {
    template <typename Socket, typename MB> struct operation;
    struct cpo;
}

// ----------------------------------------------------------------------------

template <typename Socket, typename MB>
struct nstd::net::hidden_names::async_receive_from::operation {
    using completion_signature
        = ::nstd::execution::set_value_t(::std::size_t, typename Socket::endpoint_type);

    typename Socket::native_handle_type     d_handle;
    ::nstd::type_traits::remove_cvref_t<MB> d_buffer;
    ::nstd::hidden_names::message_flags     d_flags;
    struct state {
        ::sockaddr_storage d_address;
        ::socklen_t        d_addrlen{sizeof(::sockaddr_storage)};
    };
    auto start(::nstd::net::io_context::scheduler_type scheduler, state& s, ::nstd::file::context::io_base* cont) -> void{
        scheduler.recvfrom(this->d_handle, this->d_buffer.data(), this->d_buffer.size(), static_cast<int>(this->d_flags),
                           reinterpret_cast<::sockaddr*>(&s.d_address), &s.d_addrlen, cont);
    }
    template <::nstd::execution::receiver Receiver>
    auto complete(int32_t rc, uint32_t, bool cancelled, state& s, Receiver& receiver) -> void {
        if (cancelled) {
            ::nstd::execution::set_stopped(::nstd::utility::move(receiver));
        }
        else if (rc < 0) {
            ::nstd::execution::set_error(::nstd::utility::move(receiver), ::std::error_code(-rc, std::system_category()));
        }
        else {
            typename Socket::endpoint_type endpoint;
            endpoint.set_address(&s.d_address, s.d_addrlen);
            ::nstd::execution::set_value(::nstd::utility::move(receiver), rc, endpoint);
        }
    }
};

// ----------------------------------------------------------------------------

struct nstd::net::hidden_names::async_receive_from::cpo {
    template <typename Socket, typename MB>
    friend auto tag_invoke(cpo,
                           Socket&                             socket,
                           MB&&                                mb,
                           ::nstd::hidden_names::message_flags flags) {
        return nstd::net::hidden_names::async_io_sender<::nstd::net::hidden_names::async_receive_from::operation<Socket, MB>>(
            socket.native_handle(), ::nstd::utility::forward<MB>(mb), flags
            );
    }
    template <typename Socket, typename MB>
    auto operator()(Socket& socket, MB&& mb, ::nstd::hidden_names::message_flags flags) const {
        return ::nstd::tag_invoke(*this, socket, ::nstd::utility::forward<MB>(mb), flags);
    }
    template <typename Socket, typename MB>
    auto operator()(Socket& socket, MB&& mb) const {
        return ::nstd::tag_invoke(*this, socket, ::nstd::utility::forward<MB>(mb), ::nstd::hidden_names::message_flags());
    }
};

// ----------------------------------------------------------------------------

namespace nstd::net::inline customization_points {
    using async_receive_from_t = ::nstd::net::hidden_names::async_receive_from::cpo;
    inline constexpr async_receive_from_t async_receive_from;
}

// ----------------------------------------------------------------------------

#endif