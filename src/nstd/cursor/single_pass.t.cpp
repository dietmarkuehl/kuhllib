// nstd/cursor/single_pass.t.cpp                                      -*-C++-*-
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

#include "nstd/cursor/single_pass.hpp"
#include "nstd/cursor/array.hpp"
#include "nstd/utility/pair.hpp"
#include <kuhl/test.hpp>

namespace NC = nstd::cursor;
namespace NU = nstd::utility;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    template <typename T, T Value>
    auto test_integer(KT::context& c) -> bool {
        T begin{Value};
        T it{begin};

        nstd::cursor::step(it);
        return KT::assert_type<T, decltype(nstd::cursor::key(begin))>(c, "key type")
            && KT::assert_type<bool, decltype(nstd::cursor::at_same_pos(begin, it))>(c, "at_same_pos type")
            && KT::assert_type<void, decltype(nstd::cursor::step(begin))>(c, "step type")
            && KT::assert_equal(c, "key value (at begin)", nstd::cursor::key(begin),  Value)
            && KT::assert_equal(c, "key value (at it)", nstd::cursor::key(it), Value + 1)
            && KT::assert_false(c, "not same pos: begin and it", nstd::cursor::at_same_pos(begin, it))
            && KT::assert_false(c, "not same pos: Value and it", nstd::cursor::at_same_pos(Value, it))
            && KT::assert_true(c, "same pos: it and Value + 1", nstd::cursor::at_same_pos(it, Value + 1))
            && KT::assert_true(c, "same pos: it and it", nstd::cursor::at_same_pos(it, it))
            ;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("operations on pointers", [](KT::context& c)->bool{
            int array[] = { 17 };
            int* begin{nstd::cursor::begin(array)};
            int* it{nstd::cursor::begin(array)};

            nstd::cursor::step(it);
            return KT::assert_type<int&, decltype(nstd::cursor::key(begin))>(c, "key type")
                && KT::assert_type<bool, decltype(nstd::cursor::at_same_pos(begin, it))>(c, "same pos type")
                && KT::assert_type<void, decltype(nstd::cursor::step(begin))>(c, "step type")
                && KT::assert_equal(c, "key result", nstd::cursor::key(begin), 17)
                && KT::assert_equal(c, "key(begin(array)", nstd::cursor::key(nstd::cursor::begin(array)), 17)
                && KT::assert_false(c, "begin/end array", nstd::cursor::at_same_pos(nstd::cursor::begin(array), nstd::cursor::end(array)))
                && KT::assert_false(c, "begin/it", nstd::cursor::at_same_pos(nstd::cursor::begin(array), it))
                && KT::assert_true(c, "it/end", nstd::cursor::at_same_pos(it, nstd::cursor::end(array)))
                && KT::assert_true(c, "it/it", nstd::cursor::at_same_pos(it, it))
                ;
        }),
    KT::expect_success("operations on char", &test_integer<char, 123>),
    KT::expect_success("operations on wchar_t", &test_integer<wchar_t, 123>),
    KT::expect_success("operations on char16_t", &test_integer<char16_t, 123>),
    KT::expect_success("operations on char32_t", &test_integer<char32_t, 123>),
    KT::expect_success("operations on unsigned char", &test_integer<unsigned char, 123>),
    KT::expect_success("operations on signed char", &test_integer<signed char, 123>),
    KT::expect_success("operations on unsigned short", &test_integer<unsigned short, 1023>),
    KT::expect_success("operations on signed short", &test_integer<signed short, 1023>),
    KT::expect_success("operations on unsigned int", &test_integer<unsigned int, 1023>),
    KT::expect_success("operations on signed int", &test_integer<signed int, 1023>),
    KT::expect_success("operations on unsigned long", &test_integer<unsigned long, 1023>),
    KT::expect_success("operations on signed long", &test_integer<signed long, 1023>),
    KT::expect_success("operations on unsigned long long", &test_integer<unsigned long long, 1023>),
    KT::expect_success("operations on signed long long", &test_integer<signed long long, 1023>),
    KT::expect_success("pair of cursors at same pos", [](KT::context& c) {
            int  array0[] = { 17 };
            long array1[] = { 18 };
            auto begin0(nstd::cursor::begin(array0));
            auto cur0(nstd::cursor::begin(array0));
            nstd::cursor::step(cur0);
            auto end0(nstd::cursor::end(array0));

            auto begin1(nstd::cursor::begin(array1));
            auto cur1(nstd::cursor::begin(array1));
            nstd::cursor::step(cur1);
            auto end1(nstd::cursor::end(array1));

            return KT::assert_true(c, "begin/begin and begin/begin",
                                   NC::at_same_pos(NU::make_pair(begin0, begin1), NU::make_pair(begin0, begin1)))
                && KT::assert_false(c, "begin/begin and begin/cur",
                                   NC::at_same_pos(NU::make_pair(begin0, begin1), NU::make_pair(begin0, cur1)))
                && KT::assert_true(c, "begin/cur and begin/end",
                                   NC::at_same_pos(NU::make_pair(begin0, cur1), NU::make_pair(begin0, end1)))
                &&  KT::assert_false(c, "begin/begin and cur/begin",
                                   NC::at_same_pos(NU::make_pair(begin0, begin1), NU::make_pair(cur0, begin1)))
                && KT::assert_false(c, "begin/begin and cur/cur",
                                   NC::at_same_pos(NU::make_pair(begin0, begin1), NU::make_pair(cur0, cur1)))
                && KT::assert_false(c, "begin/cur and cur/end",
                                   NC::at_same_pos(NU::make_pair(begin0, cur1), NU::make_pair(cur0, end1)))
                &&  KT::assert_true(c, "cur/begin and end/begin",
                                   NC::at_same_pos(NU::make_pair(cur0, begin1), NU::make_pair(end0, begin1)))
                && KT::assert_false(c, "cur/begin and end/cur",
                                   NC::at_same_pos(NU::make_pair(cur0, begin1), NU::make_pair(end0, cur1)))
                && KT::assert_true(c, "cur/cur and end/end",
                                   NC::at_same_pos(NU::make_pair(cur0, cur1), NU::make_pair(end0, end1)))
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("single-pass cursor functionality",
                                 ac, av, ::tests);
}
