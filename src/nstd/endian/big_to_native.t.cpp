// nstd/endian/big_to_native.t.cpp                                    -*-C++-*-
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

#include "nstd/endian/big_to_native.hpp"
#include "nstd/endian/literals.hpp"
#include <utility>
#include <stdint.h>
#include "kuhl/test.hpp"

namespace NE = ::nstd::endian;
namespace NU = ::std;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_exception<std::runtime_error>("uint8 exception", [](KT::context& c)->bool{
            using NE::literals::operator"" _eu;
            char const array0[] = { char(0x00) };
            return NE::big_to_native<8_eu>(array0, array0 + 0).second ==  array0 + 0
                ;
        }),
    KT::expect_success("uint8", [](KT::context& c)->bool{
            using NE::literals::operator"" _eu;

            char const array0[]   = { char(0x00) };
            char const array127[] = { char(0x7F) };
            char const array255[] = { char(0xFF) };
            return KT::assert_type<NU::pair<uint8_t, char const*>,
                                   decltype(NE::big_to_native<8_eu>(array0, array0 + 1))>(c, "type")
                && KT::assert_equal(c, "0", uint8_t(0x00), NE::big_to_native<8_eu>(array0, array0 + 1).first)
                && KT::assert_true(c, "0 it", array0 + 1 == NE::big_to_native<8_eu>(array0, array0 + 1).second)
                && KT::assert_equal(c, "127", uint8_t(0x7F), NE::big_to_native<8_eu>(array127, array127 + 1).first)
                && KT::assert_true(c, "127 it", array127 + 1 == NE::big_to_native<8_eu>(array127, array127 + 1).second)
                && KT::assert_equal(c, "255", uint8_t(0xFF), NE::big_to_native<8_eu>(array255, array255 + 1).first)
                && KT::assert_true(c, "255 it", array255 + 1 == NE::big_to_native<8_eu>(array255, array255 + 1).second)
                ;
        }),
    KT::expect_exception<std::runtime_error>("uint16 exception", [](KT::context& c)->bool{
            using NE::literals::operator"" _eu;
            char const array0[] = { char(0x00) };
            return NE::big_to_native<16_eu>(array0, array0 + 1).second ==  array0 + 1
                ;
        }),
    KT::expect_success("uint16", [](KT::context& c)->bool{
            using NE::literals::operator"" _eu;

            char const array0[] = { char(0x00), char(0x00) };
            char const array1[] = { char(0x00), char(0x7F) };
            char const array2[] = { char(0x00), char(0xFF) };
            char const array3[] = { char(0x12), char(0x34) };
            return KT::assert_type<NU::pair<uint16_t, char const*>,
                                   decltype(NE::big_to_native<16_eu>(array0, array0 + 2))>(c, "type")
                && KT::assert_equal(c, "0", uint16_t(0x0000), NE::big_to_native<16_eu>(array0, array0 + 2).first)
                && KT::assert_true(c, "0 it", array0 + 2 == NE::big_to_native<16_eu>(array0, array0 + 2).second)
                && KT::assert_equal(c, "127", uint16_t(0x007F), NE::big_to_native<16_eu>(array1, array1 + 2).first)
                && KT::assert_true(c, "127 it", array1 + 2 == NE::big_to_native<16_eu>(array1, array1 + 2).second)
                && KT::assert_equal(c, "255", uint16_t(0x00FF), NE::big_to_native<16_eu>(array2, array2 + 2).first)
                && KT::assert_true(c, "255 it", array2 + 2 == NE::big_to_native<16_eu>(array2, array2 + 2).second)
                && KT::assert_equal(c, "0x1234", uint16_t(0x1234), NE::big_to_native<16_eu>(array3, array3 + 2).first)
                && KT::assert_true(c, "0x1234 it", array3 + 2 == NE::big_to_native<16_eu>(array3, array3 + 2).second)
                ;
        }),
    KT::expect_exception<std::runtime_error>("uint32 exception", [](KT::context& c)->bool{
            using NE::literals::operator"" _eu;
            char const array0[] = { char(0x00), char(0x00), char(0x00) };
            return NE::big_to_native<32_eu>(array0, array0 + 3).second ==  array0 + 3
                ;
        }),
    KT::expect_success("uint32", [](KT::context& c)->bool{
            using NE::literals::operator"" _eu;

            char const array0[] = { char(0x00), char(0x00), char(0x00), char(0x12) };
            char const array1[] = { char(0x00), char(0x00), char(0x12), char(0x34) };
            char const array2[] = { char(0x00), char(0x12), char(0x34), char(0x56) };
            char const array3[] = { char(0x12), char(0x34), char(0x56), char(0x78) };
            return KT::assert_type<NU::pair<uint32_t, char const*>,
                                   decltype(NE::big_to_native<32_eu>(array0, array0 + 4))>(c, "type")
                && KT::assert_equal(c, "0", uint32_t(0x00000012), NE::big_to_native<32_eu>(array0, array0 + 4).first)
                && KT::assert_true(c, "0 it", array0 + 4 == NE::big_to_native<32_eu>(array0, array0 + 4).second)
                && KT::assert_equal(c, "1", uint32_t(0x00001234), NE::big_to_native<32_eu>(array1, array1 + 4).first)
                && KT::assert_true(c, "1 it", array1 + 4 == NE::big_to_native<32_eu>(array1, array1 + 4).second)
                && KT::assert_equal(c, "2", uint32_t(0x00123456), NE::big_to_native<32_eu>(array2, array2 + 4).first)
                && KT::assert_true(c, "2 it", array2 + 4 == NE::big_to_native<32_eu>(array2, array2 + 4).second)
                && KT::assert_equal(c, "3", uint32_t(0x12345678), NE::big_to_native<32_eu>(array3, array3 + 4).first)
                && KT::assert_true(c, "3 it", array3 + 4 == NE::big_to_native<32_eu>(array3, array3 + 4).second)
                ;
        }),
    KT::expect_exception<std::runtime_error>("uint64 exception", [](KT::context& c)->bool{
            using NE::literals::operator"" _eu;
            char const array0[] = { char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00) };
            return NE::big_to_native<64_eu>(array0, array0 + 7).second ==  array0 + 7
                ;
        }),
    KT::expect_success("uint64", [](KT::context& c)->bool{
            using NE::literals::operator"" _eu;

            char const array0[] = { char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x12) };
            char const array1[] = { char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x12), char(0x34) };
            char const array2[] = { char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x12), char(0x34), char(0x56) };
            char const array3[] = { char(0x00), char(0x00), char(0x00), char(0x00), char(0x12), char(0x34), char(0x56), char(0x78) };
            char const array4[] = { char(0x00), char(0x00), char(0x00), char(0x12), char(0x34), char(0x56), char(0x78), char(0x9a) };
            char const array5[] = { char(0x00), char(0x00), char(0x12), char(0x34), char(0x56), char(0x78), char(0x9a), char(0xbc) };
            char const array6[] = { char(0x00), char(0x12), char(0x34), char(0x56), char(0x78), char(0x9a), char(0xbc), char(0xde) };
            char const array7[] = { char(0x12), char(0x34), char(0x56), char(0x78), char(0x9a), char(0xbc), char(0xde), char(0xf0) };
            return KT::assert_type<NU::pair<uint64_t, char const*>,
                                   decltype(NE::big_to_native<64_eu>(array0, array0 + 4))>(c, "type")
                && KT::assert_equal(c, "0", uint64_t(0x00000012), NE::big_to_native<64_eu>(array0, array0 + 8).first)
                && KT::assert_true(c, "0 it", array0 + 8 == NE::big_to_native<64_eu>(array0, array0 + 8).second)
                && KT::assert_equal(c, "1", uint64_t(0x00001234), NE::big_to_native<64_eu>(array1, array1 + 8).first)
                && KT::assert_true(c, "1 it", array1 + 8 == NE::big_to_native<64_eu>(array1, array1 + 8).second)
                && KT::assert_equal(c, "2", uint64_t(0x00123456), NE::big_to_native<64_eu>(array2, array2 + 8).first)
                && KT::assert_true(c, "2 it", array2 + 8 == NE::big_to_native<64_eu>(array2, array2 + 8).second)
                && KT::assert_equal(c, "3", uint64_t(0x12345678), NE::big_to_native<64_eu>(array3, array3 + 8).first)
                && KT::assert_true(c, "3 it", array3 + 8 == NE::big_to_native<64_eu>(array3, array3 + 8).second)
                && KT::assert_equal(c, "4", uint64_t(0x000000123456789a), NE::big_to_native<64_eu>(array4, array4 + 8).first)
                && KT::assert_true(c, "4 it", array4 + 8 == NE::big_to_native<64_eu>(array4, array4 + 8).second)
                && KT::assert_equal(c, "5", uint64_t(0x0000123456789abc), NE::big_to_native<64_eu>(array5, array5 + 8).first)
                && KT::assert_true(c, "5 it", array5 + 8 == NE::big_to_native<64_eu>(array5, array5 + 8).second)
                && KT::assert_equal(c, "6", uint64_t(0x00123456789abcde), NE::big_to_native<64_eu>(array6, array6 + 8).first)
                && KT::assert_true(c, "6 it", array6 + 8 == NE::big_to_native<64_eu>(array6, array6 + 8).second)
                && KT::assert_equal(c, "7", uint64_t(0x123456789abcdef0), NE::big_to_native<64_eu>(array7, array7 + 8).first)
                && KT::assert_true(c, "7 it", array7 + 8 == NE::big_to_native<64_eu>(array7, array7 + 8).second)
                ;
        }),

    KT::expect_exception<std::runtime_error>("int8 exception", [](KT::context& c)->bool{
            using NE::literals::operator"" _es;
            char const array0[] = { char(0x00) };
            return NE::big_to_native<8_es>(array0, array0 + 0).second ==  array0 + 0
                ;
        }),
    KT::expect_success("int8", [](KT::context& c)->bool{
            using NE::literals::operator"" _es;

            char const array0[] = { char(0x00) };
            char const array1[] = { char(0x7F) };
            char const array2[] = { char(0xFF) };
            return KT::assert_type<NU::pair<int8_t, char const*>,
                                   decltype(NE::big_to_native<8_es>(array0, array0 + 1))>(c, "type")
                && KT::assert_equal(c, "0", int8_t(0x00), NE::big_to_native<8_es>(array0, array0 + 1).first)
                && KT::assert_true(c, "0 it", array0 + 1 == NE::big_to_native<8_es>(array0, array0 + 1).second)
                && KT::assert_equal(c, "1", int8_t(0x7F), NE::big_to_native<8_es>(array1, array1 + 1).first)
                && KT::assert_true(c, "1 it", array1 + 1 == NE::big_to_native<8_es>(array1, array1 + 1).second)
                && KT::assert_equal(c, "2", int8_t(0xFF), NE::big_to_native<8_es>(array2, array2 + 1).first)
                && KT::assert_true(c, "2 it", array2 + 1 == NE::big_to_native<8_es>(array2, array2 + 1).second)
                ;
        }),
    KT::expect_exception<std::runtime_error>("int16 exception", [](KT::context& c)->bool{
            using NE::literals::operator"" _es;
            char const array0[] = { char(0x00) };
            return NE::big_to_native<16_es>(array0, array0 + 1).second ==  array0 + 1
                ;
        }),
    KT::expect_success("int16", [](KT::context& c)->bool{
            using NE::literals::operator"" _es;

            char const array0[] = { char(0x00), char(0x00) };
            char const array1[] = { char(0x00), char(0x7F) };
            char const array2[] = { char(0x00), char(0xFF) };
            char const array3[] = { char(0x12), char(0x34) };
            return KT::assert_type<NU::pair<int16_t, char const*>,
                                   decltype(NE::big_to_native<16_es>(array0, array0 + 2))>(c, "type")
                && KT::assert_equal(c, "0", int16_t(0x0000), NE::big_to_native<16_es>(array0, array0 + 2).first)
                && KT::assert_true(c, "0 it", array0 + 2 == NE::big_to_native<16_es>(array0, array0 + 2).second)
                && KT::assert_equal(c, "127", int16_t(0x007F), NE::big_to_native<16_es>(array1, array1 + 2).first)
                && KT::assert_true(c, "127 it", array1 + 2 == NE::big_to_native<16_es>(array1, array1 + 2).second)
                && KT::assert_equal(c, "255", int16_t(0x00FF), NE::big_to_native<16_es>(array2, array2 + 2).first)
                && KT::assert_true(c, "255 it", array2 + 2 == NE::big_to_native<16_es>(array2, array2 + 2).second)
                && KT::assert_equal(c, "0x1234", int16_t(0x1234), NE::big_to_native<16_es>(array3, array3 + 2).first)
                && KT::assert_true(c, "0x1234 it", array3 + 2 == NE::big_to_native<16_es>(array3, array3 + 2).second)
                ;
        }),
    KT::expect_exception<std::runtime_error>("int32 exception", [](KT::context& c)->bool{
            using NE::literals::operator"" _es;
            char const array0[] = { char(0x00), char(0x00), char(0x00) };
            return NE::big_to_native<32_es>(array0, array0 + 3).second ==  array0 + 3
                ;
        }),
    KT::expect_success("int32", [](KT::context& c)->bool{
            using NE::literals::operator"" _es;

            char const array0[] = { char(0x00), char(0x00), char(0x00), char(0x12) };
            char const array1[] = { char(0x00), char(0x00), char(0x12), char(0x34) };
            char const array2[] = { char(0x00), char(0x12), char(0x34), char(0x56) };
            char const array3[] = { char(0x12), char(0x34), char(0x56), char(0x78) };
            return KT::assert_type<NU::pair<int32_t, char const*>,
                                   decltype(NE::big_to_native<32_es>(array0, array0 + 4))>(c, "type")
                && KT::assert_equal(c, "0", int32_t(0x00000012), NE::big_to_native<32_es>(array0, array0 + 4).first)
                && KT::assert_true(c, "0 it", array0 + 4 == NE::big_to_native<32_es>(array0, array0 + 4).second)
                && KT::assert_equal(c, "1", int32_t(0x00001234), NE::big_to_native<32_es>(array1, array1 + 4).first)
                && KT::assert_true(c, "1 it", array1 + 4 == NE::big_to_native<32_es>(array1, array1 + 4).second)
                && KT::assert_equal(c, "2", int32_t(0x00123456), NE::big_to_native<32_es>(array2, array2 + 4).first)
                && KT::assert_true(c, "2 it", array2 + 4 == NE::big_to_native<32_es>(array2, array2 + 4).second)
                && KT::assert_equal(c, "3", int32_t(0x12345678), NE::big_to_native<32_es>(array3, array3 + 4).first)
                && KT::assert_true(c, "3 it", array3 + 4 == NE::big_to_native<32_es>(array3, array3 + 4).second)
                ;
        }),
    KT::expect_exception<std::runtime_error>("int64 exception", [](KT::context& c)->bool{
            using NE::literals::operator"" _es;
            char const array0[] = { char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00) };
            return NE::big_to_native<64_es>(array0, array0 + 7).second ==  array0 + 7
                ;
        }),
    KT::expect_success("int64", [](KT::context& c)->bool{
            using NE::literals::operator"" _es;

            char const array0[] = { char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x12) };
            char const array1[] = { char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x12), char(0x34) };
            char const array2[] = { char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x12), char(0x34), char(0x56) };
            char const array3[] = { char(0x00), char(0x00), char(0x00), char(0x00), char(0x12), char(0x34), char(0x56), char(0x78) };
            char const array4[] = { char(0x00), char(0x00), char(0x00), char(0x12), char(0x34), char(0x56), char(0x78), char(0x9a) };
            char const array5[] = { char(0x00), char(0x00), char(0x12), char(0x34), char(0x56), char(0x78), char(0x9a), char(0xbc) };
            char const array6[] = { char(0x00), char(0x12), char(0x34), char(0x56), char(0x78), char(0x9a), char(0xbc), char(0xde) };
            char const array7[] = { char(0x12), char(0x34), char(0x56), char(0x78), char(0x9a), char(0xbc), char(0xde), char(0xf0) };
            return KT::assert_type<NU::pair<int64_t, char const*>,
                                   decltype(NE::big_to_native<64_es>(array0, array0 + 4))>(c, "type")
                && KT::assert_equal(c, "0", int64_t(0x00000012), NE::big_to_native<64_es>(array0, array0 + 8).first)
                && KT::assert_true(c, "0 it", array0 + 8 == NE::big_to_native<64_es>(array0, array0 + 8).second)
                && KT::assert_equal(c, "1", int64_t(0x00001234), NE::big_to_native<64_es>(array1, array1 + 8).first)
                && KT::assert_true(c, "1 it", array1 + 8 == NE::big_to_native<64_es>(array1, array1 + 8).second)
                && KT::assert_equal(c, "2", int64_t(0x00123456), NE::big_to_native<64_es>(array2, array2 + 8).first)
                && KT::assert_true(c, "2 it", array2 + 8 == NE::big_to_native<64_es>(array2, array2 + 8).second)
                && KT::assert_equal(c, "3", int64_t(0x12345678), NE::big_to_native<64_es>(array3, array3 + 8).first)
                && KT::assert_true(c, "3 it", array3 + 8 == NE::big_to_native<64_es>(array3, array3 + 8).second)
                && KT::assert_equal(c, "4", int64_t(0x000000123456789a), NE::big_to_native<64_es>(array4, array4 + 8).first)
                && KT::assert_true(c, "4 it", array4 + 8 == NE::big_to_native<64_es>(array4, array4 + 8).second)
                && KT::assert_equal(c, "5", int64_t(0x0000123456789abc), NE::big_to_native<64_es>(array5, array5 + 8).first)
                && KT::assert_true(c, "5 it", array5 + 8 == NE::big_to_native<64_es>(array5, array5 + 8).second)
                && KT::assert_equal(c, "6", int64_t(0x00123456789abcde), NE::big_to_native<64_es>(array6, array6 + 8).first)
                && KT::assert_true(c, "6 it", array6 + 8 == NE::big_to_native<64_es>(array6, array6 + 8).second)
                && KT::assert_equal(c, "7", int64_t(0x123456789abcdef0), NE::big_to_native<64_es>(array7, array7 + 8).first)
                && KT::assert_true(c, "7 it", array7 + 8 == NE::big_to_native<64_es>(array7, array7 + 8).second)
                ;
        }),

    KT::expect_exception<std::runtime_error>("float32 exception", [](KT::context& c)->bool{
            using NE::literals::operator"" _ef;
            char const array0[] = { char(0x00), char(0x00), char(0x00) };
            return NE::big_to_native<32_ef>(array0, array0 + 3).second ==  array0 + 3
                ;
        }),
    KT::expect_success("float32", [](KT::context& c)->bool{
            using NE::literals::operator"" _ef;
            char const array0[] = { char(0x65), char(0x94), char(0x7D), char(0xFE) };
            return KT::assert_type<NU::pair<float, char const*>,
                                   decltype(NE::big_to_native<32_ef>(array0, array0 + 4))>(c, "type")
                && KT::assert_equal(c, "0", 87.6543e21f, NE::big_to_native<32_ef>(array0, array0 + 4).first)
                && KT::assert_true(c, "0 it", array0 + 4 == NE::big_to_native<32_ef>(array0, array0 + 4).second)
                ;
        }),
    KT::expect_exception<std::runtime_error>("float64 exception", [](KT::context& c)->bool{
            using NE::literals::operator"" _ef;
            char const array0[] = { char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00) };
            return NE::big_to_native<64_ef>(array0, array0 + 7).second ==  array0 + 7
                ;
        }),
    KT::expect_success("float64", [](KT::context& c)->bool{
            using NE::literals::operator"" _ef;
            char const array0[] = { char(0x44), char(0xB2), char(0x8F), char(0xBF), char(0xC6), char(0x9D), char(0x9D), char(0x62) };
            return KT::assert_type<NU::pair<double, char const*>,
                                   decltype(NE::big_to_native<64_ef>(array0, array0 + 8))>(c, "type")
                && KT::assert_equal(c, "0", 87.6543e21, NE::big_to_native<64_ef>(array0, array0 + 8).first)
                && KT::assert_true(c, "0 it", array0 + 8 == NE::big_to_native<64_ef>(array0, array0 + 8).second)
                ;
        }),
#if 0
    KT::expect_failure("placeholder", [](KT::context& c)->bool{
           return false;
        }),
#endif
};

int main(int ac, char* av[])
{
    return KT::run_tests("endian::big_to_native", ac, av, ::tests);
}
