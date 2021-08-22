// nstd/net/tcp.hpp                                                   -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_TCP
#define INCLUDED_NSTD_NET_TCP

#include "nstd/net/netfwd.hpp"

// ----------------------------------------------------------------------------

namespace nstd::net::ip {
    class tcp;
}

// ----------------------------------------------------------------------------

class nstd::net::ip::tcp
{
private:
    explicit constexpr tcp(int) {}

public:
    using endpoint = ::nstd::net::ip::basic_endpoint<::nstd::net::ip::tcp>;
    using resolver = ::nstd::net::ip::basic_resolver<::nstd::net::ip::tcp>;
    using socket = ::nstd::net::basic_stream_socket<::nstd::net::ip::tcp>;
    using acceptor = ::nstd::net::basic_socket_acceptor<::nstd::net::ip::tcp>;
    using iostream = ::nstd::net::basic_socket_iostream<::nstd::net::ip::tcp>;

    class no_delay;

    static constexpr auto v4() noexcept -> ::nstd::net::ip::tcp { return ::nstd::net::ip::tcp(0); }
    static constexpr auto v6() noexcept -> ::nstd::net::ip::tcp { return ::nstd::net::ip::tcp(0); }

    tcp() = delete;

    constexpr auto operator== (tcp const&) const noexcept -> bool = default;
};

// ----------------------------------------------------------------------------

#endif
