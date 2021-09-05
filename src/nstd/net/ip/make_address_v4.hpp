// nstd/net/ip/make_address_v4.hpp                                    -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_IP_MAKE_ADDRESS_V4
#define INCLUDED_NSTD_NET_IP_MAKE_ADDRESS_V4

#include "nstd/net/ip/address_v4.hpp"
#include <arpa/inet.h>

// ----------------------------------------------------------------------------

namespace nstd::net::ip {
    constexpr auto make_address_v4(::nstd::net::ip::address_v4::bytes_type const&)
        -> ::nstd::net::ip::address_v4;
    constexpr auto make_address_v4(::nstd::net::ip::address_v4::uint_type)
        -> ::nstd::net::ip::address_v4;
    constexpr auto make_address_v4(::nstd::net::ip::v4_mapped_t,
                                   ::nstd::net::ip::address_v6 const&)
        -> ::nstd::net::ip::address_v4;
    auto make_address_v4(char const*)
        -> ::nstd::net::ip::address_v4;
    constexpr auto make_address_v4(char const*, ::std::error_code&) noexcept
        -> ::nstd::net::ip::address_v4;
    constexpr auto make_address_v4(::std::string const&)
        -> ::nstd::net::ip::address_v4;
    constexpr auto make_address_v4(::std::string const&, ::std::error_code&) noexcept
        -> ::nstd::net::ip::address_v4;
    constexpr auto make_address_v4(::std::string_view)
        -> ::nstd::net::ip::address_v4;
    constexpr auto make_address_v4(::std::string_view, ::std::error_code&) noexcept
        -> ::nstd::net::ip::address_v4;
}

// ----------------------------------------------------------------------------

inline auto nstd::net::ip::make_address_v4(char const* addr)
    -> ::nstd::net::ip::address_v4
{    
    address_v4::bytes_type bytes;
    return ::inet_pton(AF_INET, addr, &bytes[0])? address_v4(bytes): address_v4();
}

// ----------------------------------------------------------------------------

#endif
