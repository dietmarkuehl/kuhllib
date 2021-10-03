// nstd/net/basic_socket_acceptor.hpp                                 -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_BASIC_SOCKET_ACCEPTOR
#define INCLUDED_NSTD_NET_BASIC_SOCKET_ACCEPTOR

#include "nstd/net/netfwd.hpp"
#include "nstd/net/ip/tcp.hpp"
#include "nstd/net/io_context.hpp"
#include "nstd/net/socket_base.hpp"
#include "nstd/net/basic_stream_socket.hpp"
#include "nstd/file/descriptor.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/operation_state.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_done.hpp"
#include "nstd/execution/receiver_of.hpp"
#include "nstd/execution/sender.hpp"
#include <optional>
#include <tuple>
#include <variant>
#include <system_error>
#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <iostream> //-dk:TODO remove

// ----------------------------------------------------------------------------

namespace nstd::net {
    template <typename AcceptableProtocol>
    class basic_socket_acceptor;

    inline constexpr struct async_accept_t {
        template <typename Socket, ::nstd::execution::receiver_of<::std::error_code, Socket&&> Receiver> struct state;
        template <typename > struct sender;
        template <typename Acceptor, typename Scheduler>
        auto operator()(Acceptor&, Scheduler scheduler) const
            -> sender<typename Acceptor::socket_type>;
    } async_accept;
}

// ----------------------------------------------------------------------------

template <typename AcceptableProtocol>
class nstd::net::basic_socket_acceptor
    : public ::nstd::net::socket_base
{
public:
    using native_handle_type = int;
    using protocol_type      = AcceptableProtocol;
    using endpoint_type      = typename protocol_type::endpoint;
    using socket_type        = typename protocol_type::socket;

private:
    ::std::optional<protocol_type> d_protocol;
    ::nstd::file::descriptor       d_fd;

    static auto internal_open(protocol_type) -> ::nstd::file::descriptor;

public:
    basic_socket_acceptor();
    explicit basic_socket_acceptor(protocol_type const&);
    basic_socket_acceptor(endpoint_type const&, bool = true);
    basic_socket_acceptor(protocol_type const&, native_handle_type const&);
    basic_socket_acceptor(basic_socket_acceptor const&) = delete;
    basic_socket_acceptor(basic_socket_acceptor&&) = default;
    ~basic_socket_acceptor() = default;

    template <typename SettableSocketOption>
    auto set_option(SettableSocketOption const& option) -> void {
        ::setsockopt(this->native_handle(),
                     option.level(this->d_protocol),
                     option.name(this->d_protocol),
                     option.data(this->d_protocol),
                     option.size(this->d_protocol));
    }
    auto listen(int n = max_listen_connections) -> void;
    auto bind(endpoint_type const&) -> void;

    auto is_open() const -> bool { return bool(this->d_fd); }
    auto non_blocking() const -> bool { return false; }
    auto enable_connection_aborted() const -> bool { return false; }
    auto protocol() const -> protocol_type { return *this->d_protocol; }
    auto native_handle() const noexcept -> native_handle_type { return this->d_fd.get(); }
};

// ----------------------------------------------------------------------------

template <typename AcceptableProtocol>
auto
nstd::net::basic_socket_acceptor<AcceptableProtocol>
    ::internal_open(protocol_type protocol) -> ::nstd::file::descriptor
{
    return ::nstd::file::descriptor(::socket(protocol.family(),
                                             protocol.type(),
                                             protocol.protocol()));
}

template <typename AcceptableProtocol>
nstd::net::basic_socket_acceptor<AcceptableProtocol>::basic_socket_acceptor() = default;

template <typename AcceptableProtocol>
nstd::net::basic_socket_acceptor<AcceptableProtocol>::basic_socket_acceptor(
        protocol_type const& protocol)
    : d_protocol(protocol)
    , d_fd(internal_open(*this->d_protocol))
{
}

template <typename AcceptableProtocol>
nstd::net::basic_socket_acceptor<AcceptableProtocol>::basic_socket_acceptor(
    endpoint_type const& endpoint, bool reuse)
    : d_protocol(endpoint.protocol())
    , d_fd(internal_open(*this->d_protocol))
{
    if (reuse) {
        this->set_option(reuse_address(true));
    }
    this->bind(endpoint);
    this->listen();
}

// ----------------------------------------------------------------------------

