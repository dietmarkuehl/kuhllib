// nstd/execution/error_types_of_t.t.cpp                              -*-C++-*-
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

#include "nstd/execution/error_types_of_t.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/hidden_names/variant_or_empty.hpp"
#include "kuhl/test.hpp"

namespace test_declaration {}
namespace TD = ::test_declaration;
namespace EX = ::nstd::execution;
namespace HN = ::nstd::hidden_names;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
        struct env {};
        struct error {};
        struct type {};

        template <typename... Signatures>
        struct sender {
            using completion_signatures = EX::completion_signatures<Signatures...>;
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("TD::sender is a sender", []{
            return EX::sender<TD::sender<>>
                && EX::sender<TD::sender<EX::set_value_t(TD::type), EX::set_stopped_t(), EX::set_error_t(TD::error)>>
                ;
        }),
    KT::expect_success("empty", []{
            return KT::type<HN::variant_or_empty<>>
                    == KT::type<EX::error_types_of_t<TD::sender<>, TD::env>>;
        }),
    KT::expect_success("one error", []{
            return KT::type<HN::variant_or_empty<TD::error>>
                    == KT::type<EX::error_types_of_t<TD::sender<EX::set_value_t(TD::type), EX::set_stopped_t(), EX::set_error_t(TD::error)>, TD::env>>;
        }),
};

static KT::add_tests suite("nstd/execution/error_types_of_t", ::tests);
