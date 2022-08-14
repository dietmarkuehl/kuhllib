// nstd/hidden_names/unique_completion_signatures.t.cpp               -*-C++-*-
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

#include "nstd/hidden_names/unique_completion_signatures.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/set_value.hpp"
#include "kuhl/test.hpp"

namespace test_declaration {}
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;
namespace EX = ::nstd::execution;
namespace HN = ::nstd::hidden_names;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
        struct env {};
        template <int I> struct type;
        template <int I> using signature = EX::set_value_t(TD::type<I>);
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("already unique type doesn't change", []{
            using list = EX::completion_signatures<TD::signature<0>, TD::signature<1>, TD::signature<2>>;
            return KT::type<list> == KT::type<typename HN::unique_completion_signatures<EX::completion_signatures<>, list>::type>
                ;
        }),
    KT::expect_success("two identical unique type doesn't change", []{
            using list = EX::completion_signatures<TD::signature<0>, TD::signature<1>, TD::signature<2>>;
            return KT::type<list> == KT::type<typename HN::unique_completion_signatures<list, list>::type>
                ;
        }),
    KT::expect_success("two overlapping lists get unique values", []{
            using list1 = EX::completion_signatures<TD::signature<0>, TD::signature<1>, TD::signature<2>>;
            using list2 = EX::completion_signatures<TD::signature<2>, TD::signature<3>, TD::signature<3>>;
            using expect = EX::completion_signatures<TD::signature<0>, TD::signature<1>, TD::signature<2>, TD::signature<3>>;
            return KT::type<expect> == KT::type<typename HN::unique_completion_signatures<list1, list2>::type>
                ;
        }),
    KT::expect_success("placeholder", []{
            return true;
        }),
};

static KT::add_tests suite("nstd/hidden_names/unique_completion_signatures", ::tests);
