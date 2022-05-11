// nstd/type_traits/copy_cvref.t.cpp                                  -*-C++-*-
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

#include "nstd/type_traits/copy_cvref.hpp"
#include "kuhl/test.hpp"

namespace TT = ::nstd::type_traits;
namespace test_declaration {}
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
        struct from;
        struct to;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("copy_cvref with basic types", []{
            return KT::type<TD::to                 > == KT::type<TT::copy_cvref<TD::from                 , TD::to>::type>
                && KT::type<TD::to&                > == KT::type<TT::copy_cvref<TD::from&                , TD::to>::type>
                && KT::type<TD::to&&               > == KT::type<TT::copy_cvref<TD::from               &&, TD::to>::type>

                && KT::type<TD::to const           > == KT::type<TT::copy_cvref<TD::from const           , TD::to>::type>
                && KT::type<TD::to const&          > == KT::type<TT::copy_cvref<TD::from const         & , TD::to>::type>
                && KT::type<TD::to const&&         > == KT::type<TT::copy_cvref<TD::from const         &&, TD::to>::type>

                && KT::type<TD::to       volatile  > == KT::type<TT::copy_cvref<TD::from       volatile  , TD::to>::type>
                && KT::type<TD::to       volatile& > == KT::type<TT::copy_cvref<TD::from       volatile& , TD::to>::type>
                && KT::type<TD::to       volatile&&> == KT::type<TT::copy_cvref<TD::from       volatile&&, TD::to>::type>

                && KT::type<TD::to const volatile  > == KT::type<TT::copy_cvref<TD::from const volatile  , TD::to>::type>
                && KT::type<TD::to const volatile& > == KT::type<TT::copy_cvref<TD::from const volatile& , TD::to>::type>
                && KT::type<TD::to const volatile&&> == KT::type<TT::copy_cvref<TD::from const volatile&&, TD::to>::type>
                ;
        }),
    KT::expect_success("copy_cvref with lvalue reference", []{
            return KT::type<TD::to&> == KT::type<TT::copy_cvref<TD::from                 , TD::to&>::type>
                && KT::type<TD::to&> == KT::type<TT::copy_cvref<TD::from               & , TD::to&>::type>
                && KT::type<TD::to&> == KT::type<TT::copy_cvref<TD::from               &&, TD::to&>::type>

                && KT::type<TD::to&> == KT::type<TT::copy_cvref<TD::from const           , TD::to&>::type>
                && KT::type<TD::to&> == KT::type<TT::copy_cvref<TD::from const         & , TD::to&>::type>
                && KT::type<TD::to&> == KT::type<TT::copy_cvref<TD::from const         &&, TD::to&>::type>

                && KT::type<TD::to&> == KT::type<TT::copy_cvref<TD::from       volatile  , TD::to&>::type>
                && KT::type<TD::to&> == KT::type<TT::copy_cvref<TD::from       volatile& , TD::to&>::type>
                && KT::type<TD::to&> == KT::type<TT::copy_cvref<TD::from       volatile&&, TD::to&>::type>

                && KT::type<TD::to&> == KT::type<TT::copy_cvref<TD::from const volatile  , TD::to&>::type>
                && KT::type<TD::to&> == KT::type<TT::copy_cvref<TD::from const volatile& , TD::to&>::type>
                && KT::type<TD::to&> == KT::type<TT::copy_cvref<TD::from const volatile&&, TD::to&>::type>
                ;
        }),
    KT::expect_success("copy_cvref with rvalue reference", []{
            return KT::type<TD::to&&> == KT::type<TT::copy_cvref<TD::from                 , TD::to&&>::type>
                && KT::type<TD::to& > == KT::type<TT::copy_cvref<TD::from               & , TD::to&&>::type>
                && KT::type<TD::to&&> == KT::type<TT::copy_cvref<TD::from               &&, TD::to&&>::type>

                && KT::type<TD::to&&> == KT::type<TT::copy_cvref<TD::from const           , TD::to&&>::type>
                && KT::type<TD::to& > == KT::type<TT::copy_cvref<TD::from const         & , TD::to&&>::type>
                && KT::type<TD::to&&> == KT::type<TT::copy_cvref<TD::from const         &&, TD::to&&>::type>

                && KT::type<TD::to&&> == KT::type<TT::copy_cvref<TD::from       volatile  , TD::to&&>::type>
                && KT::type<TD::to& > == KT::type<TT::copy_cvref<TD::from       volatile& , TD::to&&>::type>
                && KT::type<TD::to&&> == KT::type<TT::copy_cvref<TD::from       volatile&&, TD::to&&>::type>

                && KT::type<TD::to&&> == KT::type<TT::copy_cvref<TD::from const volatile  , TD::to&&>::type>
                && KT::type<TD::to& > == KT::type<TT::copy_cvref<TD::from const volatile& , TD::to&&>::type>
                && KT::type<TD::to&&> == KT::type<TT::copy_cvref<TD::from const volatile&&, TD::to&&>::type>
                ;
        }),
    KT::expect_success("copy_cvref_t with basic types", []{
            return KT::type<TD::to                 > == KT::type<TT::copy_cvref_t<TD::from                 , TD::to>>
                && KT::type<TD::to&                > == KT::type<TT::copy_cvref_t<TD::from&                , TD::to>>
                && KT::type<TD::to&&               > == KT::type<TT::copy_cvref_t<TD::from               &&, TD::to>>

                && KT::type<TD::to const           > == KT::type<TT::copy_cvref_t<TD::from const           , TD::to>>
                && KT::type<TD::to const&          > == KT::type<TT::copy_cvref_t<TD::from const         & , TD::to>>
                && KT::type<TD::to const&&         > == KT::type<TT::copy_cvref_t<TD::from const         &&, TD::to>>

                && KT::type<TD::to       volatile  > == KT::type<TT::copy_cvref_t<TD::from       volatile  , TD::to>>
                && KT::type<TD::to       volatile& > == KT::type<TT::copy_cvref_t<TD::from       volatile& , TD::to>>
                && KT::type<TD::to       volatile&&> == KT::type<TT::copy_cvref_t<TD::from       volatile&&, TD::to>>

                && KT::type<TD::to const volatile  > == KT::type<TT::copy_cvref_t<TD::from const volatile  , TD::to>>
                && KT::type<TD::to const volatile& > == KT::type<TT::copy_cvref_t<TD::from const volatile& , TD::to>>
                && KT::type<TD::to const volatile&&> == KT::type<TT::copy_cvref_t<TD::from const volatile&&, TD::to>>
                ;
        }),
    KT::expect_success("copy_cvref_t with lvalue reference", []{
            return KT::type<TD::to&> == KT::type<TT::copy_cvref_t<TD::from                 , TD::to&>>
                && KT::type<TD::to&> == KT::type<TT::copy_cvref_t<TD::from               & , TD::to&>>
                && KT::type<TD::to&> == KT::type<TT::copy_cvref_t<TD::from               &&, TD::to&>>

                && KT::type<TD::to&> == KT::type<TT::copy_cvref_t<TD::from const           , TD::to&>>
                && KT::type<TD::to&> == KT::type<TT::copy_cvref_t<TD::from const         & , TD::to&>>
                && KT::type<TD::to&> == KT::type<TT::copy_cvref_t<TD::from const         &&, TD::to&>>

                && KT::type<TD::to&> == KT::type<TT::copy_cvref_t<TD::from       volatile  , TD::to&>>
                && KT::type<TD::to&> == KT::type<TT::copy_cvref_t<TD::from       volatile& , TD::to&>>
                && KT::type<TD::to&> == KT::type<TT::copy_cvref_t<TD::from       volatile&&, TD::to&>>

                && KT::type<TD::to&> == KT::type<TT::copy_cvref_t<TD::from const volatile  , TD::to&>>
                && KT::type<TD::to&> == KT::type<TT::copy_cvref_t<TD::from const volatile& , TD::to&>>
                && KT::type<TD::to&> == KT::type<TT::copy_cvref_t<TD::from const volatile&&, TD::to&>>
                ;
        }),
    KT::expect_success("copy_cvref_t with rvalue reference", []{
            return KT::type<TD::to&&> == KT::type<TT::copy_cvref_t<TD::from                 , TD::to&&>>
                && KT::type<TD::to& > == KT::type<TT::copy_cvref_t<TD::from               & , TD::to&&>>
                && KT::type<TD::to&&> == KT::type<TT::copy_cvref_t<TD::from               &&, TD::to&&>>

                && KT::type<TD::to&&> == KT::type<TT::copy_cvref_t<TD::from const           , TD::to&&>>
                && KT::type<TD::to& > == KT::type<TT::copy_cvref_t<TD::from const         & , TD::to&&>>
                && KT::type<TD::to&&> == KT::type<TT::copy_cvref_t<TD::from const         &&, TD::to&&>>

                && KT::type<TD::to&&> == KT::type<TT::copy_cvref_t<TD::from       volatile  , TD::to&&>>
                && KT::type<TD::to& > == KT::type<TT::copy_cvref_t<TD::from       volatile& , TD::to&&>>
                && KT::type<TD::to&&> == KT::type<TT::copy_cvref_t<TD::from       volatile&&, TD::to&&>>

                && KT::type<TD::to&&> == KT::type<TT::copy_cvref_t<TD::from const volatile  , TD::to&&>>
                && KT::type<TD::to& > == KT::type<TT::copy_cvref_t<TD::from const volatile& , TD::to&&>>
                && KT::type<TD::to&&> == KT::type<TT::copy_cvref_t<TD::from const volatile&&, TD::to&&>>
                ;
        }),
};

static KT::add_tests suite("nstd/type_traits/copy_cvref", ::tests);
