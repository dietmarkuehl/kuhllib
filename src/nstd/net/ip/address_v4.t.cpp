// nstd/net/ip/address_v4.t.cpp                                       -*-C++-*-
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

#include "nstd/net/ip/address_v4.hpp"
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
            return KT::type<NI::address_v4::uint_type> == KT::type<::std::uint_least32_t>
                && KT::assert_type_exists<NI::address_v4::bytes_type>
                ;
        }),
    KT::expect_success("bytes_type derives from array<unsigned char, 4>", []{
            NI::address_v4::bytes_type      bytes{ 192, 168, '\0', 1u };
            ::std::array<unsigned char, 4>& array(bytes);
            return KT::use(array)
                && array == ::std::array<unsigned char, 4>{192, 168, 0, 1}
                ;
        }),
    KT::expect_success("address_v4 default ctor", []{
            constexpr NI::address_v4 address;
            return address.to_bytes() == NI::address_v4::bytes_type{0, 0, 0, 0}
                && address.to_uint() == 0u
                && noexcept(NI::address_v4())
                ;
        }),
    KT::expect_success("address_v4 ctor from bytes", []{
            constexpr NI::address_v4::bytes_type bytes{192, 168, 0, 1};
            return NI::address_v4(bytes).to_bytes() == bytes
                && NI::address_v4(0xC0A80001).to_uint() == 0xC0A80001
                ;
        }),
    KT::expect_success("address_v4 ctor from uint", []{
            constexpr NI::address_v4::bytes_type bytes{192, 168, 0, 1};
            return NI::address_v4(0xC0A80001).to_bytes() == bytes
                && NI::address_v4(0xC0A80001).to_uint() == 0xC0A80001
                ;
        }),
    KT::expect_success("address_v4::is_unspecified()", []{
            return NI::address_v4(0x00000000).is_unspecified()
                && not NI::address_v4(0xC0A80001).is_unspecified()
                && noexcept(NI::address_v4().is_unspecified())
                ;
        }),
    KT::expect_success("address_v4::is_loopback()", []{
            return not NI::address_v4(0x00000000).is_loopback()
                && not NI::address_v4(0xC0A80001).is_loopback()
                && NI::address_v4(0x7F000001).is_loopback()
                && noexcept(NI::address_v4().is_loopback())
                ;
        }),
    KT::expect_success("address_v4::is_multicast()", []{
            return not NI::address_v4(0x00000000).is_multicast()
                && not NI::address_v4(0xC0A80001).is_multicast()
                && NI::address_v4(0xE0000001).is_multicast()
                && noexcept(NI::address_v4().is_multicast())
                ;
        }),
    KT::expect_success("address_v4::to_string()", []{
            return NI::address_v4(0xC0A80001).to_string() == "192.168.0.1"
                ;
        }),
    KT::expect_success("address_v4::any()", []{
            return NI::address_v4::any() == NI::address_v4()
                && noexcept(NI::address_v4::any())
                ;
        }),
    KT::expect_success("address_v4::loopback()", []{
            return NI::address_v4::loopback() == NI::address_v4(0x7F000001)
                && noexcept(NI::address_v4::loopback())
                ;
        }),
    KT::expect_success("address_v4::broadcast()", []{
            return NI::address_v4::broadcast() == NI::address_v4(0xFFFFFFFF)
                && noexcept(NI::address_v4::broadcast())
                ;
        }),
};

static KT::add_tests suite("address_v4", ::tests);
