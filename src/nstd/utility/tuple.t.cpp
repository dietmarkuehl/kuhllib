// nstd/utility/tuple.t.cpp                                           -*-C++-*-
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

#include "nstd/utility/tuple.hpp"
#include "kuhl/test.hpp"

namespace NU = nstd::utility;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("default ctor", [](KT::context& c)->bool{
            constexpr NU::tuple<> empty;
            KT::use(empty);
#ifndef KUHLLIB_EDG
            constexpr
#else
            const
#endif
                NU::tuple<bool, int, char> ct;
            KT::use(ct);
            NU::tuple<bool, int, char> t;
            return true
                && KT::assert_type<bool const&, decltype(NU::get<0>(ct))>(c, "ct<0> type")
                && KT::assert_type<int const&, decltype(NU::get<1>(ct))>(c, "ct<1> type")
                && KT::assert_type<char const&, decltype(NU::get<2>(ct))>(c, "ct<2> type")
                && KT::assert_equal(c, "ct<0> value", NU::get<0>(ct), false)
                && KT::assert_equal(c, "ct<1> value", NU::get<1>(ct), 0)
                && KT::assert_equal(c, "ct<2> value", NU::get<2>(ct), char())
                && KT::assert_type<bool&, decltype(NU::get<0>(t))>(c, "t<0> type")
                && KT::assert_type<int&, decltype(NU::get<1>(t))>(c, "t<1> type")
                && KT::assert_type<char&, decltype(NU::get<2>(t))>(c, "t<2> type")
                && KT::assert_equal(c, "t<0> value", NU::get<0>(t), false)
                && KT::assert_equal(c, "t<1> value", NU::get<1>(t), 0)
                && KT::assert_equal(c, "t<2> value", NU::get<2>(t), char())
                && KT::assert_type<bool&&, decltype(NU::get<0>(NU::tuple<bool, int, char>()))>(c, "tmpt<0> type")
                && KT::assert_type<int&&, decltype(NU::get<1>(NU::tuple<bool, int, char>()))>(c, "tmpt<1> type")
                && KT::assert_type<char&&, decltype(NU::get<2>(NU::tuple<bool, int, char>()))>(c, "tmpt<2> type")
                && KT::assert_equal(c, "tmp<0> value", NU::get<0>(NU::tuple<bool, int, char>()), false)
                && KT::assert_equal(c, "tmp<1> value", NU::get<1>(NU::tuple<bool, int, char>()), 0)
                && KT::assert_equal(c, "tmp<2> value", NU::get<2>(NU::tuple<bool, int, char>()), char())
                ;
        }),
    KT::expect_success("lvalue ctor", [](KT::context& c)->bool{
            constexpr int  i(17);
            constexpr char d('a');
            constexpr bool b(true);
#ifndef KUHLLIB_EDG
            constexpr
#else
            const
#endif
                NU::tuple<int, char, bool> t(i, d, b);
            KT::use(t);
            return true
                && KT::assert_equal(c, "t<0> value", NU::get<0u>(t), i)
                && KT::assert_equal(c, "t<1> value", NU::get<1>(t), d)
                && KT::assert_equal(c, "t<2> value", NU::get<2>(t), b)
                ;
        }),
    KT::expect_success("rvalue ctor", [](KT::context& c)->bool{
#ifndef KUHLLIB_INTEL
            constexpr NU::tuple<char, bool, int> t0('c', true, 42);
            constexpr NU::tuple<int, int, long> t1('c', true, 42);
#else
            const NU::tuple<char, bool, int> t0('c', true, 42);
            const NU::tuple<int, int, long> t1('c', true, 42);
#endif
            return KT::assert_equal(c, "t0<0> value", NU::get<0>(t0), 'c')
                && KT::assert_equal(c, "t0<1> value", NU::get<1>(t0), true)
                && KT::assert_equal(c, "t0<2> value", NU::get<2>(t0), 42)
                && KT::assert_equal(c, "t1<0> value", NU::get<0>(t1), 'c')
                && KT::assert_equal(c, "t1<1> value", NU::get<1>(t1), true)
                && KT::assert_equal(c, "t1<2> value", NU::get<2>(t1), 42)
                ;
        }),
    KT::expect_success("copy ctor", [](KT::context& c)->bool{
            NU::tuple<bool, char, long> const t0(true, 'b', 314);
            NU::tuple<bool, char, long> t1(t0);
            NU::tuple<bool, char, long> t2(t1);
            return KT::assert_equal(c, "t1<0> value", NU::get<0>(t0), NU::get<0>(t1))
                && KT::assert_equal(c, "t1<1> value", NU::get<1>(t0), NU::get<1>(t1))
                && KT::assert_equal(c, "t1<2> value", NU::get<2>(t0), NU::get<2>(t1))
                && KT::assert_equal(c, "t2<0> value", NU::get<0>(t0), NU::get<0>(t1))
                && KT::assert_equal(c, "t2<1> value", NU::get<1>(t0), NU::get<1>(t2))
                && KT::assert_equal(c, "t2<2> value", NU::get<2>(t0), NU::get<2>(t2))
                ;
        }),
    KT::expect_success("move ctor", [](KT::context& c)->bool{
            NU::tuple<bool, char, long> t0(NU::tuple<bool, char, long>(true, 'b', 314));
            return KT::assert_equal(c, "t0<0> value", NU::get<0>(t0), true)
                && KT::assert_equal(c, "t0<1> value", NU::get<1>(t0), 'b')
                && KT::assert_equal(c, "t0<2> value", NU::get<2>(t0), 314)
                ;
        }),
    KT::expect_success("lvalue conversion", [](KT::context& c)->bool{
            NU::tuple<int, int, int> const t0(true, 'b', 314);
            NU::tuple<bool, char, long> t1(t0);
            NU::tuple<int, int, int> t2(t1);
            return KT::assert_equal(c, "t1<0> value", NU::get<0>(t1), true)
                && KT::assert_equal(c, "t1<1> value", NU::get<1>(t1), 'b')
                && KT::assert_equal(c, "t1<2> value", NU::get<2>(t1), 314)
                && KT::assert_equal(c, "t2<0> value", NU::get<0>(t2), true)
                && KT::assert_equal(c, "t2<1> value", NU::get<1>(t2), 'b')
                && KT::assert_equal(c, "t2<2> value", NU::get<2>(t2), 314)
                ;
        }),
    KT::expect_success("rvalue conversion", [](KT::context& c)->bool{
            NU::tuple<bool, char, long> t0(NU::tuple<int, int, int>(true, 'b', 314));
            return KT::assert_equal(c, "t0<0> value", NU::get<0>(t0), true)
                && KT::assert_equal(c, "t0<1> value", NU::get<1>(t0), 'b')
                && KT::assert_equal(c, "t0<2> value", NU::get<2>(t0), 314)
                ;
        }),
    KT::expect_success("make_tuple()", [](KT::context& c)->bool{
#ifndef KUHLLIB_INTEL
            constexpr
#else
            const
#endif
            NU::tuple<bool, char, long> t0(NU::make_tuple(true, 'b', 314));
            return KT::assert_equal(c, "t0<0> value", NU::get<0>(t0), true)
                && KT::assert_equal(c, "t0<1> value", NU::get<1>(t0), 'b')
                && KT::assert_equal(c, "t0<2> value", NU::get<2>(t0), 314)
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("utility::tuple", ac, av, ::tests);
}
