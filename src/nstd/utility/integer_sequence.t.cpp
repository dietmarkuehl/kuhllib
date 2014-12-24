// nstd/utility/integer_sequence.t.cpp                                -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2014 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/utility/integer_sequence.hpp"
#include "kuhl/test.hpp"

namespace NU = nstd::utility;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("integer_sequence", [](KT::context& c)->bool{
            using int0 = NU::integer_sequence<int>;
            constexpr std::size_t size_int0{int0::size()};
            using int1 = NU::integer_sequence<int, 1>;
            constexpr std::size_t size_int1{int1::size()};
            using int2 = NU::integer_sequence<int, 2, 1>;
            constexpr std::size_t size_int2{int2::size()};
            using size_t0 = NU::integer_sequence<size_t>;
            constexpr std::size_t size_size_t0{size_t0::size()};
            using size_t1 = NU::integer_sequence<size_t, 1>;
            constexpr std::size_t size_size_t1{size_t1::size()};
            using size_t2 = NU::integer_sequence<size_t, 2, 1>;
            constexpr std::size_t size_size_t2{size_t2::size()};
            return KT::assert_type<int, int0::value_type>(c, "int0 value_type")
                && KT::assert_equal(c, "int0 size()", 0u, size_int0)
                && KT::assert_type<int, int1::value_type>(c, "int1 value_type")
                && KT::assert_equal(c, "int1 size()", 1u, size_int1)
                && KT::assert_type<int, int2::value_type>(c, "int2 value_type")
                && KT::assert_equal(c, "int2 size()", 2u, size_int2)
                && KT::assert_type<size_t, size_t0::value_type>(c, "size_t0 value_type")
                && KT::assert_equal(c, "size_t0 size()", 0u, size_size_t0)
                && KT::assert_type<size_t, size_t1::value_type>(c, "size_t1 value_type")
                && KT::assert_equal(c, "size_t1 size()", 1u, size_size_t1)
                && KT::assert_type<size_t, size_t2::value_type>(c, "size_t2 value_type")
                && KT::assert_equal(c, "size_t2 size()", 2u, size_size_t2)
                ;
        }),
    KT::expect_success("index_sequence", [](KT::context& c)->bool{
            return KT::assert_type<NU::integer_sequence<std::size_t, 4, 3, 1, 2>,
                                   NU::index_sequence<4, 3, 1, 2> >(c, "4, 3, 1, 2")
                && KT::assert_type<NU::integer_sequence<std::size_t, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9>,
                                   NU::index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9> >(c, "0, 1, 2, 3, 4, 5, 6, 7, 8, 9")
                ;
        }),
    KT::expect_success("make_integer_sequence", [](KT::context& c)->bool{
            return KT::assert_type<NU::integer_sequence<int>,
                                   NU::make_integer_sequence<int, 0> >(c, "make_integer_sequence<int, 0>")
                && KT::assert_type<NU::integer_sequence<int, 0>,
                                   NU::make_integer_sequence<int, 1> >(c, "make_integer_sequence<int, 1>")
                && KT::assert_type<NU::integer_sequence<int, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9>,
                                   NU::make_integer_sequence<int, 10> >(c, "make_integer_sequence<int, 10>")
                && KT::assert_type<NU::integer_sequence<std::size_t>,
                                   NU::make_integer_sequence<std::size_t, 0> >(c, "make_integer_sequence<std::size_t, 0>")
                && KT::assert_type<NU::integer_sequence<std::size_t, 0>,
                                   NU::make_integer_sequence<std::size_t, 1> >(c, "make_integer_sequence<std::size_t, 1>")
                && KT::assert_type<NU::integer_sequence<std::size_t, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9>,
                                   NU::make_integer_sequence<std::size_t, 10> >(c, "make_integer_sequence<std::size_t, 10>")
                ;
        }),
    KT::expect_success("make_index_sequence", [](KT::context& c)->bool{
            return KT::assert_type<NU::integer_sequence<std::size_t>,
                                   NU::make_index_sequence<0> >(c, "make_index_sequence<0>")
                && KT::assert_type<NU::integer_sequence<std::size_t, 0>,
                                   NU::make_index_sequence<1> >(c, "make_index_sequence<1>")
                && KT::assert_type<NU::integer_sequence<std::size_t, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9>,
                                   NU::make_index_sequence<10> >(c, "make_index_sequence<10>")
                ;
        }),
    KT::expect_success("index_sequence_for", [](KT::context& c)->bool{
            return KT::assert_type<NU::integer_sequence<std::size_t>,
                                   NU::index_sequence_for<> >(c, "index_sequence_for<>")
                && KT::assert_type<NU::integer_sequence<std::size_t, 0>,
                                   NU::index_sequence_for<bool> >(c, "index_sequence_for<bool>")
                && KT::assert_type<NU::integer_sequence<std::size_t, 0, 1, 2, 3, 4, 5, 6>,
                                   NU::index_sequence_for<bool, char, short, int, long, float, double> >(c,
                                       "index_sequence_for<bool, char, short, int, long, float, double>")
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("utility::integer_sequence", ac, av, ::tests);
}
