// nstd/type_traits/is_array.t.cpp                                    -*-C++-*-
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

#include "nstd/type_traits/is_array.hpp"
#include "kuhl/test.hpp"

#if 0
#include <type_traits>
namespace NT = std;
#else
namespace NT = nstd::type_traits;
#endif
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct foo;
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("non-array", [](KT::context& c)->bool{
            constexpr bool ir(NT::is_array<int>::value);
            constexpr bool ifo(NT::is_array<foo>::value);
            constexpr bool if0(NT::is_array<int()>::value);
            constexpr bool if1(NT::is_array<int(int)>::value);
            constexpr bool if2(NT::is_array<int(int, int)>::value);
            constexpr bool mf0(NT::is_array<int(foo::*)()>::value);
            constexpr bool mf1(NT::is_array<int(foo::*)(int)>::value);
            constexpr bool mf2(NT::is_array<int(foo::*)(int, int)>::value);
            return KT::assert_false(c, "int", ir)
                && KT::assert_false(c, "foo", ifo)
                && KT::assert_false(c, "int()", if0)
                && KT::assert_false(c, "int(int)", if1)
                && KT::assert_false(c, "int(int, int)", if2)
                && KT::assert_false(c, "int(foo::*)()", mf0)
                && KT::assert_false(c, "int(foo::*)(int)", mf1)
                && KT::assert_false(c, "int(foo::*)(int, int)", mf2)
                ;
        }),
    KT::expect_success("sized array", [](KT::context& c)->bool{
            constexpr bool a1(NT::is_array<foo[1]>::value);
            constexpr bool a2(NT::is_array<foo[2]>::value);
            constexpr bool a21(NT::is_array<foo[2][1]>::value);
            constexpr bool a22(NT::is_array<foo[2][2]>::value);
            constexpr bool a221(NT::is_array<foo[2][2][1]>::value);
            constexpr bool a222(NT::is_array<foo[2][2][2]>::value);
            return KT::assert_true(c, "foo[1]", a1)
                && KT::assert_true(c, "foo[2]", a2)
                && KT::assert_true(c, "foo[2][1]", a21)
                && KT::assert_true(c, "foo[2][2]", a22)
                && KT::assert_true(c, "foo[2][2][1]", a221)
                && KT::assert_true(c, "foo[2][2][2]", a222)
                ;
        }),
    KT::expect_success("sized array reference", [](KT::context& c)->bool{
            constexpr bool a1(NT::is_array<foo(&)[1]>::value);
            constexpr bool a2(NT::is_array<foo(&)[2]>::value);
            constexpr bool a21(NT::is_array<foo(&)[2][1]>::value);
            constexpr bool a22(NT::is_array<foo(&)[2][2]>::value);
            constexpr bool a221(NT::is_array<foo(&)[2][2][1]>::value);
            constexpr bool a222(NT::is_array<foo(&)[2][2][2]>::value);
            return KT::assert_false(c, "foo(&)[1]", a1)
                && KT::assert_false(c, "foo(&)[2]", a2)
                && KT::assert_false(c, "foo(&)[2][1]", a21)
                && KT::assert_false(c, "foo(&)[2][2]", a22)
                && KT::assert_false(c, "foo(&)[2][2][1]", a221)
                && KT::assert_false(c, "foo(&)[2][2][2]", a222)
                ;
        }),
    KT::expect_success("sized array pointer", [](KT::context& c)->bool{
            constexpr bool a1(NT::is_array<foo(*)[1]>::value);
            constexpr bool a2(NT::is_array<foo(*)[2]>::value);
            constexpr bool a21(NT::is_array<foo(*)[2][1]>::value);
            constexpr bool a22(NT::is_array<foo(*)[2][2]>::value);
            constexpr bool a221(NT::is_array<foo(*)[2][2][1]>::value);
            constexpr bool a222(NT::is_array<foo(*)[2][2][2]>::value);
            return KT::assert_false(c, "foo(*)[1]", a1)
                && KT::assert_false(c, "foo(*)[2]", a2)
                && KT::assert_false(c, "foo(*)[2][1]", a21)
                && KT::assert_false(c, "foo(*)[2][2]", a22)
                && KT::assert_false(c, "foo(*)[2][2][1]", a221)
                && KT::assert_false(c, "foo(*)[2][2][2]", a222)
                ;
        }),
    KT::expect_success("unsized array", [](KT::context& c)->bool{
            constexpr bool au(NT::is_array<foo[]>::value);
            constexpr bool au2(NT::is_array<foo[][2]>::value);
            constexpr bool au22(NT::is_array<foo[][2][2]>::value);
            return KT::assert_true(c, "foo[]", au)
                && KT::assert_true(c, "foo[][2]", au2)
                && KT::assert_true(c, "foo[][2][2]", au22)
                ;
        }),
    KT::expect_success("unsized array reference", [](KT::context& c)->bool{
            constexpr bool au(NT::is_array<foo(&)[]>::value);
            constexpr bool au2(NT::is_array<foo(&)[][2]>::value);
            constexpr bool au22(NT::is_array<foo(&)[][2][2]>::value);
            return KT::assert_false(c, "foo(&)[]", au)
                && KT::assert_false(c, "foo(&)[][2]", au2)
                && KT::assert_false(c, "foo(&)[][2][2]", au22)
                ;
        }),
    KT::expect_success("unsized array pointer", [](KT::context& c)->bool{
            constexpr bool au(NT::is_array<foo(*)[]>::value);
            constexpr bool au2(NT::is_array<foo(*)[][2]>::value);
            constexpr bool au22(NT::is_array<foo(*)[][2][2]>::value);
            return KT::assert_false(c, "foo(*)[]", au)
                && KT::assert_false(c, "foo(*)[][2]", au2)
                && KT::assert_false(c, "foo(*)[][2][2]", au22)
                ;
        }),
};

static KT::add_tests suite("type_traits::is_array", ::tests);
