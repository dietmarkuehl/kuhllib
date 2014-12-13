// nstd/type_traits/is_same.t.cpp                                     -*-C++-*-
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

#include "nstd/type_traits/is_same.hpp"
#include "kuhl/test.hpp"

namespace NT = nstd::type_traits;

// ----------------------------------------------------------------------------

namespace
{
    // This does roughly what nstd::type_traits::is_same does but it seems
    // like testing the implementation with itself seems wrong.

    template <typename S, typename T>
    struct is_same {
        static constexpr bool value{false};
    };
    template <typename T>
    struct is_same<T, T> {
        static constexpr bool value{true};
    };
}

// ----------------------------------------------------------------------------

namespace
{
    struct foo;
    struct bar;
}

// ----------------------------------------------------------------------------

static kuhl::test::testcase const tests[] = {
    kuhl::test::expect_success("is_same<foo, bar>", []()->bool{
            constexpr bool value{NT::is_same<foo, bar>::value};
            constexpr NT::is_same<foo, bar> object;
            NT::false_type const* ptr = &object;
            return !is_same<NT::is_same<foo, bar>, NT::false_type>::value
                && noexcept(NT::is_same<foo, bar>())
                && value == false
                && ptr;
        }),
    kuhl::test::expect_success("is_same<foo, foo>", []()->bool{
            constexpr bool value{NT::is_same<foo, foo>::value};
            constexpr NT::is_same<foo, foo> object;
            NT::true_type const* ptr = &object;
            return !is_same<NT::is_same<foo, foo>, NT::true_type>::value
                && noexcept(NT::is_same<foo, foo>())
                && value == true
                && ptr;
        }),
    kuhl::test::expect_success("is_same<foo const, foo>", []()->bool{
            constexpr bool value{NT::is_same<foo const, foo>::value};
            return !is_same<NT::is_same<foo const, foo>, NT::false_type>::value
                && value == false;
        }),
};

int main(int ac, char* av[])
{
    return kuhl::test::run_tests("nstd::is_same", ac, av, ::tests);
}
