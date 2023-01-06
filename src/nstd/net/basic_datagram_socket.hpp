// nstd/net/basic_datagram_socket.hpp                                 -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_BASIC_DATAGRAM_SOCKET
#define INCLUDED_NSTD_NET_BASIC_DATAGRAM_SOCKET

#include "nstd/net/basic_socket.hpp"
#include <system_error>
#include <cerrno>

// ----------------------------------------------------------------------------

namespace nstd::net {
    template <typename Protocol>
    class basic_datagram_socket
        : public ::nstd::net::basic_socket<Protocol>
    {
    public:
        using protocol_type = Protocol;
        using endpoint_type = typename Protocol::endpoint;

        basic_datagram_socket(endpoint_type endpoint)
            : ::nstd::net::basic_socket<Protocol>(endpoint.protocol())
        {
            ::sockaddr_storage address;
            ::socklen_t        socklen(endpoint.get_address(&address));
            if (::bind(this->native_handle(), reinterpret_cast<::sockaddr*>(&address), socklen)) {
                throw ::std::system_error(errno, ::std::system_category(), "failed to bind");
            }
            //-dk:TODO bind
        }
    };
}

// ----------------------------------------------------------------------------

#endif
