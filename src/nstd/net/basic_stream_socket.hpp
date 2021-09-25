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

// ----------------------------------------------------------------------------

namespace nstd::net {
    template <typename> class basic_stream_socket;
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

#endif
