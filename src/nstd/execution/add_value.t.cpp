// nstd/execution/add_value.t.cpp                                     -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2023 Dietmar Kuehl http://www.dietmar-kuehl.de
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

#include "nstd/execution/add_value.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/execution/operation_state.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/thread/sync_wait.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"

namespace test_declaration {}
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;
namespace EX = ::nstd::execution;
namespace TR = ::nstd::this_thread;
namespace TT = ::nstd::type_traits;
namespace UT = ::nstd::utility;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
        template <EX::receiver R>
        struct state {
            TT::remove_cvref_t<R> r;
            friend auto tag_invoke(EX::start_t, state& self) noexcept -> void {
                EX::set_value(UT::move(self.r));
            }
        };
        struct sender {
            using completion_signatures = EX::completion_signatures<EX::set_value_t()>;

             template <EX::receiver R>
            friend auto tag_invoke(EX::connect_t, sender&&, R&& r)
                -> state<R> {
                return { UT::forward<R>(r) };
            }
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("dummy", []{ return true; }),
    KT::expect_success("TD::sender is a sender", []{ return EX::sender<TD::sender>; }),
    KT::expect_success("breathing", []{
            auto sender
                = EX::add_value(TD::sender(), 17, true)
                | EX::then([](auto&&...){})
                ;
            //-dk:TODO TR::sync_wait(::nstd::utility::move(sender));
            (void)sender;
            return EX::sender<decltype(sender)>;
        }),
    KT::expect_success("breathing", []{
            bool check_values{false};
            auto sender
                = EX::add_value(EX::just('a'), 17, true)
                | EX::then([&](char c, int i, bool b){
                    check_values = c == 'a' && i == 17 && b == true;
                })
                ;
            TR::sync_wait(sender);
            return EX::sender<decltype(sender)>
                && check_values
                ;
        }),
    KT::expect_success("piped use", []{
            bool check_values{false};
            auto sender
                = EX::just('a')
                | EX::add_value(17, true)
                | EX::then([&](char c, int i, bool b){
                    check_values = c == 'a' && i == 17 && b == true;
                })
                ;
            TR::sync_wait(sender);
            return EX::sender<decltype(sender)>
                && check_values
                ;
        }),
};

static KT::add_tests suite("nstd/execution/add_value", ::tests);
