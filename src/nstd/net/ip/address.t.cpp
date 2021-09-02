// nstd/net/ip/address.t.cpp                                          -*-C++-*-
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

#include "nstd/net/ip/address.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = test_declarations;
namespace KT = ::kuhl::test;
namespace NI = ::nstd::net::ip;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("default ctor", []{
            constexpr NI::address address;
            return KT::use(address)
                && noexcept(NI::address())
                && address.is_v4()
                && not address.is_v6()
                && address.is_unspecified()
                ;
        }),
    KT::expect_success("ctor from address_v4", []{
            constexpr NI::address_v4  v4(0xC0000001);
            constexpr NI::address     address(v4);
            return KT::use(address)
                && noexcept(NI::address(v4))
                && address.is_v4()
                && not address.is_v6()
                && address.to_v4() == v4
                ;
        }),
    KT::expect_success("ctor from address_v6", []{
            constexpr NI::address_v6::bytes_type bytes{ 0x7f };
            constexpr NI::address_v6             v6(bytes);
            constexpr NI::address                address(v6);
            return KT::use(address)
                && noexcept(NI::address(v6))
                && not address.is_v4()
                && address.is_v6()
                && address.to_v6() == v6
                ;
        }),
    KT::expect_success("assign from address_v4", []{
            constexpr NI::address_v4  v4(0xC0000001);
            NI::address               address(NI::address_v6::loopback());
            return KT::use(address)
                && not address.is_v4()
                && noexcept(address = v4)
                && &address == &(address = v4)
                && address.is_v4()
                && not address.is_v6()
                && address.to_v4() == v4
                ;
        }),
    KT::expect_success("assign from address_v6", []{
            constexpr NI::address_v6::bytes_type bytes{ 0x7f };
            constexpr NI::address_v6             v6(bytes);
            NI::address                          address(NI::address_v4::loopback());
            return KT::use(address)
                && not address.is_v6()
                && noexcept(address = v6)
                && &address == &(address = v6)
                && address.is_v6()
                && not address.is_v4()
                && address.to_v6() == v6
                ;
        }),
};

static KT::add_tests suite("address", ::tests);
