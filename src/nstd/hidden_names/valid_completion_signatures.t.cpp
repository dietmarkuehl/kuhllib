// src/nstd/hidden_names/valid_completion_signatures.t.cpp            -*-C++-*-
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

#include "nstd/hidden_names/valid_completion_signatures.hpp"
#include "nstd/execution/no_env.hpp"
#include "nstd/execution/set_value.hpp"
#include <tuple>
#include "kuhl/test.hpp"

namespace test_declarations {};
namespace TD = test_declarations;
namespace EX = ::nstd::execution;
namespace HN = ::nstd::hidden_names;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct type {};
        struct env {};
        template <typename...> struct completion_signatures {};
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("non-completion signature", []{
            return not HN::valid_completion_signatures<TD::completion_signatures<EX::set_value_t(TD::type)>, TD::env>
                && not HN::valid_completion_signatures<TD::completion_signatures<EX::set_value_t(TD::type)>, HN::exec_envs::no_env>
                && not HN::valid_completion_signatures<::std::tuple<EX::set_value_t(TD::type)>, TD::env>
                && not HN::valid_completion_signatures<::std::tuple<EX::set_value_t(TD::type)>, HN::exec_envs::no_env>
                ;
        }),
    KT::expect_success("valid completion signature", []{
            return HN::valid_completion_signatures<EX::completion_signatures<EX::set_value_t(TD::type)>, TD::env>
                && HN::valid_completion_signatures<EX::completion_signatures<EX::set_value_t(TD::type)>, HN::exec_envs::no_env>
                ;
        }),
    KT::expect_success("dependent completion signature", []{
            return not HN::valid_completion_signatures<EX::dependent_completion_signatures<TD::env>, TD::env>
                && not HN::valid_completion_signatures<EX::dependent_completion_signatures<HN::exec_envs::no_env>, TD::env>
                && HN::valid_completion_signatures<EX::dependent_completion_signatures<HN::exec_envs::no_env>, HN::exec_envs::no_env>
                ;
        }),
};

static KT::add_tests suite("valid_completion_signatures", ::tests);
