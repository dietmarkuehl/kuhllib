// nstd/hidden_names/sender_base.t.cpp                                -*-C++-*-
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

#include "nstd/hidden_names/sender_base.hpp"
#include "nstd/hidden_names/no_completion_signatures.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/get_completion_signatures.hpp"
#include "nstd/execution/no_env.hpp"
#include "kuhl/test.hpp"


namespace test_declarations {}
namespace TD = ::test_declarations;
namespace EX = ::nstd::execution;
namespace HN = ::nstd::hidden_names;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct signatures {};
        struct env {};
        struct non_sender_base {};

        struct wrong_completion_signatures {
            using completion_signatures = TD::signatures;
        };

        struct sender_base_no_completion_signatures {
            using completion_signatures = HN::no_completion_signatures::type;
        };

        struct sender_base {
            using completion_signatures = EX::completion_signatures<>;
        };

        struct sender_base_with_getter {
            friend auto tag_invoke(EX::get_completion_signatures_t, sender_base_with_getter, auto&&) {
                return EX::completion_signatures<>{};
            }
        };

        struct sender_base_with_conditional_getter {
            friend auto tag_invoke(EX::get_completion_signatures_t, sender_base_with_conditional_getter, TD::env) {
                return EX::completion_signatures<>{};
            }
            friend auto tag_invoke(EX::get_completion_signatures_t, sender_base_with_conditional_getter, HN::exec_envs::no_env) {
                return HN::no_completion_signatures::type{};
            }
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("non-sender_base", []{
            return not HN::xsender_base<TD::non_sender_base, TD::env>
                ;
        }),
    KT::expect_success("wrong completion_signatures", []{
            return not HN::xsender_base<TD::wrong_completion_signatures, TD::env>
                ;
        }),
    KT::expect_success("sender_base_no_completion_signatures", []{
            return not HN::xsender_base<TD::sender_base_no_completion_signatures, TD::env>
                ;
        }),
    KT::expect_success("sender_base", []{
            return HN::xsender_base<TD::sender_base, TD::env>
                ;
        }),
    KT::expect_success("sender_base_with_getter", []{
            return HN::xsender_base<TD::sender_base, TD::env>
                ;
        }),
    KT::expect_success("sender_base_with_conditional_getter", []{
            EX::get_completion_signatures(TD::sender_base_with_conditional_getter{});
            EX::get_completion_signatures(TD::sender_base_with_conditional_getter{}, TD::env{});
            return HN::xsender_base<TD::sender_base, TD::env>
                && HN::xsender_base<TD::sender_base, HN::exec_envs::no_env>
                ;
        }),
};

static KT::add_tests suite("hidden_names::sender_base", ::tests);
