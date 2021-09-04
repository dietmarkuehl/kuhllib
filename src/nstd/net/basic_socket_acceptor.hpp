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
#include "nstd/net/io_context.hpp"
#include "nstd/net/socket_base.hpp"
#include "nstd/file/descriptor.hpp"
#include <optional>
#include <sys/types.h>
#include <sys/socket.h>

// ----------------------------------------------------------------------------

namespace nstd::net {
    template <typename AcceptableProtocol, typename Context>
    class basic_socket_acceptor;
}

// ----------------------------------------------------------------------------

template <typename AcceptableProtocol, typename Context>
class nstd::net::basic_socket_acceptor
    : public ::nstd::net::socket_base
{
public:
    using scheduler_type     = typename Context::scheduler_type;
    using native_handle_type = int;
    using protocol_type      = AcceptableProtocol;
    using endpoint_type      = typename protocol_type::endpoint;
    using socket_type        = typename protocol_type::socket;

private:
    Context* d_context;
    ::std::optional<protocol_type> d_protocol;
    ::nstd::file::descriptor       d_fd;

    static auto internal_open(protocol_type) -> ::nstd::file::descriptor;

public:
    explicit basic_socket_acceptor(Context&);
    basic_socket_acceptor(Context&, protocol_type const&);
    basic_socket_acceptor(Context&, endpoint_type const&, bool = true);
    basic_socket_acceptor(Context&, protocol_type const&, native_handle_type const&);
    basic_socket_acceptor(basic_socket_acceptor const&) = delete;
    basic_socket_acceptor(basic_socket_acceptor&&);
    ~basic_socket_acceptor() = default;

    template <typename SettableSocketOption>
    auto set_option(SettableSocketOption const& option) -> void {
        ::setsockopt(this->native_handle(),
                     option.level(this->d_protocol),
                     option.name(this->d_protocol),
                     option.data(this->d_protocol),
                     option.size(this->d_protocol));
    }
    auto listen(int n = max_listen_connections) -> void{ ::listen(this->native_handle(), n); /*-dk:TODO*/ }
    auto bind(endpoint_type const&) -> void { /*-dk:TODO*/ }

    auto is_open() const -> bool { return bool(this->d_fd); }
    auto non_blocking() const -> bool { return false; }
    auto enable_connection_aborted() const -> bool { return false; }
    auto protocol() const -> protocol_type { return *this->d_protocol; }
    auto native_handle() const noexcept -> native_handle_type { return this->d_fd.get(); }
};

// ----------------------------------------------------------------------------

template <typename AcceptableProtocol, typename Context>
auto
nstd::net::basic_socket_acceptor<AcceptableProtocol, Context>
    ::internal_open(protocol_type protocol) -> ::nstd::file::descriptor
{
    return ::nstd::file::descriptor(::socket(protocol.family(),
                                             protocol.type(),
                                             protocol.protocol()));
}

template <typename AcceptableProtocol, typename Context>
nstd::net::basic_socket_acceptor<AcceptableProtocol, Context>::basic_socket_acceptor(Context& context)
    : d_context(&context)
{
}

template <typename AcceptableProtocol, typename Context>
nstd::net::basic_socket_acceptor<AcceptableProtocol, Context>::basic_socket_acceptor(
        Context&             context,
        protocol_type const& protocol)
    : d_context(&context)
    , d_protocol(protocol)
    , d_fd(internal_open(*this->d_protocol))
{
}

template <typename AcceptableProtocol, typename Context>
nstd::net::basic_socket_acceptor<AcceptableProtocol, Context>::basic_socket_acceptor(
    Context&             context,
    endpoint_type const& endpoint, bool reuse)
    : d_context(&context)
    , d_protocol(endpoint.protocol())
    , d_fd(internal_open(*this->d_protocol))
{
    if (reuse) {
        this->set_option(reuse_address(true));
    }
    this->bind(endpoint);
    this->listen();
}

// ----------------------------------------------------------------------------

#endif
