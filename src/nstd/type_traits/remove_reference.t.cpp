// nstd/type_traits/remove_reference.t.cpp                            -*-C++-*-
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

#include "nstd/type_traits/remove_reference.hpp"
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
    KT::expect_success("remove_reference: non-reference stays unchanged", [](KT::context& c)->bool{
            return KT::assert_type<foo, typename NT::remove_reference<foo>::type>(c, "foo")
                && KT::assert_type<foo const, typename NT::remove_reference<foo const>::type>(c, "foo const")
                && KT::assert_type<foo volatile, typename NT::remove_reference<foo volatile>::type>(c, "foo volatile")
                && KT::assert_type<foo const volatile, typename NT::remove_reference<foo const volatile>::type>(c, "foo const volatile")
                ;
        }),
    KT::expect_success("remove_reference: lvalue reference gets removed", [](KT::context& c)->bool{
            return KT::assert_type<foo, typename NT::remove_reference<foo&>::type>(c, "foo&")
                && KT::assert_type<foo const, typename NT::remove_reference<foo const&>::type>(c, "foo const&")
                && KT::assert_type<foo volatile, typename NT::remove_reference<foo volatile&>::type>(c, "foo volatile&")
                && KT::assert_type<foo const volatile, typename NT::remove_reference<foo const volatile&>::type>(c, "foo const volatile&")
                ;
        }),
    KT::expect_success("remove_reference: rvalue reference gets removed", [](KT::context& c)->bool{
            return KT::assert_type<foo, typename NT::remove_reference<foo&&>::type>(c, "foo&&")
                && KT::assert_type<foo const, typename NT::remove_reference<foo const&&>::type>(c, "foo const&&")
                && KT::assert_type<foo volatile, typename NT::remove_reference<foo volatile&&>::type>(c, "foo volatile&&")
                && KT::assert_type<foo const volatile, typename NT::remove_reference<foo const volatile&&>::type>(c, "foo const volatile&&")
                ;
        }),
    KT::expect_success("remove_reference_t: non-reference stays unchanged", [](KT::context& c)->bool{
            return KT::assert_type<foo, NT::remove_reference_t<foo>>(c, "foo")
                && KT::assert_type<foo const, NT::remove_reference_t<foo const>>(c, "foo const")
                && KT::assert_type<foo volatile, NT::remove_reference_t<foo volatile>>(c, "foo volatile")
                && KT::assert_type<foo const volatile, NT::remove_reference_t<foo const volatile>>(c, "foo const volatile")
                ;
        }),
    KT::expect_success("remove_reference_t: lvalue reference gets removed", [](KT::context& c)->bool{
            return KT::assert_type<foo, NT::remove_reference_t<foo&>>(c, "foo&")
                && KT::assert_type<foo const, NT::remove_reference_t<foo const&>>(c, "foo const&")
                && KT::assert_type<foo volatile, NT::remove_reference_t<foo volatile&>>(c, "foo volatile&")
                && KT::assert_type<foo const volatile, NT::remove_reference_t<foo const volatile&>>(c, "foo const volatile&")
                ;
        }),
    KT::expect_success("remove_reference_t: rvalue reference gets removed", [](KT::context& c)->bool{
            return KT::assert_type<foo, NT::remove_reference_t<foo&&>>(c, "foo&&")
                && KT::assert_type<foo const, NT::remove_reference_t<foo const&&>>(c, "foo const&&")
                && KT::assert_type<foo volatile, NT::remove_reference_t<foo volatile&&>>(c, "foo volatile&&")
                && KT::assert_type<foo const volatile, NT::remove_reference_t<foo const volatile&&>>(c, "foo const volatile&&")
                ;
        }),
};

static KT::add_tests suite("type_traits::remove_reference", ::tests);
