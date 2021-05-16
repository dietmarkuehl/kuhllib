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
#include "kuhl/test.hpp"

namespace NT = nstd::type_traits;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    struct foo;
}

// ----------------------------------------------------------------------------

static kuhl::test::testcase const tests[] = {
    kuhl::test::expect_success("void stays void", [](KT::context& c)->bool{
            return KT::assert_type<void, NT::add_rvalue_reference<void>::type>(c, "add_rvalue_reference void")
                && KT::assert_type<void, NT::add_rvalue_reference_t<void>>(c, "add_rvalue_reference_t void")
                && KT::assert_type<void const, NT::add_rvalue_reference<void const>::type>(c, "add_rvalue_reference void const")
                && KT::assert_type<void const, NT::add_rvalue_reference_t<void const>>(c, "add_rvalue_reference_t void const")
                && KT::assert_type<void volatile, NT::add_rvalue_reference<void volatile>::type>(c, "add_rvalue_reference void volatile")
                && KT::assert_type<void volatile, NT::add_rvalue_reference_t<void volatile>>(c, "add_rvalue_reference_t void volatile")
                && KT::assert_type<void const volatile, NT::add_rvalue_reference<void const volatile>::type>(c, "add_rvalue_reference void const volatile")
                && KT::assert_type<void const volatile, NT::add_rvalue_reference_t<void const volatile>>(c, "add_rvalue_reference_t void const volatile")
                ;
        }),
    kuhl::test::expect_success("non-reference types get an rvalue reference", [](KT::context& c)->bool{
            return KT::assert_type<foo&&, NT::add_rvalue_reference<foo>::type>(c, "add_rvalue_reference")
                && KT::assert_type<foo&&, NT::add_rvalue_reference_t<foo>>(c, "add_rvalue_reference_t")
                ;
        }),
    kuhl::test::expect_success("function types get an rvalue reference", [](KT::context& c)->bool{
            return KT::assert_type<foo(&&)(foo), NT::add_rvalue_reference<foo(foo)>::type>(c, "add_rvalue_reference")
                && KT::assert_type<foo(&&)(foo), NT::add_rvalue_reference_t<foo(foo)>>(c, "add_rvalue_reference_t")
                ;
        }),
    kuhl::test::expect_success("reference types stay reference types", [](KT::context& c)->bool{
            return KT::assert_type<foo&, NT::add_rvalue_reference<foo&>::type>(c, "add_rvalue_reference")
                && KT::assert_type<foo&, NT::add_rvalue_reference_t<foo&>>(c, "add_rvalue_reference_t")
                ;
        }),
    kuhl::test::expect_success("const reference types stay const reference types", [](KT::context& c)->bool{
            return KT::assert_type<foo const&, NT::add_rvalue_reference<foo const&>::type>(c, "add_rvalue_reference")
                && KT::assert_type<foo const&, NT::add_rvalue_reference_t<foo const&>>(c, "add_rvalue_reference_t")
                ;
        }),
};

static KT::add_tests suite("type_traits::add_rvalue_reference", ::tests);
