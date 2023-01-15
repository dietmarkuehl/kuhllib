// nstd/net/async_send_to.hpp                                         -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_ASYNC_SEND_TO
#define INCLUDED_NSTD_NET_ASYNC_SEND_TO

#include "nstd/file/async_io.hpp"
#include "nstd/file/operation.hpp"
#include "nstd/net/socket.hpp"
#include "nstd/net/get_iovec.hpp"
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

namespace nstd::hidden_names::async_send_to {
    template <::nstd::net::socket Socket, typename Env>
    struct operation {
        using completion_signature = ::nstd::execution::set_value_t(::std::size_t);
        
        template <typename IOVector>
        struct state {
            IOVector           d_iovec;
            ::sockaddr_storage d_address;
            ::msghdr           d_msg{};
            int                d_flags;

            template <typename Buffers>
            state(Env const& env, Buffers&& buffers, ::nstd::hidden_names::message_flags flags, typename Socket::endpoint_type const& endpoint)
                : d_iovec(::nstd::net::get_iovec(env, ::nstd::utility::forward<Buffers>(buffers)))
                , d_flags(static_cast<int>(flags))
            {
                this->d_msg.msg_iov     = this->d_iovec.data();
                this->d_msg.msg_iovlen  = this->d_iovec.size();
                this->d_msg.msg_name    = &this->d_address;
                this->d_msg.msg_namelen = endpoint.get_address(&this->d_address);
            }
            auto start(
                Socket& socket,
                auto&& scheduler,
                ::nstd::file::io_base* cont) noexcept -> void
            {
                scheduler.sendmsg(socket.native_handle(), &this->d_msg, this->d_flags, cont);
            }
            template <typename Receiver>
            auto complete(::std::int32_t n, ::std::uint32_t, Receiver& receiver) noexcept -> void {
                ::nstd::execution::set_value(::nstd::utility::move(receiver), ::std::size_t(n));
            }
        };
        template <typename Buffers>
        static auto connect(Env const& env, Buffers&& buffers, typename Socket::endpoint_type const& endpoint)
        {
            using iovec_t = decltype(::nstd::net::get_iovec(env, ::nstd::utility::forward<Buffers>(buffers)));
            return state<iovec_t>(env, ::nstd::utility::forward<Buffers>(buffers), ::nstd::hidden_names::message_flags{}, endpoint);
        }
        template <typename Buffers>
        static auto connect(Env const& env, Buffers&& buffers, ::nstd::hidden_names::message_flags flags, typename Socket::endpoint_type const& endpoint)
        {
            using iovec_t = decltype(::nstd::net::get_iovec(env, ::nstd::utility::forward<Buffers>(buffers)));
            return state<iovec_t>(env, ::nstd::utility::forward<Buffers>(buffers), flags, endpoint);
        }
    };
}

namespace nstd::net::inline customization_points {
    using async_send_to_t =
        ::nstd::hidden_names::async_io::cpo<
            ::nstd::hidden_names::async_send_to::operation
        >;
    inline constexpr async_send_to_t async_send_to_adapter{};

    template <::nstd::net::socket Socket, typename Buffers>
    inline auto async_send_to(
        Socket&& socket,
        Buffers&& buffers,
        typename ::nstd::type_traits::remove_cvref_t<Socket>::endpoint_type const& endpoint)  {
        return ::nstd::net::async_send_to_adapter(
            ::nstd::execution::just(::nstd::utility::forward<Buffers>(buffers), endpoint),
            socket);
    }
    template <::nstd::net::socket Socket, typename Buffers>
    inline auto async_send_to(
        Socket&& socket,
        ::nstd::hidden_names::message_flags flags,
        Buffers&& buffers,
        typename ::nstd::type_traits::remove_cvref_t<Socket>::endpoint_type const& endpoint)  {
        return ::nstd::net::async_send_to_adapter(
            ::nstd::execution::just(::nstd::utility::forward<Buffers>(buffers), flags, endpoint),
            socket);
    }
}

// ----------------------------------------------------------------------------

#endif
