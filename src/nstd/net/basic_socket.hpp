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
#include <optional>

// ----------------------------------------------------------------------------

namespace nstd::net {
    template <typename> class basic_socket;
}

// ----------------------------------------------------------------------------

template <typename Protocol>
class nstd::net::basic_socket
    : ::nstd::net::socket_base
{
public:
    using native_handle_type = int;
    using protocol_type = Protocol;
    using endpoint_type = typename protocol_type::endpoint;

private:
    ::std::optional<protocol_type> d_protocol;

public:
    basic_socket();
    basic_socket(protocol_type const&);

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

#endif
