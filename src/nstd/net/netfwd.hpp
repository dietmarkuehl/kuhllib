// nstd/net/netfwd.hpp                                                -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_NETFWD
#define INCLUDED_NSTD_NET_NETFWD

#include "nstd/executor/executor.hpp"
#include <chrono> //-dk:TODO

// ----------------------------------------------------------------------------

namespace nstd::inline net
{
#if 0
    //-dk:TODO remove declarations
    class execution_context;
    template<typename T, typename Executor>
        class executor_binder;
    template<typename Executor>
        class executor_work_guard;
    class system_executor;
    class executor;
    template<typename Executor>
        class strand;
#endif
    class io_context;

    template<typename Clock>
        struct wait_traits;
    template<typename Clock, typename WaitTraits = wait_traits<Clock>>
        class basic_waitable_timer;
    using system_timer = basic_waitable_timer<std::chrono::system_clock>;
    using steady_timer = basic_waitable_timer<std::chrono::steady_clock>;
    using high_resolution_timer = basic_waitable_timer<std::chrono::high_resolution_clock>;

    template<typename Protocol>
        class basic_socket;
    template<typename Protocol>
        class basic_datagram_socket;
    template<typename Protocol>
        class basic_stream_socket;
    template<typename Protocol>
        class basic_socket_acceptor;

    template<typename Protocol, typename Clock = std::chrono::steady_clock,
             typename WaitTraits = wait_traits<Clock>>
        class basic_socket_streambuf;
    template<typename Protocol, typename Clock = std::chrono::steady_clock,
             typename WaitTraits = wait_traits<Clock>>
        class basic_socket_iostream;

    namespace ip {
        class address;
        class address_v4;
        class address_v6;
        template<typename Address>
            class basic_address_iterator;
        using address_v4_iterator = basic_address_iterator<address_v4>;
        using address_v6_iterator = basic_address_iterator<address_v6>;
        template<typename Address>
            class basic_address_range;
        using address_v4_range = basic_address_range<address_v4>;
        using address_v6_range = basic_address_range<address_v6>;
        class network_v4;
        class network_v6;
        template<typename InternetProtocol>
            class basic_endpoint;
        template<typename InternetProtocol>
            class basic_resolver_entry;
        template<typename InternetProtocol>
            class basic_resolver_results;
        template<typename InternetProtocol>
            class basic_resolver;
        class tcp;
        class udp;
    }
}

// ----------------------------------------------------------------------------

#endif
