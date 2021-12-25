// nstd/net/async_write_some.hpp                                      -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_ASYNC_WRITE_SOME
#define INCLUDED_NSTD_NET_ASYNC_WRITE_SOME

#include "nstd/net/async_io.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/buffer/const_buffer.hpp"
#include "nstd/utility/move.hpp"

#include <system_error>

// ----------------------------------------------------------------------------

namespace nstd::net {
    inline constexpr struct async_write_some_t {
        template <typename Socket, typename CBS> struct io_operation;

        template <typename Socket, typename CBS, ::nstd::execution::sender Sender>
        friend auto tag_invoke(async_write_some_t, Socket& socket, CBS const& cbs, Sender sndr) {
            auto scheduler{::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sndr)};
            return nstd::net::async_io_sender<decltype(scheduler), Sender, io_operation<Socket, CBS>>{
                scheduler,
                ::nstd::utility::move(sndr),
                socket.native_handle(),
                cbs
                };
        }
        template <::nstd::execution::sender Sender, typename Socket, typename CBS>
        auto operator()(Sender sender, Socket& socket, CBS const& cbs) const {
            return ::nstd::tag_invoke(*this, socket, cbs, sender);
        }
        template <typename Socket, typename CBS>
        auto operator()(Socket& socket, CBS const& cbs) const {
            return [&socket, cbs, this](::nstd::execution::sender auto sender){
                return ::nstd::tag_invoke(*this, socket, cbs, sender);
                };
        }
    } async_write_some;
}

// ----------------------------------------------------------------------------

template <typename Socket, typename CBS>
struct nstd::net::async_write_some_t::io_operation
{
    template <template <typename...> class T, template <typename...> class V>
    using value_types = V<T<int>>;
    template <template <typename...> class V>
    using error_types = V<::std::exception_ptr>;

    struct parameters {
        typename Socket::native_handle_type  d_fd;
        CBS                                  d_buffer;
    };

    parameters d_parameters;
    ::msghdr   d_msg{};

    io_operation(::nstd::net::async_write_some_t::io_operation<Socket, CBS>::parameters const& parameters)
        : d_parameters(parameters)
    {
    }

    template <::nstd::execution::scheduler Scheduler>
    auto submit(Scheduler&& scheduler, ::nstd::file::context::io_base* cont) -> void {
        iovec const* iov = reinterpret_cast<::iovec const*>(&*::nstd::net::buffer_sequence_begin(this->d_parameters.d_buffer));
        this->d_msg.msg_iov = const_cast<::iovec*>(iov);
        this->d_msg.msg_iovlen = 1u;
        scheduler.sendmsg(this->d_parameters.d_fd, &this->d_msg, int(), cont);
    }

    template <typename Receiver>
    auto complete(::std::int32_t rc, std::uint32_t, Receiver& receiver) {
        ::nstd::execution::set_value(::nstd::utility::move(receiver), rc);
    }
};

// ----------------------------------------------------------------------------

#endif
