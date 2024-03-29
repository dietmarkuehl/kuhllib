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

#include "nstd/file/async_io.hpp"
#include "nstd/file/operation.hpp"
#include "nstd/net/socket.hpp"
#include "nstd/net/get_iovec.hpp"
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

namespace nstd::hidden_names::async_receive_from {
    template <::nstd::net::socket Socket, typename Env>
    struct operation {
        using completion_signature = ::nstd::execution::set_value_t(::std::size_t, typename Socket::endpoint_type);
        
        template <typename IOVector>
        struct state {
            IOVector           d_iovec;
            ::sockaddr_storage d_address;
            ::msghdr           d_msg{};
            int                d_flags;

            template <typename Buffers>
            state(Env const& env, Buffers&& buffers, ::nstd::hidden_names::message_flags flags)
                : d_iovec(::nstd::net::get_iovec(env, ::nstd::utility::forward<Buffers>(buffers)))
                , d_flags(static_cast<int>(flags))
            {
                this->d_msg.msg_iov     = this->d_iovec.data();
                this->d_msg.msg_iovlen  = this->d_iovec.size();
                this->d_msg.msg_name    = &this->d_address;
                this->d_msg.msg_namelen = sizeof(this->d_address);
            }
            auto start(
                Socket& socket,
                auto&& scheduler,
                ::nstd::file::io_base* cont) noexcept -> void
            {
                scheduler.recvmsg(socket.native_handle(), &this->d_msg, this->d_flags, cont);
            }
            template <typename Receiver>
            auto complete(::std::int32_t n, ::std::uint32_t, Receiver& receiver) noexcept -> void {
                typename Socket::endpoint_type endpoint;
                endpoint.set_address(&this->d_address, this->d_msg.msg_namelen);
                ::nstd::execution::set_value(::nstd::utility::move(receiver), ::std::size_t(n), endpoint);
            }
        };
        template <typename Buffers>
        static auto connect(Env const& env, Buffers&& buffers)
        {
            using iovec_t = decltype(::nstd::net::get_iovec(env, ::nstd::utility::forward<Buffers>(buffers)));
            return state<iovec_t>(env, ::nstd::utility::forward<Buffers>(buffers), ::nstd::hidden_names::message_flags{});
        }
        template <typename Buffers>
        static auto connect(Env const& env, Buffers&& buffers, ::nstd::hidden_names::message_flags flags)
        {
            using iovec_t = decltype(::nstd::net::get_iovec(env, ::nstd::utility::forward<Buffers>(buffers)));
            return state<iovec_t>(env, ::nstd::utility::forward<Buffers>(buffers), flags);
        }
    };
}

namespace nstd::net::inline customization_points {
    using async_receive_from_t =
        ::nstd::hidden_names::async_io::cpo<
            ::nstd::hidden_names::async_receive_from::operation
        >;
    inline constexpr async_receive_from_t async_receive_from_adapter{};

    template <::nstd::net::socket Socket, typename Buffers>
    inline auto async_receive_from(Socket&& socket, Buffers&& buffers)  {
        return ::nstd::net::async_receive_from_adapter(
            ::nstd::execution::just(::nstd::utility::forward<Buffers>(buffers)),
            socket);
    }
    template <::nstd::net::socket Socket, typename Buffers>
    inline auto async_receive_from(Socket&& socket, Buffers&& buffers, ::nstd::hidden_names::message_flags flags)  {
        return ::nstd::net::async_receive_from_adapter(
            ::nstd::execution::just(::nstd::utility::forward<Buffers>(buffers), flags),
            socket);
    }
}

// ----------------------------------------------------------------------------

#endif
