// nstd/type_traits/copy_cv.t.cpp                                     -*-C++-*-
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

#include "nstd/type_traits/copy_cv.hpp"
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
    KT::expect_success("basic type", []{
            return KT::type<TD::to               > == KT::type<TT::copy_cv  <TD::from               , TD::to               >::type>
                && KT::type<TD::to const         > == KT::type<TT::copy_cv  <TD::from const         , TD::to               >::type>
                && KT::type<TD::to       volatile> == KT::type<TT::copy_cv  <TD::from       volatile, TD::to               >::type>
                && KT::type<TD::to const volatile> == KT::type<TT::copy_cv  <TD::from const volatile, TD::to               >::type>

                && KT::type<TD::to               > == KT::type<TT::copy_cv_t<TD::from               , TD::to               >>
                && KT::type<TD::to const         > == KT::type<TT::copy_cv_t<TD::from const         , TD::to               >>
                && KT::type<TD::to       volatile> == KT::type<TT::copy_cv_t<TD::from       volatile, TD::to               >>
                && KT::type<TD::to const volatile> == KT::type<TT::copy_cv_t<TD::from const volatile, TD::to               >>

                && KT::type<TD::to const         > == KT::type<TT::copy_cv  <TD::from               , TD::to const         >::type>
                && KT::type<TD::to const         > == KT::type<TT::copy_cv  <TD::from const         , TD::to const         >::type>
                && KT::type<TD::to const volatile> == KT::type<TT::copy_cv  <TD::from       volatile, TD::to const         >::type>
                && KT::type<TD::to const volatile> == KT::type<TT::copy_cv  <TD::from const volatile, TD::to const         >::type>

                && KT::type<TD::to const         > == KT::type<TT::copy_cv_t<TD::from               , TD::to const         >>
                && KT::type<TD::to const         > == KT::type<TT::copy_cv_t<TD::from const         , TD::to const         >>
                && KT::type<TD::to const volatile> == KT::type<TT::copy_cv_t<TD::from       volatile, TD::to const         >>
                && KT::type<TD::to const volatile> == KT::type<TT::copy_cv_t<TD::from const volatile, TD::to const         >>

                && KT::type<TD::to       volatile> == KT::type<TT::copy_cv  <TD::from               , TD::to       volatile>::type>
                && KT::type<TD::to const volatile> == KT::type<TT::copy_cv  <TD::from const         , TD::to       volatile>::type>
                && KT::type<TD::to       volatile> == KT::type<TT::copy_cv  <TD::from       volatile, TD::to       volatile>::type>
                && KT::type<TD::to const volatile> == KT::type<TT::copy_cv  <TD::from const volatile, TD::to       volatile>::type>

                && KT::type<TD::to       volatile> == KT::type<TT::copy_cv_t<TD::from               , TD::to       volatile>>
                && KT::type<TD::to const volatile> == KT::type<TT::copy_cv_t<TD::from const         , TD::to       volatile>>
                && KT::type<TD::to       volatile> == KT::type<TT::copy_cv_t<TD::from       volatile, TD::to       volatile>>
                && KT::type<TD::to const volatile> == KT::type<TT::copy_cv_t<TD::from const volatile, TD::to       volatile>>

                && KT::type<TD::to const volatile> == KT::type<TT::copy_cv  <TD::from               , TD::to const volatile>::type>
                && KT::type<TD::to const volatile> == KT::type<TT::copy_cv  <TD::from const         , TD::to const volatile>::type>
                && KT::type<TD::to const volatile> == KT::type<TT::copy_cv  <TD::from       volatile, TD::to const volatile>::type>
                && KT::type<TD::to const volatile> == KT::type<TT::copy_cv  <TD::from const volatile, TD::to const volatile>::type>

                && KT::type<TD::to const volatile> == KT::type<TT::copy_cv_t<TD::from               , TD::to const volatile>>
                && KT::type<TD::to const volatile> == KT::type<TT::copy_cv_t<TD::from const         , TD::to const volatile>>
                && KT::type<TD::to const volatile> == KT::type<TT::copy_cv_t<TD::from       volatile, TD::to const volatile>>
                && KT::type<TD::to const volatile> == KT::type<TT::copy_cv_t<TD::from const volatile, TD::to const volatile>>
                ;
        }),
};

static KT::add_tests suite("nstd/type_traits/copy_cv", ::tests);