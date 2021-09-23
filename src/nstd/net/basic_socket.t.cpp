// nstd/net/basic_socket.t.cpp                                        -*-C++-*-
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

#include "nstd/net/basic_socket.hpp"
#include "nstd/net/ip/tcp.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = test_declarations;
namespace KT = ::kuhl::test;
namespace Net = ::nstd::net;
namespace IP = ::nstd::net::ip;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        using socket = Net::basic_socket<IP::tcp>;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("types", []{
            return KT::type<TD::socket::native_handle_type> == KT::type<int>
                && KT::type<TD::socket::protocol_type> == KT::type<IP::tcp>
                && KT::type<TD::socket::endpoint_type> == KT::type<IP::tcp::endpoint>
                ;
        }),
    KT::expect_success("default ctor", []{
            TD::socket s;
            return not s.is_open()
                ;
        }),
    KT::expect_success("ctor from protocol", []{
            TD::socket s{IP::tcp::v6()};
            return s.is_open()
                && not s.non_blocking()
                && s.protocol() == IP::tcp::v6()
                ;
        }),
};

static KT::add_tests suite("basic_socket", ::tests);
