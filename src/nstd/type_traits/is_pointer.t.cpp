// nstd/type_traits/is_pointer.t.cpp                                  -*-C++-*-
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

#include "nstd/type_traits/is_pointer.hpp"
#include "kuhl/test.hpp"

namespace NT = nstd::type_traits;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct foo;
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("non-pointer are not classified as pointers", [](KT::context& c)->bool{
            return KT::assert_false(c, "foo", NT::is_pointer<foo>::value)
                && KT::assert_false(c, "foo const", NT::is_pointer<foo const>::value)
                && KT::assert_false(c, "foo volatile", NT::is_pointer<foo volatile>::value)
                && KT::assert_false(c, "foo const volatile", NT::is_pointer<foo const volatile>::value)
                && KT::assert_false(c, "foo&", NT::is_pointer<foo&>::value)
                && KT::assert_false(c, "foo(foo)", NT::is_pointer<foo(foo)>::value)
                ;
        }),
    KT::expect_success("pointer-to-member are not classified as pointers", [](KT::context& c)->bool{
            return KT::assert_false(c, "int foo::*", NT::is_pointer<int foo::*>::value)
                && KT::assert_false(c, "int foo::* const", NT::is_pointer<int foo::* const>::value)
                && KT::assert_false(c, "int foo::* volatile", NT::is_pointer<int foo::* volatile>::value)
                && KT::assert_false(c, "int foo::* const volatile", NT::is_pointer<int foo::* const volatile>::value)
                ;
        }),
    KT::expect_success("pointer-to-member are not classified as pointers", [](KT::context& c)->bool{
            return KT::assert_false(c, "int foo::*", NT::is_pointer<int foo::*>::value)
                && KT::assert_false(c, "int foo::* const", NT::is_pointer<int foo::* const>::value)
                && KT::assert_false(c, "int foo::* volatile", NT::is_pointer<int foo::* volatile>::value)
                && KT::assert_false(c, "int foo::* const volatile", NT::is_pointer<int foo::* const volatile>::value)
                ;
        }),
    KT::expect_success("pointers are classified as such", [](KT::context& c)->bool{
            return KT::assert_true(c, "foo*", NT::is_pointer<foo*>::value)
                && KT::assert_true(c, "foo* const", NT::is_pointer<foo* const>::value)
                && KT::assert_true(c, "foo* volatile", NT::is_pointer<foo* volatile>::value)
                && KT::assert_true(c, "foo* const volatile", NT::is_pointer<foo* const volatile>::value)
                && KT::assert_true(c, "foo const*", NT::is_pointer<foo const*>::value)
                && KT::assert_true(c, "foo volatile*", NT::is_pointer<foo volatile*>::value)
                && KT::assert_true(c, "foo const volatile*", NT::is_pointer<foo const volatile*>::value)
                ;
        }),
    KT::expect_success("function pointers are classified as such", [](KT::context& c)->bool{
            return KT::assert_true(c, "foo(*)()", NT::is_pointer<foo(*)()>::value)
                && KT::assert_true(c, "foo(* const)()", NT::is_pointer<foo(* const)()>::value)
                && KT::assert_true(c, "foo(* volatile)()", NT::is_pointer<foo(* volatile)()>::value)
                && KT::assert_true(c, "foo(* const volatile)()", NT::is_pointer<foo(* const volatile)()>::value)
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("type_traits::is_pointer", ac, av, ::tests);
}
