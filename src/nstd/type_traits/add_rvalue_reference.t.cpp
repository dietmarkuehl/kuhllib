// nstd/type_traits/add_rvalue_reference.t.cpp                        -*-C++-*-
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

#include "nstd/type_traits/add_rvalue_reference.hpp"
#include "nstd/type_traits/is_same.hpp"
#include "kuhl/test.hpp"

namespace NT = nstd::type_traits;

// ----------------------------------------------------------------------------

namespace
{
    struct foo;
}

// ----------------------------------------------------------------------------

static kuhl::test::testcase const tests[] = {
    kuhl::test::expect_success("non-reference types get an rvalue reference", []()->bool{
            return NT::is_same<foo&&, typename NT::add_rvalue_reference<foo>::type>::value
                && NT::is_same<foo&&, NT::add_rvalue_reference_t<foo>>::value
                ;
        }),
    kuhl::test::expect_success("function types get an rvalue reference", []()->bool{
            return NT::is_same<foo(&&)(foo), typename NT::add_rvalue_reference<foo(foo)>::type>::value
                && NT::is_same<foo(&&)(foo), NT::add_rvalue_reference_t<foo(foo)>>::value
                ;
        }),
    kuhl::test::expect_success("reference types stay reference types", []()->bool{
            return NT::is_same<foo&, typename NT::add_rvalue_reference<foo&>::type>::value
                && NT::is_same<foo&, NT::add_rvalue_reference_t<foo&>>::value
                ;
        }),
    kuhl::test::expect_success("const reference types stay const reference types", []()->bool{
            return NT::is_same<foo const&, typename NT::add_rvalue_reference<foo const&>::type>::value
                && NT::is_same<foo const&, NT::add_rvalue_reference_t<foo const&>>::value
                ;
        }),
};

int main(int ac, char* av[])
{
    return kuhl::test::run_tests("type_traits::add_rvalue_reference", ac, av, ::tests);
}
