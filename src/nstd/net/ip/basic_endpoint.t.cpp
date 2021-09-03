// nstd/net/ip/basic_endpoint.t.cpp                                      -*-C++-*-
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

#include "nstd/net/ip/basic_endpoint.hpp"
#include "nstd/net/ip/tcp.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = test_declarations;
namespace KT = ::kuhl::test;
namespace NN = ::nstd::net;
namespace NI = ::nstd::net::ip;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("nested typedef", []{
            return KT::type<NI::basic_endpoint<NI::tcp>::protocol_type>
                    == KT::type<NI::tcp>
                ;
        }),
    KT::expect_success("default ctor", []{
            constexpr NI::basic_endpoint<NI::tcp> ep;
            return KT::use(ep)
                && noexcept(NI::basic_endpoint<NI::tcp>())
                && ep.address() == NI::address()
                && ep.port() == 0
                ;
        }),
    KT::expect_success("ctor from protocol", []{
            constexpr NI::basic_endpoint<NI::tcp> ep(NI::tcp::v4(), 17u);
            return KT::use(ep)
                && noexcept(NI::basic_endpoint<NI::tcp>(NI::tcp::v4(), 17u))
                && ep.address() == NI::address_v4()
                && ep.protocol() == NI::tcp::v4()
                && ep.port() == 17u
                ;
        }),
    KT::expect_success("ctor from address", []{
            constexpr NI::address                 address(NI::address_v4(0xC0ea0001));
            constexpr NI::basic_endpoint<NI::tcp> ep(address, 17u);
            return KT::use(ep)
                && noexcept(NI::basic_endpoint<NI::tcp>(NI::tcp::v4(), 17u))
                && ep.address() == address
                && ep.port() == 17u
                ;
        }),
};

static KT::add_tests suite("basic_endpoint", ::tests);
