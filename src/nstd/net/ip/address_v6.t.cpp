// nstd/net/ip/address_v6.t.cpp                                       -*-C++-*-
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

#include "nstd/net/ip/address_v6.hpp"
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
    KT::expect_success("nested types", []{
            return KT::assert_type_exists<NI::address_v6::bytes_type>
                ;
        }),
    KT::expect_success("bytes_type derives from array<unsigned char, 16>", []{
            NI::address_v6::bytes_type      bytes{ 192, 168, '\0', 1u };
            ::std::array<unsigned char, 16>& array(bytes);
            return KT::use(array)
                && array == ::std::array<unsigned char, 16>{192, 168, 0, 1}
                ;
        }),
    KT::expect_success("address_v6 default ctor", []{
            constexpr NI::address_v6 address{};
            return address.to_bytes() == NI::address_v6::bytes_type{0, 0, 0, 0}
                && address.scope_id() == 0u
                && noexcept(NI::address_v6())
                ;
        }),
    KT::expect_success("address_v6 ctor from bytes", []{
            constexpr NI::address_v6::bytes_type bytes{192, 168, 0, 1};
            return NI::address_v6(bytes).to_bytes() == bytes
                && NI::address_v6(bytes).scope_id() == NI::scope_id_type()
                ;
        }),
    KT::expect_success("address_v6 ctor from bytes and scope_id", []{
            constexpr NI::address_v6::bytes_type bytes{192, 168, 0, 1};
            return NI::address_v6(bytes, 17u).to_bytes() == bytes
                && NI::address_v6(bytes, 17u).scope_id() == 17
                ;
        }),
    KT::expect_success("address_v6::scope_id()", []{
            NI::address_v6 address{};
            address.scope_id(17u);
            return address.scope_id() == 17u
                && noexcept(address.scope_id(17u))
                ;
        }),
#if 0
    KT::expect_success("address_v6::is_unspecified()", []{
            return NI::address_v6(0x00000000).is_unspecified()
                && not NI::address_v6(0xC0A80001).is_unspecified()
                && noexcept(NI::address_v6().is_unspecified())
                ;
        }),
    KT::expect_success("address_v6::is_loopback()", []{
            return not NI::address_v6(0x00000000).is_loopback()
                && not NI::address_v6(0xC0A80001).is_loopback()
                && NI::address_v6(0x7F000001).is_loopback()
                && noexcept(NI::address_v6().is_loopback())
                ;
        }),
    KT::expect_success("address_v6::is_multicast()", []{
            return not NI::address_v6(0x00000000).is_multicast()
                && not NI::address_v6(0xC0A80001).is_multicast()
                && NI::address_v6(0xE0000001).is_multicast()
                && noexcept(NI::address_v6().is_multicast())
                ;
        }),
    KT::expect_success("address_v6::to_string()", []{
            return NI::address_v6(0xC0A80001).to_string() == "192.168.0.1"
                ;
        }),
    KT::expect_success("address_v6::any()", []{
            return NI::address_v6::any() == NI::address_v4()
                && noexcept(NI::address_v6::any())
                ;
        }),
    KT::expect_success("address_v6::loopback()", []{
            return NI::address_v6::loopback() == NI::address_v4(0x7F000001)
                && noexcept(NI::address_v6::loopback())
                ;
        }),
    KT::expect_success("address_v6::broadcast()", []{
            return NI::address_v6::broadcast() == NI::address_v4(0xFFFFFFFF)
                && noexcept(NI::address_v6::broadcast())
                ;
        }),
#endif
};

static KT::add_tests suite("address_v6", ::tests);
