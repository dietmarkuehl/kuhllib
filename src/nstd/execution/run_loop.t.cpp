// nstd/execution/run_loop.t.cpp                                      -*-C++-*-
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

#include "nstd/execution/run_loop.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/get_env.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/receiver_of.hpp"
#include "nstd/execution/schedule.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/sender_of.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"
#include <exception>
#include <type_traits>

namespace EX = ::nstd::execution;
namespace UT = ::nstd::utility;
namespace test_declaration {}
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
        struct env {};

        template <bool Noexcept>
        struct receiver {
            receiver() noexcept {}
            receiver(receiver&&) noexcept(Noexcept) {}
            friend auto tag_invoke(EX::set_value_t, receiver&&) noexcept -> void {}
            friend auto tag_invoke(EX::set_stopped_t, receiver&&) noexcept -> void {}
            friend auto tag_invoke(EX::get_env_t, receiver const&) noexcept -> TD::env { return {}; }
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("TD::receiver", []{
            using completion_signatures = EX::completion_signatures<EX::set_value_t(), EX::set_stopped_t()>;
            return EX::receiver<TD::receiver<true>>
                && EX::receiver<TD::receiver<false>>
                && EX::receiver_of<TD::receiver<true>, completion_signatures>
                && EX::receiver_of<TD::receiver<false>, completion_signatures>
                && noexcept(TD::receiver(TD::receiver<true>()))
                ;
        }),
    KT::expect_success("exec.run_loop.ctor", []{
            EX::run_loop loop;
            KT::use(loop);
            return noexcept(EX::run_loop())
                && not ::std::is_copy_constructible_v<EX::run_loop>
                && not ::std::is_move_constructible_v<EX::run_loop>
                && not ::std::is_copy_assignable_v<EX::run_loop>
                && not ::std::is_move_assignable_v<EX::run_loop>
                && KT::test_terminate([]{
                    // if count == 0 or state is running
                    ::std::terminate(); //-dk:TODO
                })
                ;
        }),
    KT::expect_success("exec.run_loop.members get_scheduler", []{
            using scheduler_type = decltype(EX::run_loop().get_scheduler());
            EX::run_loop l1;
            EX::run_loop l2;

            return KT::type<EX::run_loop::scheduler> == KT::type<scheduler_type>
                && EX::scheduler<scheduler_type>
                && l1.get_scheduler() == l1.get_scheduler()
                && l2.get_scheduler() == l2.get_scheduler()
                && l1.get_scheduler() != l2.get_scheduler()
                ;
        }),
    KT::expect_success("run_loop sender", []{
            using scheduler_type = decltype(EX::run_loop().get_scheduler());
            using sender_type = decltype(EX::schedule(EX::run_loop().get_scheduler()));
            EX::run_loop loop;
            sender_type  sender = EX::schedule(loop.get_scheduler());

            return EX::sender_of<sender_type>
                && EX::sender<sender_type>
                && noexcept(EX::get_completion_scheduler<EX::set_value_t>(sender))
                && KT::type<scheduler_type> == KT::type<decltype(EX::get_completion_scheduler<EX::set_value_t>(sender))>
                && loop.get_scheduler() == EX::get_completion_scheduler<EX::set_value_t>(sender)
                && noexcept(EX::connect(UT::move(sender), TD::receiver<true>()))
                && not noexcept(EX::connect(UT::move(sender), TD::receiver<false>()))
                ;
        }),
};

static KT::add_tests suite("nstd/execution/run_loop", ::tests);
