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
#include "nstd/file/io_object.hpp"
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

namespace nstd::hidden_names::async_read_some {
    template <::nstd::file::io_object IOObject, typename Env>
    struct operation {
        using completion_signature = ::nstd::execution::set_value_t(::std::size_t);
        
        template <typename IOVector>
        struct state {
            IOVector d_iovec;
            ::msghdr d_msg{}; //-dk:TODO don't build a msghdr for non-sockets

            template <typename Buffers>
            state(Env const& env, Buffers&& buffers)
                : d_iovec(::nstd::net::get_iovec(env, ::nstd::utility::forward<Buffers>(buffers)))
            {
                this->d_msg.msg_iov    = this->d_iovec.data();
                this->d_msg.msg_iovlen = this->d_iovec.size();
            }
            auto start(
                ::nstd::file::io_object auto& object,
                auto&& scheduler,
                ::nstd::file::io_base* cont) noexcept -> void
            {
                scheduler.read(object.native_handle(), this->d_iovec.data(), this->d_iovec.size(), cont);
            }
            auto start(
                ::nstd::net::socket auto& socket,
                auto&& scheduler,
                ::nstd::file::io_base* cont) noexcept -> void
            {
                scheduler.recvmsg(socket.native_handle(), &this->d_msg, int(), cont);
            }
            template <typename Receiver>
            auto complete(::std::int32_t n, ::std::uint32_t, Receiver& receiver) noexcept -> void {
                ::nstd::execution::set_value(::nstd::utility::move(receiver), ::std::size_t(n));
            }
        };
        template <typename Buffers>
        static auto connect(Env const& env, Buffers&& buffers)
        {
            using iovec_t = decltype(::nstd::net::get_iovec(env, ::nstd::utility::forward<Buffers>(buffers)));
            return state<iovec_t>(env, ::nstd::utility::forward<Buffers>(buffers));
        }
    };
}

namespace nstd::net::inline customization_points {
    using async_read_some_t =
        ::nstd::hidden_names::async_io::cpo<
            ::nstd::hidden_names::async_read_some::operation
        >;
    inline constexpr async_read_some_t async_read_some_adapter{};

    template <::nstd::file::io_object IOObject, typename Buffers>
    inline auto async_read_some(IOObject&& io_object, Buffers&& buffers)  {
        return ::nstd::net::async_read_some_adapter(
            ::nstd::execution::just(::nstd::utility::forward<Buffers>(buffers)),
            io_object);
    }
}

// ----------------------------------------------------------------------------

#endif
