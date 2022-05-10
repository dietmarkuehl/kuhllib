// nstd/type_traits/add_volatile.t.cpp                                -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2022 Dietmar Kuehl http://www.dietmar-kuehl.de
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

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace TT = ::nstd::type_traits;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    namespace {
        struct type {};
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("add_volatile to reference type does nothing", []{
           return KT::type<TD::type&> == KT::type<TT::add_volatile<TD::type&>::type>
               && KT::type<TD::type&> == KT::type<TT::add_volatile_t<TD::type&>>
               ;
        }),
    KT::expect_success("add_volatile to function type does nothing", []{
           return KT::type<TD::type()> == KT::type<TT::add_volatile<TD::type()>::type>
               && KT::type<TD::type()> == KT::type<TT::add_volatile_t<TD::type()>>
               ;
        }),
    KT::expect_success("add_volatile to non-volatile value type adds volatile", []{
           return KT::type<TD::type volatile> == KT::type<TT::add_volatile<TD::type>::type>
               && KT::type<TD::type volatile> == KT::type<TT::add_volatile_t<TD::type>>
               ;
        }),
    KT::expect_success("add_volatile to const value type adds volatile", []{
           return KT::type<TD::type const volatile> == KT::type<TT::add_volatile<TD::type const>::type>
               && KT::type<TD::type const volatile> == KT::type<TT::add_volatile_t<TD::type const>>
               ;
        }),
    KT::expect_success("add_volatile to volatile value type does nothing ", []{
           return KT::type<TD::type volatile> == KT::type<TT::add_volatile<TD::type volatile>::type>
               && KT::type<TD::type volatile> == KT::type<TT::add_volatile_t<TD::type volatile>>
               ;
        }),
    KT::expect_success("add_volatile to const volatile value type does nothing ", []{
           return KT::type<TD::type const volatile> == KT::type<TT::add_volatile<TD::type const volatile>::type>
               && KT::type<TD::type const volatile> == KT::type<TT::add_volatile_t<TD::type const volatile>>
               ;
        }),
    KT::expect_success("add_volatile to non-volatile array type adds volatile", []{
           return KT::type<TD::type volatile[1]> == KT::type<TT::add_volatile<TD::type[1]>::type>
               && KT::type<TD::type volatile[1]> == KT::type<TT::add_volatile_t<TD::type[1]>>
               ;
        }),
    KT::expect_success("add_volatile to const array type adds volatile", []{
           return KT::type<TD::type const volatile[1]> == KT::type<TT::add_volatile<TD::type const[1]>::type>
               && KT::type<TD::type const volatile[1]> == KT::type<TT::add_volatile_t<TD::type const[1]>>
               ;
        }),
    KT::expect_success("add_volatile to volatile array type does nothing ", []{
           return KT::type<TD::type volatile[1]> == KT::type<TT::add_volatile<TD::type volatile[1]>::type>
               && KT::type<TD::type volatile[1]> == KT::type<TT::add_volatile_t<TD::type volatile[1]>>
               ;
        }),
    KT::expect_success("add_volatile to const volatile array type does nothing ", []{
           return KT::type<TD::type const volatile[1]> == KT::type<TT::add_volatile<TD::type const volatile[1]>::type>
               && KT::type<TD::type const volatile[1]> == KT::type<TT::add_volatile_t<TD::type const volatile[1]>>
               ;
        }),
};

static KT::add_tests suite("[meta.trans.cv]::add_volatile", ::tests);
