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
#include <kuhl/test.hpp>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace
{
    template <typename T, T Value>
    auto test_integer() -> bool {
        T begin{Value};
        T it{begin};

        nstd::cursor::step(it);
        return std::is_same<T, decltype(nstd::cursor::key(begin))>::value
            && std::is_same<bool, decltype(nstd::cursor::at_same_pos(begin, it))>::value
            && std::is_same<void, decltype(nstd::cursor::step(begin))>::value
            && nstd::cursor::key(begin) == Value
            && nstd::cursor::key(it) == Value + 1
            && !nstd::cursor::at_same_pos(begin, it)
            && !nstd::cursor::at_same_pos(Value, it)
            && nstd::cursor::at_same_pos(it, Value + 1)
            && nstd::cursor::at_same_pos(it, it)
            ;
    }
}

// ----------------------------------------------------------------------------

static kuhl::test::testcase const tests[] = {
    kuhl::test::expect_success("operations on pointers", []()->bool{
            int array[] = { 17 };
            int* begin{nstd::cursor::begin(array)};
            int* it{nstd::cursor::begin(array)};

            nstd::cursor::step(it);
            return std::is_same<int&, decltype(nstd::cursor::key(begin))>::value
                && std::is_same<bool, decltype(nstd::cursor::at_same_pos(begin, it))>::value
                && std::is_same<void, decltype(nstd::cursor::step(begin))>::value
                && nstd::cursor::key(begin) == 17
                && nstd::cursor::key(nstd::cursor::begin(array)) == 17
                && !nstd::cursor::at_same_pos(nstd::cursor::begin(array), nstd::cursor::end(array))
                && !nstd::cursor::at_same_pos(nstd::cursor::begin(array), it)
                && nstd::cursor::at_same_pos(it, nstd::cursor::end(array))
                && nstd::cursor::at_same_pos(it, it)
                ;
        }),
    kuhl::test::expect_success("operations on char", &test_integer<char, 123>),
    kuhl::test::expect_success("operations on wchar_t", &test_integer<wchar_t, 123>),
    kuhl::test::expect_success("operations on char16_t", &test_integer<char16_t, 123>),
    kuhl::test::expect_success("operations on char32_t", &test_integer<char32_t, 123>),
    kuhl::test::expect_success("operations on unsigned char", &test_integer<unsigned char, 123>),
    kuhl::test::expect_success("operations on signed char", &test_integer<signed char, 123>),
    kuhl::test::expect_success("operations on unsigned short", &test_integer<unsigned short, 1023>),
    kuhl::test::expect_success("operations on signed short", &test_integer<signed short, 1023>),
    kuhl::test::expect_success("operations on unsigned int", &test_integer<unsigned int, 1023>),
    kuhl::test::expect_success("operations on signed int", &test_integer<signed int, 1023>),
    kuhl::test::expect_success("operations on unsigned long", &test_integer<unsigned long, 1023>),
    kuhl::test::expect_success("operations on signed long", &test_integer<signed long, 1023>),
    kuhl::test::expect_success("operations on unsigned long long", &test_integer<unsigned long long, 1023>),
    kuhl::test::expect_success("operations on signed long long", &test_integer<signed long long, 1023>),
};

int main(int ac, char* av[])
{
    return kuhl::test::run_tests("single-pass cursor functionality",
                                 ac, av, ::tests);
}
