// include/internet/tcp.hpp                                           -*-C++-*-
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

#ifndef INCLUDED_INTERNET_TCP
#define INCLUDED_INTERNET_TCP

#include <netfwd.hpp>
#include <arpa/inet.h>

// ----------------------------------------------------------------------------

namespace cxxrt::net::ip
{
    class tcp;
}

// ----------------------------------------------------------------------------

class cxxrt::net::ip::tcp
{
private:
    int d_af;
    int d_pf;
    
    constexpr tcp(int af): d_af(af), d_pf() {}

public:
    using endpoint = cxxrt::net::ip::basic_endpoint<tcp>;
    using resolver = cxxrt::net::ip::basic_resolver<tcp>;

    using socket   = cxxrt::net::basic_stream_socket<tcp>;
    using acceptor = cxxrt::net::basic_socket_acceptor<tcp>;
    using iostream = cxxrt::net::basic_socket_iostream<tcp>;

    class no_delay;

    static constexpr tcp v4() noexcept { return tcp(AF_INET); }
    static constexpr tcp v6() noexcept { return tcp(AF_INET6); } 

    tcp() = delete;

    constexpr int domain()   const noexcept { return this->d_af; }
    constexpr int protocol() const noexcept { return this->d_pf; }

    friend constexpr bool operator==(const tcp& a, const tcp& b) noexcept;
};

// ----------------------------------------------------------------------------

#endif
