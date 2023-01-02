// nstd/hidden_names/merge_completion_signatures.t.cpp                -*-C++-*-
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

#include "nstd/hidden_names/merge_completion_signatures.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/hidden_names/print_completion_signatures.hpp"
#include "kuhl/test.hpp"

#include <system_error>
#include <concepts>

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
        template <int I> using signature = EX::set_value_t(type<I>);
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("merge of one list remains unchanged", []{
            using list = EX::completion_signatures<TD::signature<1>, TD::signature<2>, TD::signature<3>>;
            return KT::type<list> == KT::type<HN::merge_completion_signatures_t<TD::env, list>>
                ;
        }),
    KT::expect_success("merge of two identical lists remains unchanged", []{
            using list = EX::completion_signatures<TD::signature<1>, TD::signature<2>, TD::signature<3>>;
            return KT::type<list> == KT::type<HN::merge_completion_signatures_t<TD::env, list, list>>
                ;
        }),
    KT::expect_success("merge lists ", []{
            using list = ::nstd::hidden_names::merge_completion_signatures_t<
                ::nstd::hidden_names::exec_envs::no_env,
                EX::completion_signatures<
                    EX::set_value_t(),
                    EX::set_error_t(::std::exception_ptr),
                    EX::set_stopped_t()
                >,
                EX::completion_signatures<
                    EX::set_error_t(::std::exception_ptr),
                    EX::set_error_t(::std::error_code)
                >
            >;
            return ::std::same_as<
                    list,
                    EX::completion_signatures<
                        EX::set_value_t(),
                        EX::set_error_t(::std::exception_ptr),
                        EX::set_stopped_t(),
                        EX::set_error_t(::std::error_code)
                    >
                >;
        }),

};

static KT::add_tests suite("nstd/hidden_names/merge_completion_signatures", ::tests);
