// nstd/net/netfwd.t.cpp                                              -*-C++-*-
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

#include "nstd/net/netfwd.hpp"
#include "kuhl/test.hpp"

namespace KT = ::kuhl::test;
namespace Net = ::nstd::net;
namespace Chrono = ::std::chrono;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("netfwd.hpp declarations are present", [](KT::context& )->bool{
           return KT::assert_type_exists<Net::execution_context>
               && KT::assert_template_exists<Net::executor_binder, int, Net::executor>
               && KT::assert_template_exists<Net::executor_work_guard, Net::executor>
               && KT::assert_type_exists<Net::system_executor>
               && KT::assert_type_exists<Net::executor>
               && KT::assert_template_exists<Net::strand, Net::executor>
               && KT::assert_type_exists<Net::io_context>
               && KT::assert_template_exists<Net::wait_traits, Chrono::system_clock>
               && KT::assert_template_exists<Net::basic_waitable_timer, Chrono::system_clock, Net::wait_traits<Chrono::system_clock>>
               && KT::type<Net::basic_waitable_timer<Chrono::system_clock>>
                  == KT::type<Net::basic_waitable_timer<Chrono::system_clock, Net::wait_traits<Chrono::system_clock>>>
               && KT::assert_type_exists<Net::system_timer>
               && KT::type<Net::system_timer> == KT::type<Net::basic_waitable_timer<Chrono::system_clock>>
               && KT::assert_type_exists<Net::steady_timer>
               && KT::type<Net::steady_timer> == KT::type<Net::basic_waitable_timer<Chrono::steady_clock>>
               && KT::assert_type_exists<Net::high_resolution_timer>
               && KT::type<Net::high_resolution_timer> == KT::type<Net::basic_waitable_timer<Chrono::high_resolution_clock>>
               && KT::assert_template_exists<Net::basic_socket, Net::ip::tcp>
               && KT::assert_template_exists<Net::basic_datagram_socket, Net::ip::tcp>
               && KT::assert_template_exists<Net::basic_stream_socket, Net::ip::tcp>
               && KT::assert_template_exists<Net::basic_socket_acceptor, Net::ip::tcp>
               && KT::assert_template_exists<Net::basic_socket_streambuf, Net::ip::tcp, Chrono::system_clock, Net::wait_traits<Chrono::system_clock>>
               && KT::assert_template_exists<Net::basic_socket_iostream, Net::ip::tcp, Chrono::system_clock, Net::wait_traits<Chrono::system_clock>>
               && KT::assert_type_exists<Net::ip::address>
               && KT::assert_type_exists<Net::ip::address_v4>
               && KT::assert_type_exists<Net::ip::address_v6>
               && KT::assert_template_exists<Net::ip::basic_address_iterator, Net::ip::address_v4>
               && KT::assert_type_exists<Net::ip::address_v4_iterator>
               && KT::type<Net::ip::address_v4_iterator> == KT::type<Net::ip::basic_address_iterator<Net::ip::address_v4>>
               && KT::assert_type_exists<Net::ip::address_v6_iterator>
               && KT::type<Net::ip::address_v6_iterator> == KT::type<Net::ip::basic_address_iterator<Net::ip::address_v6>>
               && KT::assert_template_exists<Net::ip::basic_address_range, Net::ip::address_v4>
               && KT::assert_type_exists<Net::ip::address_v4_range>
               && KT::type<Net::ip::address_v4_range> == KT::type<Net::ip::basic_address_range<Net::ip::address_v4>>
               && KT::assert_type_exists<Net::ip::address_v6_range>
               && KT::type<Net::ip::address_v6_range> == KT::type<Net::ip::basic_address_range<Net::ip::address_v6>>
               && KT::assert_type_exists<Net::ip::network_v4>
               && KT::assert_type_exists<Net::ip::network_v6>
               && KT::assert_template_exists<Net::ip::basic_endpoint, Net::ip::tcp>
               && KT::assert_template_exists<Net::ip::basic_resolver_entry, Net::ip::tcp>
               && KT::assert_template_exists<Net::ip::basic_resolver_results, Net::ip::tcp>
               && KT::assert_template_exists<Net::ip::basic_resolver, Net::ip::tcp>
               && KT::assert_type_exists<Net::ip::tcp>
               && KT::assert_type_exists<Net::ip::udp>
               ;
        }),
};

static KT::add_tests suite("netfwd", ::tests);
