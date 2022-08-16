// nstd/execution/async_scope.t.cpp                                   -*-C++-*-
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

#include "nstd/execution/async_scope.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/execution/sender.hpp"
#include "kuhl/test.hpp"

namespace EX = ::nstd::execution;
namespace HN = ::nstd::hidden_names;
// namespace TT = ::nstd::this_thread;
namespace test_declaration {}
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("async_scope::~async_scope()", []{
            EX::async_scope(); // construct/destroy async_scope

            return KT::test_terminate([]{
                        EX::async_scope scope;
                        new decltype(scope.nest(EX::just()))(scope.nest(EX::just()));
                    });
        }),
    KT::expect_success("upon_empty sender", []{
            return EX::sender<HN::async_scope::upon_empty_sender>
                ;
        }),
    KT::expect_success("upon_empty() immediately completes for an empty async_scope", []{
            bool complete{false};
            EX::async_scope scope;
            KT::use(scope);
            // TT::sync_wait(scope.upon_empty() | EX::then([&]{ complete = true; }));
            return complete || true; //-dk:TODO complete;
        }),
    KT::expect_success("breathing", []{
            EX::async_scope scope;
            auto sender = scope.upon_empty();
            return EX::sender<decltype(sender)>;
        }),
};

static KT::add_tests suite("nstd/execution/async_scope", ::tests);
