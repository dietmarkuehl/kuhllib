// nstd/type_traits/is_member_function_pointer.t.cpp                  -*-C++-*-
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

#include "nstd/type_traits/is_member_function_pointer.hpp"
#include "kuhl/test.hpp"

namespace NT = nstd::type_traits;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct foo;
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("normal types and pointers aren't member pointers", [](KT::context& c)->bool{
            constexpr bool f{NT::is_member_function_pointer<foo>::value};
            constexpr bool fr{NT::is_member_function_pointer<foo&>::value};
            constexpr bool fp{NT::is_member_function_pointer<foo*>::value};
            constexpr bool fc{NT::is_member_function_pointer<foo const>::value};
            constexpr bool fcr{NT::is_member_function_pointer<foo const&>::value};
            constexpr bool fcp{NT::is_member_function_pointer<foo const*>::value};
            return KT::assert_false(c, "foo", f)
                && KT::assert_false(c, "foo&", fr)
                && KT::assert_false(c, "foo*", fp)
                && KT::assert_false(c, "foo const", fc)
                && KT::assert_false(c, "foo const&", fcr)
                && KT::assert_false(c, "foo const*", fcp)
                ;
        }),
    KT::expect_success("member data pointers do not qualify", [](KT::context& c)->bool{
            constexpr bool imp{NT::is_member_function_pointer<int foo::*>::value};
            constexpr bool icmp{NT::is_member_function_pointer<int const foo::*>::value};
            constexpr bool ivmp{NT::is_member_function_pointer<int volatile foo::*>::value};
            constexpr bool icvmp{NT::is_member_function_pointer<int const volatile foo::*>::value};
            return KT::assert_false(c, "int foo::*", imp)
                && KT::assert_false(c, "int const foo::*", icmp)
                && KT::assert_false(c, "int volatile foo::*", ivmp)
                && KT::assert_false(c, "int const volatile foo::*", icvmp)
                ;
        }),
    KT::expect_success("member function pointers qualify", [](KT::context& c)->bool{
            constexpr bool mp0{NT::is_member_function_pointer<int (foo::*)()>::value};
            constexpr bool mp1{NT::is_member_function_pointer<int (foo::*)(int)>::value};
            constexpr bool mp2{NT::is_member_function_pointer<int (foo::*)(int, bool)>::value};
            return KT::assert_true(c, "int (foo::*)()", mp0)
                && KT::assert_true(c, "int (foo::*)(int)", mp1)
                && KT::assert_true(c, "int (foo::*)(int, bool)", mp2)
                ;
        }),
    KT::expect_success("member function pointers (variable) qualify", [](KT::context& c)->bool{
            constexpr bool mp0{NT::is_member_function_pointer<int (foo::*)(...)>::value};
            constexpr bool mp1{NT::is_member_function_pointer<int (foo::*)(int, ...)>::value};
            constexpr bool mp2{NT::is_member_function_pointer<int (foo::*)(int, bool, ...)>::value};
            return KT::assert_true(c, "int (foo::*)(...)", mp0)
                && KT::assert_true(c, "int (foo::*)(int, ...)", mp1)
                && KT::assert_true(c, "int (foo::*)(int, bool, ...)", mp2)
                ;
        }),
};


int main(int ac, char* av[])
{
    return KT::run_tests("type_traits::is_member_function_pointer", ac, av, ::tests);
}
