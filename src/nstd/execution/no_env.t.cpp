// src/nstd/execution/no_env.t.cpp                                    -*-C++-*-
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

#include "nstd/concepts/invocable.hpp"
#include "nstd/execution/no_env.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include "kuhl/test.hpp"
#include <functional>

namespace KT = ::kuhl::test;
namespace test_declarations {};
namespace TD = test_declarations;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    namespace {
        struct type {};
        struct cpo_t {};

        template <typename Arg>
        auto tag_invoke(cpo_t, Arg&&) -> bool { return true; }
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("test classes", []{
            return ::nstd::concepts::invocable<decltype(::nstd::tag_invoke), TD::cpo_t, int>
                && ::nstd::concepts::invocable<decltype(::nstd::tag_invoke), TD::cpo_t, TD::type>
                ;
        }),
    KT::expect_success("no_env doesn't tag_invoke", []{
            return not ::nstd::concepts::invocable<decltype(::nstd::tag_invoke), TD::cpo_t, ::nstd::hidden_names::exec_envs::no_env>
                ;
        }),
};

static KT::add_tests suite("no_env", ::tests);