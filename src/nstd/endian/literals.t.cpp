// nstd/endian/literals.t.cpp                                         -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2015 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/endian/literals.hpp"
#include "kuhl/test.hpp"

namespace NE = ::nstd::endian;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("unsigned literals", [](KT::context& c)->bool{
            using NE::literals::operator""_eu;

            constexpr NE::type u8{8_eu};
            constexpr NE::type u16{16_eu};
            constexpr NE::type u32{32_eu};
            constexpr NE::type u64{64_eu};
            return KT::assert_type<NE::type, decltype(8_eu)>(c, "8_eu type")
                && KT::assert_true(c, "8_eu type", NE::type::uint8 == u8)
                && KT::assert_type<NE::type, decltype(16_eu)>(c, "16_eu type")
                && KT::assert_true(c, "16_eu type", NE::type::uint16 == u16)
                && KT::assert_type<NE::type, decltype(32_eu)>(c, "32_eu type")
                && KT::assert_true(c, "32_eu type", NE::type::uint32 == u32)
                && KT::assert_type<NE::type, decltype(64_eu)>(c, "64_eu type")
                && KT::assert_true(c, "64_eu type", NE::type::uint64 == u64)
                ;
        }),
    KT::expect_success("signed literals", [](KT::context& c)->bool{
            using NE::literals::operator""_es;

            constexpr NE::type s8{8_es};
            constexpr NE::type s16{16_es};
            constexpr NE::type s32{32_es};
            constexpr NE::type s64{64_es};
            return KT::assert_type<NE::type, decltype(8_es)>(c, "8_es type")
                && KT::assert_true(c, "8_es type", NE::type::int8 == s8)
                && KT::assert_type<NE::type, decltype(16_es)>(c, "16_es type")
                && KT::assert_true(c, "16_es type", NE::type::int16 == s16)
                && KT::assert_type<NE::type, decltype(32_es)>(c, "32_es type")
                && KT::assert_true(c, "32_es type", NE::type::int32 == s32)
                && KT::assert_type<NE::type, decltype(64_es)>(c, "64_es type")
                && KT::assert_true(c, "64_es type", NE::type::int64 == s64)
                ;
        }),
    KT::expect_success("float literals", [](KT::context& c)->bool{
            using NE::literals::operator""_ef;

            constexpr NE::type f32{32_ef};
            constexpr NE::type f64{64_ef};
            return KT::assert_type<NE::type, decltype(32_ef)>(c, "32_ef type")
                && KT::assert_true(c, "32_ef type", NE::type::float32 == f32)
                && KT::assert_type<NE::type, decltype(64_ef)>(c, "64_ef type")
                && KT::assert_true(c, "64_ef type", NE::type::float64 == f64)
                ;
        }),
};

static KT::add_tests suite("endian::literals", ::tests);
