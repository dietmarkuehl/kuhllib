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
//#include "nstd/net/async_accept.hpp"
#include "nstd/net/ip/tcp.hpp"
#include "nstd/net/socket_base.hpp"
#include "nstd/net/basic_stream_socket.hpp"
#include "nstd/file/descriptor.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include <optional>
#include <tuple>
#include <variant>
#include <system_error>
#include <cerrno>
#include "nstd/file/socket.hpp"
#ifndef _MSC_VER
#    include <netinet/ip.h>
#endif
#include <iostream> //-dk:TODO remove

// ----------------------------------------------------------------------------

namespace nstd::net {
    template <typename AcceptableProtocol>
    class basic_socket_acceptor;

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

    auto enable_connection_aborted() const -> bool { return false; }
    auto protocol() const -> protocol_type { return *this->d_protocol; }

    auto accept() -> socket_type;
    auto accept(::std::error_code&) -> socket_type;
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
    : ::nstd::net::socket_base(protocol.family(),
                               protocol.type(),
                               protocol.protocol())
    , d_protocol(protocol)
{
}

template <typename AcceptableProtocol>
nstd::net::basic_socket_acceptor<AcceptableProtocol>::basic_socket_acceptor(
    endpoint_type const& endpoint, bool reuse)
    : ::nstd::net::basic_socket_acceptor<AcceptableProtocol>(endpoint.protocol())
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

template <typename AcceptableProtocol>
auto nstd::net::basic_socket_acceptor<AcceptableProtocol>::accept(::std::error_code& ec) -> socket_type
{
    auto res = ::accept(this->native_handle(), nullptr, 0);
    if (res < 0) {
        ec = std::error_code(errno, std::system_category());
        return socket_type();
    }
    return socket_type(this->protocol(), res);
}

template <typename AcceptableProtocol>
auto nstd::net::basic_socket_acceptor<AcceptableProtocol>::accept() -> socket_type
{
    ::std::error_code ec;
    auto rc = this->accept(ec);
    if (ec) {
        throw ::std::system_error(ec);
    }
    return rc;
}

// ----------------------------------------------------------------------------

#endif
