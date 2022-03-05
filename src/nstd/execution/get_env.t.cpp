// nstd/execution/get_env.t.cpp                                       -*-C++-*-
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

#include "nstd/execution/get_env.hpp"
#include "nstd/execution/no_env.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include "nstd/type_traits/is_same.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include <concepts>
#include <functional>
#include "kuhl/test.hpp"

namespace test_declaration {}
namespace EX = ::nstd::execution;
namespace TT = ::nstd::type_traits;
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
        struct env {
            int value;
        };

        struct context {
            int value;
            friend auto tag_invoke(EX::get_env_t, context self) -> env {
                return env{self.value};
            }
        };

        template <typename RC>
        struct no_env_context {
            nstd::hidden_names::exec_envs::no_env rc;
            template <typename T>
                requires TT::is_same_v<no_env_context, TT::remove_cvref_t<T>>
            friend auto tag_invoke(EX::get_env_t, T&& self) -> RC {
                return self.rc;
            }
        };

        struct non_context {};
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("context", []{
            return KT::type<TD::env> == KT::type<decltype(::nstd::tag_invoke(EX::get_env, TD::context{42}))>
                && ::nstd::tag_invocable<EX::get_env_t, TD::context>
                && ::nstd::tag_invoke(EX::get_env, TD::context{42}).value == 42
                ;
        }),
    KT::expect_success("no_env_context", []{
            using no_env = nstd::hidden_names::exec_envs::no_env;
            TD::no_env_context<no_env const&> const c1{};
            TD::no_env_context<no_env&>             c2{};
            return KT::type<no_env> == KT::type<decltype(::nstd::tag_invoke(EX::get_env, TD::no_env_context<no_env>{}))>
                && ::nstd::tag_invocable<EX::get_env_t, TD::no_env_context<no_env>>
                && KT::type<no_env const&> == KT::type<decltype(::nstd::tag_invoke(EX::get_env, c1))>
                && ::nstd::tag_invocable<EX::get_env_t, TD::no_env_context<no_env const&> const&>
                && KT::type<no_env&> == KT::type<decltype(::nstd::tag_invoke(EX::get_env, c2))>
                && ::nstd::tag_invocable<EX::get_env_t, TD::no_env_context<no_env&>&>
                ;
        }),
    KT::expect_success("basic", []{
            return KT::type<TD::env> == KT::type<decltype(EX::get_env(TD::context{42}))>
                && ::std::invocable<EX::get_env_t, TD::context>
                && EX::get_env(TD::context{42}).value == 42
                ;
        }),
    KT::expect_success("result isn't no_env", []{
            using no_env = nstd::hidden_names::exec_envs::no_env;
            return not ::std::invocable<EX::get_env_t, TD::no_env_context<no_env>>
                && not ::std::invocable<EX::get_env_t, TD::no_env_context<no_env const&> const&>
                && not ::std::invocable<EX::get_env_t, TD::no_env_context<no_env&>&>
                ;
        }),
    KT::expect_success("non-context", []{
            return not ::std::invocable<EX::get_env_t, TD::non_context>
                ;
        }),
};

static KT::add_tests suite("nstd/execution/get_env", ::tests);
