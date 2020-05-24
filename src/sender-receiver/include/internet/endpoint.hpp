// include/internet/endpoint.hpp                                      -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2020 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_INTERNET_ENDPOINT
#define INCLUDED_INTERNET_ENDPOINT

#include <netfwd.hpp>
#include <internet/address.hpp>

#include <ostream>
#include <cstdint>

// ----------------------------------------------------------------------------

namespace cxxrt::net::ip
{
    using port_type = std::uint_least16_t;
    template <typename> class basic_endpoint;
}

// ----------------------------------------------------------------------------

template<class InternetProtocol>
class cxxrt::net::ip::basic_endpoint
{
public:
    using protocol_type = InternetProtocol;

private:
    cxxrt::net::ip::address d_address;
    port_type               d_port;

public:
    constexpr basic_endpoint() noexcept;
    constexpr basic_endpoint(protocol_type const& proto,
                             port_type port_num) noexcept;
    constexpr basic_endpoint(cxxrt::net::ip::address const& addr,
                             port_type port_num) noexcept;

    constexpr protocol_type protocol() const noexcept;
    constexpr ip::address address() const noexcept { return this->d_address; }
    void address(cxxrt::net::ip::address const& addr) noexcept;
    constexpr port_type port() const noexcept { return this->d_port; }
    void port(port_type port_num) noexcept;

    bool operator== (basic_endpoint const&) const;
    bool operator<  (basic_endpoint const&) const;
    template <typename cT, typename Traits>
    friend std::basic_ostream<cT, Traits>&
    operator<< (std::basic_ostream<cT, Traits>& out, basic_endpoint const& e)
    {
        return out << e.address() << ':' << e.port();
    }
};

// ----------------------------------------------------------------------------

template<class IP>
constexpr
cxxrt::net::ip::basic_endpoint<IP>::basic_endpoint(cxxrt::net::ip::address const& address,
                                                   port_type                      port) noexcept
    : d_address(address)
    , d_port(port)
{
}

// ----------------------------------------------------------------------------

#endif
