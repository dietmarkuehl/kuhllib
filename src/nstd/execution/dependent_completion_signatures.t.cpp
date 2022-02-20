// src/nstd/execution/dependent_completion_signatures.t.cpp           -*-C++-*-
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

#include "nstd/execution/dependent_completion_signatures.hpp"
#include <concepts>
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = test_declarations;
namespace EX = ::nstd::execution;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        namespace A {
            struct type {};

            void fun(EX::dependent_completion_signatures<type>) {}
        }

        namespace B {
            template <typename T>
            concept qualified_fun
                = requires(T val) { TD::A::fun(val); }
                ;

            template <typename T>
            concept unqualified_fun
                = requires(T val) { fun(val); }
                ;
        }
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("breathing", []{
            return 0u != sizeof(EX::dependent_completion_signatures<int>);
        }),
    KT::expect_success("function can be called qualified", []{
            if constexpr (TD::B::qualified_fun<EX::dependent_completion_signatures<TD::A::type>>) {
                TD::A::fun(EX::dependent_completion_signatures<TD::A::type>{});
            }
            return TD::B::qualified_fun<EX::dependent_completion_signatures<TD::A::type>>
                ;
        }),
    KT::expect_success("function can not be called unqualified", []{
            return not TD::B::unqualified_fun<EX::dependent_completion_signatures<TD::A::type>>
                ;
        }),
};

static KT::add_tests suite("dependent_completion_signatures", ::tests);
