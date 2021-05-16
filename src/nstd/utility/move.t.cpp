// nstd/utility/move.t.cpp                                            -*-C++-*-
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

#include "nstd/utility/move.hpp"
#include "nstd/type_traits/declval.hpp"
#include "kuhl/test.hpp"

namespace NT = nstd::type_traits;
namespace NU = nstd::utility;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    struct foo {
        foo() = default;
        foo(foo const&) = delete;
        foo(foo&&) = delete;
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("move() value yields an rvalue reference", [](KT::context& c)->bool{
            return KT::assert_type<foo&&, decltype(NU::move(NT::declval<foo>()))>(c, "type (declval)")
                && KT::assert_type<foo&&, decltype(NU::move(foo()))>(c, "type (temporary)")
                ;
        }),
    KT::expect_success("move() const value yields a const rvalue reference", [](KT::context& c)->bool{
            return KT::assert_type<foo const&&, decltype(NU::move(NT::declval<foo const>()))>(c, "type")
                ;
        }),
    KT::expect_success("move() reference yields a rvalue reference", [](KT::context& c)->bool{
            foo object{};
            return KT::assert_type<foo&&, decltype(NU::move(NT::declval<foo&>()))>(c, "type (declval)")
                && KT::assert_type<foo&&, decltype(NU::move(object))>(c, "type (object)")
                ;
        }),
    KT::expect_success("move() const reference yields a const rvalue reference", [](KT::context& c)->bool{
            foo const object{};
            return KT::assert_type<foo const&&, decltype(NU::move(NT::declval<foo const&>()))>(c, "type (declval)")
                && KT::assert_type<foo const&&, decltype(NU::move(object))>(c, "type (object)")
                ;
        }),
};

static KT::add_tests suite("utility::move", ::tests);
