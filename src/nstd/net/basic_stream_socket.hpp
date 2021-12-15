// nstd/net/basic_stream_socket.hpp                                   -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_BASIC_STREAM_SOCKET
#define INCLUDED_NSTD_NET_BASIC_STREAM_SOCKET

#include "nstd/net/netfwd.hpp"
#include "nstd/net/basic_socket.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_done.hpp"
#include "nstd/execution/repeat_effect_until.hpp"
#include "nstd/execution/let_value.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/buffer/const_buffer.hpp"
#include "nstd/buffer/mutable_buffer.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include <sys/socket.h>

// ----------------------------------------------------------------------------

namespace nstd::net {
    template <typename> class basic_stream_socket;

    inline constexpr struct async_read_some_t {
        template <typename R, typename Scheduler, typename MBS>
        struct state;

        template <typename Scheduler, typename MBS>
        struct sender;

        template <typename P, typename Scheduler, typename MBS>
        auto operator()(basic_stream_socket<P>&, Scheduler, MBS const&) const
            -> sender<Scheduler, MBS>;
    } async_read_some;

    inline constexpr struct async_write_some_t {
        template <typename R, typename Scheduler, typename CBS>
        struct state;

        template <typename Scheduler, typename CBS>
        struct sender;

        template <typename Scheduler, typename P, typename CBS>
        auto operator()(Scheduler, basic_stream_socket<P>&, CBS const&) const
            -> sender<Scheduler, CBS>;
    } async_write_some;

    inline constexpr struct async_write_t {
        template <::nstd::execution::sender Sender, typename P, typename CBS>
        auto operator()(Sender&&, basic_stream_socket<P>&, CBS const&) const;

        template <typename P, typename CBS>
        auto operator()(basic_stream_socket<P>& socket, CBS const& buffer) const;
    } async_write;
}

// ----------------------------------------------------------------------------

template <typename Protocol>
class nstd::net::basic_stream_socket
    : public ::nstd::net::basic_socket<Protocol>
{
public:
    using protocol_type = Protocol;
    using endpoint_type = typename protocol_type::endpoint;
    using native_handle_type = typename ::nstd::net::basic_socket<Protocol>::native_handle_type;

    basic_stream_socket();
    explicit basic_stream_socket(protocol_type const&);
    explicit basic_stream_socket(endpoint_type const&);
    basic_stream_socket(protocol_type const&, native_handle_type const&);
    basic_stream_socket(basic_stream_socket const&) = delete;
    basic_stream_socket(basic_stream_socket &&) = default;
    auto operator=(basic_stream_socket const&) -> basic_stream_socket& = delete;
    auto operator=(basic_stream_socket &&) -> basic_stream_socket& = default;
};

// ----------------------------------------------------------------------------

template <typename Protocol>
nstd::net::basic_stream_socket<Protocol>::basic_stream_socket()
    : ::nstd::net::basic_socket<Protocol>()
{
}

template <typename Protocol>
nstd::net::basic_stream_socket<Protocol>::basic_stream_socket(protocol_type const& protocol)
    : ::nstd::net::basic_socket<Protocol>(protocol)
{
}

template <typename Protocol>
nstd::net::basic_stream_socket<Protocol>::basic_stream_socket(protocol_type const& protocol,
                                                              native_handle_type const& fd)
    : ::nstd::net::basic_socket<Protocol>(protocol, fd)
{
}

// ----------------------------------------------------------------------------

template <typename Receiver, typename Scheduler, typename MBS>
struct nstd::net::async_read_some_t::state
    : public ::nstd::file::context::io_base
{
    ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
    Scheduler                                     d_scheduler;
    MBS                                           d_buffers;
    ::nstd::net::socket_base::native_handle_type  d_handle;
    ::msghdr                                      d_msg;

    template <typename R>
    state(R&& r,
          Scheduler scheduler,
          MBS const& buffers,
          ::nstd::net::socket_base::native_handle_type handle)
        : d_receiver(nstd::utility::forward<R>(r))
        , d_scheduler(scheduler)
        , d_buffers(buffers)
        , d_handle(handle)
        , d_msg()
    {
    }
    friend auto tag_invoke(::nstd::execution::start_t, state& s)
        noexcept -> void
    {
        iovec const* iov = reinterpret_cast<::iovec const*>(&*::nstd::net::buffer_sequence_begin(s.d_buffers));
        s.d_msg.msg_iov = const_cast<::iovec*>(iov);
        s.d_msg.msg_iovlen = 1u;
        s.d_scheduler.context()->hidden_context()->recvmsg(s.d_handle, &s.d_msg, int(), &s);
    }

    auto do_result(::std::int32_t size, ::std::uint32_t) -> void override
    {
        ::nstd::execution::set_value(::nstd::utility::move(this->d_receiver), size);
    }
};

template <typename Scheduler, typename MBS>
struct nstd::net::async_read_some_t::sender
{
    template <template <typename...> class T, template <typename...> class V>
    using value_types = V<T<int>>;
    template <template <typename...> class V>
    using error_types = V<::std::exception_ptr>;
    static constexpr bool sends_done = true;

    Scheduler                                    d_scheduler;
    MBS                                          d_buffers;
    ::nstd::net::socket_base::native_handle_type d_handle;

