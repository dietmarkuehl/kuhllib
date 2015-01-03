// nstd/type_traits/add_volatile.t.cpp                                -*-C++-*-
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

#include "nstd/type_traits/add_volatile.hpp"
#include "kuhl/test.hpp"

namespace NT = nstd::type_traits;
namespace KT = kuhl::test;

namespace {
    struct foo;
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("reference types stay unchanged", [](KT::context& c)->bool{
            return KT::assert_type<foo&, NT::add_volatile<foo&>::type>(c, "foo& type")
                && KT::assert_type<foo&, NT::add_volatile_t<foo&> >(c, "foo& _t")
                && KT::assert_type<foo const&, NT::add_volatile<foo const&>::type>(c, "foo const& type")
                && KT::assert_type<foo const&, NT::add_volatile_t<foo const&> >(c, "foo const& _t")
                && KT::assert_type<foo volatile&, NT::add_volatile<foo volatile&>::type>(c, "foo volatile& type")
                && KT::assert_type<foo volatile&, NT::add_volatile_t<foo volatile&> >(c, "foo volatile& _t")
                && KT::assert_type<foo const volatile&, NT::add_volatile<foo const volatile&>::type>(c, "foo const volatile& type")
                && KT::assert_type<foo const volatile&, NT::add_volatile_t<foo const volatile&> >(c, "foo const volatile& _t")
                ;
        }),
    KT::expect_success("function types stay unchanged", [](KT::context& c)->bool{
            return KT::assert_type<void(), NT::add_volatile<void()>::type>(c, "void() type")
                && KT::assert_type<void(), NT::add_volatile_t<void()> >(c, "void() _t")
                && KT::assert_type<foo(foo), NT::add_volatile<foo(foo)>::type>(c, "foo(foo) type")
                && KT::assert_type<foo(foo), NT::add_volatile_t<foo(foo)> >(c, "foo(foo) _t")
                && KT::assert_type<bool(char), NT::add_volatile<bool(char)>::type>(c, "bool(char) type")
                && KT::assert_type<bool(char), NT::add_volatile_t<bool(char)> >(c, "bool(char) _t")
                && KT::assert_type<int(double, long), NT::add_volatile<int(double, long)>::type>(c, "int(double, long) type")
                && KT::assert_type<int(double, long), NT::add_volatile_t<int(double, long)> >(c, "int(double, long) _t")
                ;
        }),
    KT::expect_success("volatile types stay unchanged", [](KT::context& c)->bool{
            return KT::assert_type<foo volatile, NT::add_volatile<foo volatile>::type>(c, "foo volatile type")
                && KT::assert_type<foo volatile, NT::add_volatile_t<foo volatile> >(c, "foo volatile _t")
                && KT::assert_type<foo const volatile, NT::add_volatile<foo const volatile>::type>(c, "foo const volatile type")
                && KT::assert_type<foo const volatile, NT::add_volatile_t<foo const volatile> >(c, "foo const volatile _t")
                ;
        }),
    KT::expect_success("other types get volatile added", [](KT::context& c)->bool{
            return KT::assert_type<foo volatile, NT::add_volatile<foo>::type>(c, "foo type")
                && KT::assert_type<foo volatile, NT::add_volatile_t<foo> >(c, "foo _t")
                && KT::assert_type<foo const volatile, NT::add_volatile<foo const>::type>(c, "foo const type")
                && KT::assert_type<foo const volatile, NT::add_volatile_t<foo const> >(c, "foo const _t")
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("type_traits::add_volatile", ac, av, ::tests);
}
