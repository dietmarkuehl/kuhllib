// nstd/execution/sender_adaptor_closure.t.cpp                        -*-C++-*-
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

#include "nstd/execution/sender_adaptor_closure.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/thread/sync_wait.hpp"
#include "kuhl/test.hpp"

namespace test_declaration {}
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;
namespace EX = ::nstd::execution;
namespace TT = ::nstd::this_thread;
namespace UT = ::nstd::utility;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
        constexpr struct sender
            : ::nstd::execution::sender_adaptor_closure<sender> {
            using ::nstd::execution::sender_adaptor_closure<sender>::operator();

            template <::nstd::execution::sender Sender>
            auto operator()(Sender&& sender) const {
                return EX::then(::nstd::utility::forward<Sender>(sender), [](int v){ return 2 * v; });
            };
            template <::nstd::execution::sender Sender, typename... T>
            auto operator()(Sender&& sender, T&&... x) const {
                return EX::then(sender, [=](int v){ return ((v * x) + ... + 0); });
            };
        } cpo;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("basic operation", []{
            auto sender = TD::cpo(EX::just(17));

            auto value = TT::sync_wait(::nstd::utility::move(sender));
            return value
                && ::std::get<0>(*value) == 34
                ;
        }),
    KT::expect_success("one argument", []{
            auto sender = TD::cpo(EX::just(2), 3);
            auto value = TT::sync_wait(::nstd::utility::move(sender));
            return value
                && ::std::get<0>(*value) == 6
                ;
        }),
    KT::expect_success("multiple arguments", []{
            auto sender = TD::cpo(EX::just(2), 1, 2, 3);
            auto value = TT::sync_wait(::nstd::utility::move(sender));
            return value
                && ::std::get<0>(*value) == 12
                ;
        }),
    KT::expect_success("basic pipe", []{
            auto sender = EX::just(17) | TD::cpo;
            auto value = TT::sync_wait(::nstd::utility::move(sender));
            return value
                && ::std::get<0>(*value) == 34
                ;
        }),
    KT::expect_success("pipe with one argument", []{
            auto sender = EX::just(17) | TD::cpo(3);
            auto value = TT::sync_wait(::nstd::utility::move(sender));
            return value
                && ::std::get<0>(*value) == 51
                ;
        }),
    KT::expect_success("pipe with multiple arguments", []{
            auto sender = EX::just(17) | TD::cpo(1, 2, 3);
            auto value = TT::sync_wait(::nstd::utility::move(sender));
            return value
                && ::std::get<0>(*value) == 102
                ;
        }),
};

static KT::add_tests suite("nstd/execution/sender_adaptor_closure", ::tests);
