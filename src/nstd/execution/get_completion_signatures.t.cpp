// src/nstd/execution/get_completion_signatures.t.cpp                 -*-C++-*-
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

#include "nstd/execution/get_completion_signatures.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/type_traits/declval.hpp"
#include <tuple>
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = test_declarations;
namespace EX = ::nstd::execution;
namespace HN = ::nstd::hidden_names;
namespace TT = ::nstd::type_traits;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    namespace {
        struct error {};
        struct non_sender {};

        struct sender {
            friend auto tag_invoke(EX::get_completion_signatures_t, sender, auto&&)
                -> EX::completion_signatures<EX::set_stopped_t(), EX::set_error_t(TD::error)>
            {
                return {};
            }
        };

        struct typedef_sender {
            using completion_signatures = EX::completion_signatures<EX::set_stopped_t(), EX::set_value_t(int)>;
        };

        struct dependent_sender {
            template <typename E>
            friend auto tag_invoke(EX::get_completion_signatures_t, dependent_sender, E&&)
                -> EX::dependent_completion_signatures<E>
            {
                return {};
            }
        };

        struct dependent_typedef_sender {
            using completion_signatures = EX::dependent_completion_signatures<void>;
        };

        struct broken_sender {
            template <typename E>
            friend auto tag_invoke(EX::get_completion_signatures_t, broken_sender, E&&) {
                return std::tuple<bool, int>{};
            }
        };

        struct broken_typedef_sender {
            using completion_signatures = ::std::tuple<bool, int>;
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("placeholder", []{ return true; }),
    KT::expect_success("non-sender", []{
            return KT::type<HN::no_completion_signatures::type>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::non_sender>()))>
                && KT::type<HN::no_completion_signatures::type>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::non_sender const&&>()))>
                && KT::type<HN::no_completion_signatures::type>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::non_sender&>()))>
                && KT::type<HN::no_completion_signatures::type>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::non_sender const&>()))>
                ;
        }),
    KT::expect_success("sender", []{
            return KT::type<EX::completion_signatures<EX::set_stopped_t(), EX::set_error_t(TD::error)>>
                    == KT::type<decltype(::nstd::tag_invoke(EX::get_completion_signatures, TT::declval<TD::sender>(), HN::exec_envs::no_env{}))>
                && KT::type<EX::completion_signatures<EX::set_stopped_t(), EX::set_error_t(TD::error)>>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::sender>()))>
                && KT::type<EX::completion_signatures<EX::set_stopped_t(), EX::set_error_t(TD::error)>>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::sender const&&>()))>
                && KT::type<EX::completion_signatures<EX::set_stopped_t(), EX::set_error_t(TD::error)>>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::sender&>()))>
                && KT::type<EX::completion_signatures<EX::set_stopped_t(), EX::set_error_t(TD::error)>>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::sender const&>()))>
                ;
        }),
    KT::expect_success("sender with nested typedef", []{
            return KT::type<EX::completion_signatures<EX::set_stopped_t(), EX::set_value_t(int)>>
                    == KT::type<TD::typedef_sender::completion_signatures>
                && KT::type<EX::completion_signatures<EX::set_stopped_t(), EX::set_value_t(int)>>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::typedef_sender>()))>
                && KT::type<EX::completion_signatures<EX::set_stopped_t(), EX::set_value_t(int)>>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::typedef_sender const&&>()))>
                && KT::type<EX::completion_signatures<EX::set_stopped_t(), EX::set_value_t(int)>>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::typedef_sender&>()))>
                && KT::type<EX::completion_signatures<EX::set_stopped_t(), EX::set_value_t(int)>>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::typedef_sender const&>()))>
                ;
        }),
    KT::expect_success("dependent_sender", []{
            return KT::type<EX::dependent_completion_signatures<HN::exec_envs::no_env>>
                    == KT::type<decltype(::nstd::tag_invoke(EX::get_completion_signatures, TT::declval<TD::dependent_sender>(), HN::exec_envs::no_env{}))>
                    #if 1
                && KT::type<EX::dependent_completion_signatures<HN::exec_envs::no_env>>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::dependent_sender>()))>
                && KT::type<EX::dependent_completion_signatures<HN::exec_envs::no_env>>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::dependent_sender const&&>()))>
                && KT::type<EX::dependent_completion_signatures<HN::exec_envs::no_env>>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::dependent_sender&>()))>
                && KT::type<EX::dependent_completion_signatures<HN::exec_envs::no_env>>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::dependent_sender const&>()))>
                    #endif
                ;
        }),
    KT::expect_success("dependent sender with nested typedef", []{
            return KT::type<EX::dependent_completion_signatures<void>>
                    == KT::type<TD::dependent_typedef_sender::completion_signatures>
                && KT::type<EX::dependent_completion_signatures<void>>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::dependent_typedef_sender>()))>
                && KT::type<EX::dependent_completion_signatures<void>>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::dependent_typedef_sender const&&>()))>
                && KT::type<EX::dependent_completion_signatures<void>>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::dependent_typedef_sender&>()))>
                && KT::type<EX::dependent_completion_signatures<void>>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::dependent_typedef_sender const&>()))>
                ;
        }),
    KT::expect_success("broken_sender", []{
            return KT::type<::std::tuple<bool, int>>
                    == KT::type<decltype(::nstd::tag_invoke(EX::get_completion_signatures, TT::declval<TD::broken_sender>(), HN::exec_envs::no_env{}))>
                && KT::type<HN::no_completion_signatures::type>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::broken_sender>()))>
                && KT::type<HN::no_completion_signatures::type>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::broken_sender const&&>()))>
                && KT::type<HN::no_completion_signatures::type>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::broken_sender&>()))>
                && KT::type<HN::no_completion_signatures::type>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::broken_sender const&>()))>
                ;
        }),
    KT::expect_success("broken sender with nested typedef", []{
            return KT::type<::std::tuple<bool, int>>
                    == KT::type<TD::broken_typedef_sender::completion_signatures>
                && KT::type<HN::no_completion_signatures::type>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::broken_typedef_sender>()))>
                && KT::type<HN::no_completion_signatures::type>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::broken_typedef_sender const&&>()))>
                && KT::type<HN::no_completion_signatures::type>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::broken_typedef_sender&>()))>
                && KT::type<HN::no_completion_signatures::type>
                    == KT::type<decltype(EX::get_completion_signatures(TT::declval<TD::broken_typedef_sender const&>()))>
                ;
        }),
};

static KT::add_tests suite("get_completion_signatures", ::tests);