template <typename AcceptableProtocol>
auto nstd::net::basic_socket_acceptor<AcceptableProtocol>::listen(int n) -> void
{
    if (int rc = ::listen(this->native_handle(), n)) {
        throw ::std::system_error(::std::error_code(errno, ::std::system_category()));
    }
}

template <typename AcceptableProtocol>
auto nstd::net::basic_socket_acceptor<AcceptableProtocol>::bind(endpoint_type const& endpoint) -> void
{
    ::sockaddr_storage storage;
    ::socklen_t        size = endpoint.address().get_address(&storage, endpoint.port());
    if (int rc = ::bind(this->native_handle(), reinterpret_cast<::sockaddr const*>(&storage), size)) {
        throw ::std::system_error(::std::error_code(errno, ::std::system_category()));
    }
}

// ----------------------------------------------------------------------------

template <typename Socket, ::nstd::execution::receiver_of<::std::error_code, Socket&&> Receiver>
struct nstd::net::async_accept_t::state
    : public ::nstd::file::ring_context::io_base
{
    ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
    ::nstd::file::ring_context*                   d_context;
    typename Socket::protocol_type                d_protocol;
    typename Socket::native_handle_type           d_fd;
    ::sockaddr_storage                            d_address;
    ::socklen_t                                   d_length;

    template <::nstd::execution::receiver R>
    state(R&&                                    receiver,
          ::nstd::file::ring_context*            context,
          typename Socket::protocol_type const&  protocol,
          typename Socket::native_handle_type    fd)
        : d_receiver(::nstd::utility::forward<R>(receiver))
        , d_context(context)
        , d_protocol(protocol)
        , d_fd(fd)
        , d_length(sizeof(this->d_address))
    {
    }

    friend auto tag_invoke(::nstd::execution::start_t, state& s)
        noexcept -> void {
        s.d_context->accept(s.d_fd, &s.d_address, &s.d_length, 0, &s);
    }
    auto do_result(::std::int32_t rc, ::std::uint32_t)
        -> void override
    {
        if (rc < 0) {
            ::nstd::execution::set_value(::nstd::utility::move(this->d_receiver),
                                         ::std::error_code(-rc, ::std::system_category()),
                                         Socket());
        }
        else {
            ::nstd::execution::set_value(::nstd::utility::move(this->d_receiver),
                                         ::std::error_code(),
                                         Socket(this->d_protocol, rc));

        }
    }
};

static_assert(::nstd::execution::operation_state<
                  ::nstd::net::async_accept_t::state<
                      ::nstd::net::basic_stream_socket<::nstd::net::ip::tcp>,
                      ::nstd::execution::test_receiver_of<::std::error_code,
                                                          ::nstd::net::basic_stream_socket<::nstd::net::ip::tcp>&&>>>);

template <typename Socket>
struct nstd::net::async_accept_t::sender
{
    template <template <typename...> class V, template <typename...> class T>
    using value_types = V<T<::std::error_code, Socket>>;
    template <template <typename...> class V>
    using error_types = V<::std::exception_ptr>;
    static constexpr bool sends_done = true;

    ::nstd::file::ring_context*            d_context;
    typename Socket::protocol_type const&  d_protocol;
    typename Socket::native_handle_type    d_fd;

    template <::nstd::execution::receiver_of<::std::error_code, Socket&&> Receiver>
    friend auto tag_invoke(::nstd::execution::connect_t, sender const& sndr, Receiver&& receiver)
        noexcept -> ::nstd::net::async_accept_t::state<Socket, Receiver>
    {
        return ::nstd::net::async_accept_t::state<Socket, Receiver>{
            ::nstd::utility::forward<Receiver>(receiver),
            sndr.d_context,
            sndr.d_protocol,
            sndr.d_fd
        };
    }
};

static_assert(::nstd::execution::sender<
                 ::nstd::net::async_accept_t::sender<
                     ::nstd::net::basic_stream_socket<::nstd::net::ip::tcp>>>);

template <typename Acceptor, typename Scheduler>
auto nstd::net::async_accept_t::operator()(Acceptor& socket, Scheduler scheduler) const
    -> nstd::net::async_accept_t::sender<typename Acceptor::socket_type>
{
    return nstd::net::async_accept_t::sender<typename Acceptor::socket_type>{
        scheduler.context(),
        socket.protocol(),
        socket.native_handle()
        };
}

// ----------------------------------------------------------------------------

#endif
