// nstd/net/ip/basic_endpoint.hpp                                     -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_IP_BASIC_ENDPOINT
#define INCLUDED_NSTD_NET_IP_BASIC_ENDPOINT

#include "nstd/net/netfwd.hpp"
#include "nstd/net/ip/address.hpp"

// ----------------------------------------------------------------------------

namespace nstd::net::ip {
    template <typename InternetProtocol>
    class basic_endpoint;
}

// ----------------------------------------------------------------------------

template <typename InternetProtocol>
class nstd::net::ip::basic_endpoint
{
public:
    using protocol_type = InternetProtocol;
    using port_type = unsigned short; //-dk:TODO report missing typedef?

    constexpr auto protocol() const noexcept -> protocol_type;
    constexpr auto address() const noexcept -> ::nstd::net::ip::address;
    constexpr auto port() const noexcept -> port_type;
};

// ----------------------------------------------------------------------------

template <typename InternetProtocol>
constexpr auto nstd::net::ip::basic_endpoint<InternetProtocol>::address() const noexcept
    -> ::nstd::net::ip::address
{
    return {};
}

template <typename InternetProtocol>
constexpr auto nstd::net::ip::basic_endpoint<InternetProtocol>::port() const noexcept
    -> port_type
{
    return {};
}

// ----------------------------------------------------------------------------

#endif
