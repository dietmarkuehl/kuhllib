// nstd/execution/sender_of.t.cpp                                     -*-C++-*-
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

#include "nstd/execution/sender_of.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/set_value.hpp"
#include "kuhl/test.hpp"

namespace EX = ::nstd::execution;
namespace HN = ::nstd::hidden_names;
namespace test_declaration {}
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
        class env {};

        class non_sender {
        public:
            using completion_signatures
                = EX::completion_signatures<
                    EX::set_value_t()
                >;
            non_sender(non_sender&) = default;
        };

        template <typename... T>
        class sender {
        public:
            using completion_signatures
                = EX::completion_signatures<
                    EX::set_value_t(T...)
                >;
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("non-sender", []{
            return not EX::sender_of<TD::non_sender>
                ;
        }),
    KT::expect_success("sender", []{
            return EX::sender_of<TD::sender<>>
                && EX::sender_of<TD::sender<>, TD::env>
                && not EX::sender_of<TD::sender<int>, TD::env>
                && not EX::sender_of<TD::sender<>, TD::env, int>
                && EX::sender_of<TD::sender<int>, TD::env, int>
                ;
        }),
};

static KT::add_tests suite("nstd/execution/sender_of", ::tests);
