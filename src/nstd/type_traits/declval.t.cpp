// nstd/type_traits/declval.t.cpp                                     -*-C++-*-
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

#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/is_same.hpp"
#include "kuhl/test.hpp"

namespace NT = nstd::type_traits;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    struct foo;
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("declval for a value is an rvalue", []()->bool{
            return NT::is_same<foo&&, decltype(NT::declval<foo>())>::value;
        }),
    KT::expect_success("declval for a const value is a const rvalue", []()->bool{
            return NT::is_same<foo const&&, decltype(NT::declval<foo const>())>::value;
        }),
    KT::expect_success("declval for a volatile value is a volatile rvalue", []()->bool{
            return NT::is_same<foo volatile&&, decltype(NT::declval<foo volatile>())>::value;
        }),
    KT::expect_success("declval for a const volatile value is a const volatile rvalue", []()->bool{
            return NT::is_same<foo const volatile&&, decltype(NT::declval<foo const volatile>())>::value;
        }),
    KT::expect_success("declval for an lvalue stays an lvalue", []()->bool{
            return NT::is_same<foo&, decltype(NT::declval<foo&>())>::value;
        }),
    KT::expect_success("declval for a const lvalue stays a const lvalue", []()->bool{
            return NT::is_same<foo const&, decltype(NT::declval<foo const&>())>::value;
        }),
    KT::expect_success("declval for a volatile lvalue stays a volatile lvalue", []()->bool{
            return NT::is_same<foo volatile&, decltype(NT::declval<foo volatile&>())>::value;
        }),
    KT::expect_success("declval for a const volatile lvalue stays a const volatile lvalue", []()->bool{
            return NT::is_same<foo const volatile&, decltype(NT::declval<foo const volatile&>())>::value;
        }),
    KT::expect_success("declval for rvalue doesn't throw", [](KT::context& c)->bool{
            return KT::assert_true(c, noexcept(NT::declval<foo>()))
                ;
        }),
    KT::expect_success("declval for lvalue doesn't throw", [](KT::context& c)->bool{
            return KT::assert_true(c, noexcept(NT::declval<foo&>()))
                ;
        }),
    KT::expect_success("declval for lvalue const doesn't throw", [](KT::context& c)->bool{
            return KT::assert_true(c, noexcept(NT::declval<foo const &>()))
                ;
        }),
    KT::expect_success("declval for lvalue volatile doesn't throw", [](KT::context& c)->bool{
            return KT::assert_true(c, noexcept(NT::declval<foo volatile &>()))
                ;
        }),
    KT::expect_success("declval for lvalue const volatile doesn't throw", [](KT::context& c)->bool{
            return KT::assert_true(c, noexcept(NT::declval<foo const volatile &>()))
                ;
        }),
};

static KT::add_tests suite("type_traits::declval()", ::tests);
