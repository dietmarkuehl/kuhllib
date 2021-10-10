// nstd/net/basic_socket.hpp                                          -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_BASIC_SOCKET
#define INCLUDED_NSTD_NET_BASIC_SOCKET

#include "nstd/net/netfwd.hpp"
#include "nstd/net/socket_base.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/file/context.hpp"
#include <optional>
#include <system_error>
#include <tuple>
#include <variant>

// ----------------------------------------------------------------------------

namespace nstd::net {
    template <typename> class basic_socket;

    inline constexpr struct async_connect_t {
        template <::nstd::execution::receiver Receiver> struct state;
        template <typename Endpoint> struct sender;

        template <typename Socket, typename Scheduler>
        auto operator()(Socket&, typename Socket::endpoint_type const&, Scheduler) const
            -> sender<typename Socket::endpoint_type>;
    } async_connect;
}

// ----------------------------------------------------------------------------

template <typename Protocol>
class nstd::net::basic_socket
    : public ::nstd::net::socket_base
{
public:
    using native_handle_type = int;
    using protocol_type = Protocol;
    using endpoint_type = typename protocol_type::endpoint;

private:
    ::std::optional<protocol_type> d_protocol;

public: //-dk:TODO protocted:
    basic_socket();
    basic_socket(protocol_type const&);
    basic_socket(protocol_type const&, native_handle_type const&);
    basic_socket(basic_socket const&) = delete;
    basic_socket(basic_socket&&) = default;
    auto operator=(basic_socket const &) -> basic_socket & = delete;
    auto operator= (basic_socket&&) -> basic_socket& = default;

public:
    auto is_open() const noexcept -> bool { return this->::nstd::net::socket_base::is_open(); }
    auto non_blocking() const noexcept -> bool { return false; }
    auto protocol() const noexcept -> protocol_type { return *this->d_protocol; }

    auto open(protocol_type const&) -> void;
};

// ----------------------------------------------------------------------------

template <typename Protocol>
nstd::net::basic_socket<Protocol>::basic_socket()
{
}

template <typename Protocol>
nstd::net::basic_socket<Protocol>::basic_socket(Protocol const& proto)
{
    this->open(proto);
}

template <typename Protocol>
nstd::net::basic_socket<Protocol>::basic_socket(Protocol const& proto,
                                                native_handle_type const& fd)
    : ::nstd::net::socket_base(fd)
    , d_protocol(proto)
{
}

// ----------------------------------------------------------------------------

template <typename Protocol>
auto nstd::net::basic_socket<Protocol>::open(Protocol const& proto)
    -> void
{
    this->d_protocol = proto;
    this->::nstd::net::socket_base::open(proto.family(),
                                         proto.type(),
                                         proto.protocol());
}

// ----------------------------------------------------------------------------

template <::nstd::execution::receiver Receiver>
struct nstd::net::async_connect_t::state
    : public ::nstd::file::context::io_base
{
    ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
    ::nstd::file::context*                        d_context;
    int                                           d_fd;
    ::sockaddr_storage                            d_address;
    ::socklen_t                                   d_length;

    template <::nstd::execution::receiver R, typename Endpoint>
    state(R&& receiver, ::nstd::file::context* context, int fd, Endpoint const& endpoint)
        : d_receiver(receiver)
        , d_context(context)
        , d_fd(fd)
    {
        this->d_length = endpoint.get_address(&this->d_address);
    }

    auto do_result(::std::int32_t rc, ::std::uint32_t)
        -> void override
    {
        ::nstd::execution::set_value(::nstd::utility::move(this->d_receiver),
                                     ::std::error_code(-rc, ::std::system_category()));
    }

    friend auto tag_invoke(::nstd::execution::start_t, state& s)
        noexcept -> void
    {
        s.d_context->connect(s.d_fd, reinterpret_cast<::sockaddr*>(&s.d_address), s.d_length, &s);
    }
};

template <typename Endpoint>
struct nstd::net::async_connect_t::sender
{
    template <template <typename...> class V, template <typename...> class T>
    using value_types = V<T<::std::error_code>>;
    template <template <typename...> class V>
    using error_types = V<::std::exception_ptr>;
    static constexpr bool sends_done = true;

    ::nstd::file::context*    d_context;
    int                       d_fd;
    Endpoint                  d_endpoint;

    template <::nstd::execution::receiver Receiver>
    friend auto tag_invoke(::nstd::execution::connect_t, sender const& s, Receiver&& receiver)
        noexcept -> ::nstd::net::async_connect_t::state<Receiver>
    {
        return ::nstd::net::async_connect_t::state<Receiver>(::nstd::utility::forward<Receiver>(receiver),
                                                              s.d_context,
                                                              s.d_fd,
                                                              s.d_endpoint);
    }
};

template <typename Socket, typename Scheduler>
auto nstd::net::async_connect_t::operator()(Socket& socket, typename Socket::endpoint_type const& ep, Scheduler scheduler) const
    -> ::nstd::net::async_connect_t::sender<typename Socket::endpoint_type>
{
    return { scheduler.context()->hidden_context(), socket.native_handle(), ep };
}

// ----------------------------------------------------------------------------

#endif
