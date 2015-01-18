// nstd/endian/type.t.cpp                                             -*-C++-*-
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

#include "nstd/endian/type.hpp"
#include <stdint.h>
#include "kuhl/test.hpp"

namespace NE = ::nstd::endian;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("type members", [](KT::context& c)->bool{
            return KT::assert_type<NE::type, decltype(NE::type::uint8)>(c, "uint8")
                && KT::assert_type<NE::type, decltype(NE::type::uint16)>(c, "uint16")
                && KT::assert_type<NE::type, decltype(NE::type::uint32)>(c, "uint32")
                && KT::assert_type<NE::type, decltype(NE::type::uint64)>(c, "uint64")
                && KT::assert_type<NE::type, decltype(NE::type::int8)>(c, "int8")
                && KT::assert_type<NE::type, decltype(NE::type::int16)>(c, "int16")
                && KT::assert_type<NE::type, decltype(NE::type::int32)>(c, "int32")
                && KT::assert_type<NE::type, decltype(NE::type::int64)>(c, "int64")
                && KT::assert_type<NE::type, decltype(NE::type::float32)>(c, "float32")
                && KT::assert_type<NE::type, decltype(NE::type::float64)>(c, "float64")
                ;
        }),
    KT::expect_success("type_map", [](KT::context& c)->bool{
            return KT::assert_type<uint8_t, NE::type_map<NE::type::uint8>::type>(c, "uint8")
                && KT::assert_type<uint16_t, NE::type_map<NE::type::uint16>::type>(c, "uint16")
                && KT::assert_type<uint32_t, NE::type_map<NE::type::uint32>::type>(c, "uint32")
                && KT::assert_type<uint64_t, NE::type_map<NE::type::uint64>::type>(c, "uint64")
                && KT::assert_type<int8_t, NE::type_map<NE::type::int8>::type>(c, "int8")
                && KT::assert_type<int16_t, NE::type_map<NE::type::int16>::type>(c, "int16")
                && KT::assert_type<int32_t, NE::type_map<NE::type::int32>::type>(c, "int32")
                && KT::assert_type<int64_t, NE::type_map<NE::type::int64>::type>(c, "int64")
                && KT::assert_type<float, NE::type_map<NE::type::float32>::type>(c, "float32")
                && KT::assert_type<double, NE::type_map<NE::type::float64>::type>(c, "float64")
                ;
        }),
    KT::expect_success("type_map_t", [](KT::context& c)->bool{
            return KT::assert_type<uint8_t, NE::type_map_t<NE::type::uint8>>(c, "uint8")
                && KT::assert_type<uint16_t, NE::type_map_t<NE::type::uint16>>(c, "uint16")
                && KT::assert_type<uint32_t, NE::type_map_t<NE::type::uint32>>(c, "uint32")
                && KT::assert_type<uint64_t, NE::type_map_t<NE::type::uint64>>(c, "uint64")
                && KT::assert_type<int8_t, NE::type_map_t<NE::type::int8>>(c, "int8")
                && KT::assert_type<int16_t, NE::type_map_t<NE::type::int16>>(c, "int16")
                && KT::assert_type<int32_t, NE::type_map_t<NE::type::int32>>(c, "int32")
                && KT::assert_type<int64_t, NE::type_map_t<NE::type::int64>>(c, "int64")
                && KT::assert_type<float, NE::type_map_t<NE::type::float32>>(c, "float32")
                && KT::assert_type<double, NE::type_map_t<NE::type::float64>>(c, "float64")
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("endian::type", ac, av, ::tests);
}