    template <typename Receiver>
    friend auto tag_invoke(::nstd::execution::connect_t, sender&& s, Receiver&& r)
        noexcept -> ::nstd::net::async_read_some_t::state<Receiver, Scheduler, MBS>
    {
        using result = ::nstd::net::async_read_some_t::state<Receiver, Scheduler, MBS>;
        return result(::nstd::utility::forward<Receiver>(r),
                      ::nstd::utility::move(s.d_scheduler),
                      ::nstd::utility::move(s.d_buffers),
                      ::nstd::utility::move(s.d_handle));
    }
    template <typename Receiver>
    friend auto tag_invoke(::nstd::execution::connect_t, sender const& s, Receiver&& r)
        noexcept -> ::nstd::net::async_read_some_t::state<Receiver, Scheduler, MBS>
    {
        using result = ::nstd::net::async_read_some_t::state<Receiver, Scheduler, MBS>;
        return result(::nstd::utility::forward<Receiver>(r),
                      s.d_scheduler,
                      s.d_buffers,
                      s.d_handle);
    }
};

template <typename P, typename Scheduler, typename MBS>
auto nstd::net::async_read_some_t::operator()(basic_stream_socket<P>& socket,
                                              Scheduler scheduler,
                                              MBS const& mbs) const
    -> sender<Scheduler, MBS>
{
    return sender<Scheduler, MBS>{scheduler, mbs, socket.native_handle()};
}

// ----------------------------------------------------------------------------

template <typename Receiver, typename Scheduler, typename CBS>
struct nstd::net::async_write_some_t::state
    : public ::nstd::file::context::io_base
{
    ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
    Scheduler                                     d_scheduler;
    CBS                                           d_buffers;
    ::nstd::net::socket_base::native_handle_type  d_handle;
    ::msghdr                                      d_msg;

    template <typename R>
    state(R&& r,
          Scheduler scheduler,
          CBS const& buffers,
          ::nstd::net::socket_base::native_handle_type handle)
        : d_receiver(nstd::utility::forward<R>(r))
        , d_scheduler(scheduler)
        , d_buffers(buffers)
        , d_handle(handle)
        , d_msg()
    {
    }
    friend auto tag_invoke(::nstd::execution::start_t, state& s)
        noexcept -> void
    {
        iovec const* iov = reinterpret_cast<::iovec const*>(&*::nstd::net::buffer_sequence_begin(s.d_buffers));
        s.d_msg.msg_iov = const_cast<::iovec*>(iov);
        s.d_msg.msg_iovlen = 1u;
        s.d_scheduler.context()->hidden_context()->sendmsg(s.d_handle, &s.d_msg, int(), &s);
    }

    auto do_result(::std::int32_t size, ::std::uint32_t) -> void override
    {
        ::nstd::execution::set_value(::nstd::utility::move(this->d_receiver), size);
    }
};

template <typename Scheduler, typename CBS>
struct nstd::net::async_write_some_t::sender
{
    template <template <typename...> class T, template <typename...> class V>
    using value_types = V<T<int>>;
    template <template <typename...> class V>
    using error_types = V<::std::exception_ptr>;
    static constexpr bool sends_done = true;

    Scheduler                                    d_scheduler;
    CBS                                          d_buffers;
    ::nstd::net::socket_base::native_handle_type d_handle;

    template <typename Receiver>
    friend auto tag_invoke(::nstd::execution::connect_t, sender&& s, Receiver&& r)
        noexcept -> ::nstd::net::async_write_some_t::state<Receiver, Scheduler, CBS>
    {
        using result = ::nstd::net::async_write_some_t::state<Receiver, Scheduler, CBS>;
        return result(::nstd::utility::forward<Receiver>(r),
                      s.d_scheduler,
                      s.d_buffers,
                      s.d_handle);
    }
};

template <typename Scheduler, typename P, typename CBS>
auto nstd::net::async_write_some_t::operator()(
                                               Scheduler scheduler,
                                               basic_stream_socket<P>& socket,
                                               CBS const& cbs) const
    -> sender<Scheduler, CBS>
{
    return sender<Scheduler, CBS>{scheduler, cbs, socket.native_handle()};
}

// ----------------------------------------------------------------------------

template <::nstd::execution::sender Sender, typename P, typename CBS>
auto nstd::net::async_write_t::operator()(Sender&& sender,
                                          basic_stream_socket<P>& socket,
                                          CBS const& cbs) const
{
    auto scheduler = ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sender);
    return ::nstd::execution::let_value(::nstd::utility::forward<Sender>(sender),
        [scheduler, &socket, buffer = cbs]() mutable {
            return ::nstd::execution::repeat_effect_until(
                ::nstd::execution::let_value(::nstd::execution::just(),
                    [scheduler, &socket, &buffer]{
                        return ::nstd::net::async_write_some(scheduler, socket, buffer)
                            |  ::nstd::execution::then([&buffer](::std::size_t n){
                                buffer += n;
                            });
                    }),
                [&buffer]{ return buffer.size() == 0; }
            );
        });
}

template <typename P, typename CBS>
auto nstd::net::async_write_t::operator()(::nstd::net::basic_stream_socket<P>& socket,
                                          CBS const& buffer) const
{
    return [this, &socket, buffer](::nstd::execution::sender auto&& sender) {
        return this->operator()(sender, socket, buffer);
    };
}

// ----------------------------------------------------------------------------

#endif
