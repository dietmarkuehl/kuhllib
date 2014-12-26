// nstd/type_traits/is_function.t.cpp                                 -*-C++-*-
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

#include "nstd/type_traits/is_function.hpp"
#include "kuhl/test.hpp"

namespace NT = nstd::type_traits;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct foo;
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("non-functions", [](KT::context& c)->bool{
            constexpr bool f{NT::is_function<foo>::value};
            constexpr bool fa{NT::is_function<foo[17]>::value};
            constexpr bool fu{NT::is_function<foo[]>::value};
            return KT::assert_false(c, "foo", f)
                && KT::assert_false(c, "foo[17]", fa)
                && KT::assert_false(c, "foo[]", fu)
                ;
        }),
    KT::expect_success("functions", [](KT::context& c)->bool{
            constexpr bool f{NT::is_function<auto () -> foo>::value};
            constexpr bool fa{NT::is_function<auto (bool) -> foo>::value};
            constexpr bool fu{NT::is_function<auto (foo) -> foo>::value};
            return KT::assert_true(c, "auto () -> foo", f)
                && KT::assert_true(c, "auto (bool) -> foo", fa)
                && KT::assert_true(c, "auto (foo) -> foo", fu)
                ;
        }),
    KT::expect_success("variable functions", [](KT::context& c)->bool{
            constexpr bool f{NT::is_function<auto (...) -> foo>::value};
            constexpr bool fa{NT::is_function<auto (bool, ...) -> foo>::value};
            constexpr bool fu{NT::is_function<auto (foo, bool, ...) -> foo>::value};
            return KT::assert_true(c, "auto (, ...) -> foo", f)
                && KT::assert_true(c, "auto (bool, ...) -> foo", fa)
                && KT::assert_true(c, "auto (foo, bool, ...) -> foo", fu)
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("type_traits::is_function", ac, av, ::tests);
}
