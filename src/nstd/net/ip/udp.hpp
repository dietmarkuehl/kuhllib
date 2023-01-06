// nstd/net/ip/udp.hpp                                                -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2022 Dietmar Kuehl http://www.dietmar-kuehl.de
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

#ifndef INCLUDED_NSTD_NET_IP_UDP
#define INCLUDED_NSTD_NET_IP_UDP

#include "nstd/net/ip/basic_endpoint.hpp"
//-dk:TODO #include "nstd/net/basic_resolver.hpp"
#include "nstd/net/basic_datagram_socket.hpp"

// ----------------------------------------------------------------------------

namespace nstd::net::ip {
    class udp {
    private:
        int d_family;

        constexpr udp(int family): d_family(family) {}

    public:
        using endpoint = ::nstd::net::ip::basic_endpoint<udp>;
        using socket   = ::nstd::net::basic_datagram_socket<udp>;

        static constexpr auto v4() noexcept -> udp { return udp(AF_INET);  }
        static constexpr auto v6() noexcept -> udp { return udp(AF_INET6); }

        udp() = delete;

        constexpr auto family()   const -> int { return this->d_family; }
        constexpr auto type()     const -> int { return SOCK_DGRAM; }
        constexpr auto protocol() const -> int { return IPPROTO_UDP; }

        constexpr bool operator== (udp const&) const = default;
    };
}

// ----------------------------------------------------------------------------

#endif
