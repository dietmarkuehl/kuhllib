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
#include "nstd/execution/get_stop_token.hpp"
#include "nstd/execution/get_env.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/receiver_of.hpp"
#include "nstd/execution/schedule.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/sender_of.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/stop_token/in_place_stop_token.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"
#include <exception>
#include <type_traits>

namespace EX = ::nstd::execution;
namespace UT = ::nstd::utility;
namespace ST = ::nstd::stop_token_ns;
namespace TT = ::nstd::type_traits;
namespace test_declaration {}
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
        enum class result { none, value, stopped };
        struct env {};
        struct arg {
            arg() = default;
            arg(arg&&) = delete;
            TD::result result = TD::result::none;
        };

        template <bool Noexcept>
        struct receiver {
            TD::arg* d_arg;
            receiver(TD::arg& a) noexcept: d_arg(&a) {}
            receiver(receiver&&) noexcept(Noexcept) = default;
            friend auto tag_invoke(EX::set_value_t, receiver&& self) noexcept -> void {
                self.d_arg->result = TD::result::value;
            }
            friend auto tag_invoke(EX::set_stopped_t, receiver&& self) noexcept -> void {
                self.d_arg->result = TD::result::stopped;
            }
            friend auto tag_invoke(EX::get_env_t, receiver const&) noexcept -> TD::env { return {}; }
        };

        struct stop_receiver {
            ST::in_place_stop_source* d_source;
            TD::arg*                  d_arg;
            stop_receiver(ST::in_place_stop_source& source, TD::arg& a) noexcept: d_source(&source), d_arg(&a) {}
            stop_receiver(stop_receiver&&) noexcept = default;
            friend auto tag_invoke(EX::set_value_t, stop_receiver&& self) noexcept -> void {
                self.d_arg->result = TD::result::value;
            }
            friend auto tag_invoke(EX::set_stopped_t, stop_receiver&& self) noexcept -> void {
                self.d_arg->result = TD::result::stopped;
            }
            friend auto tag_invoke(EX::get_env_t, stop_receiver const&) noexcept -> TD::env { return {}; }
            friend auto tag_invoke(EX::get_stop_token_t, stop_receiver const& self) noexcept -> ST::in_place_stop_token {
                return self.d_source->token();
            }
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
                && noexcept(TD::receiver(TD::receiver<true>(TT::declval<TD::arg&>())))
                ;
        }),
    KT::expect_success("TD::stop_receiver", []{
            using completion_signatures = EX::completion_signatures<EX::set_value_t(), EX::set_stopped_t()>;
            return EX::receiver<TD::stop_receiver>
                && EX::receiver<TD::stop_receiver>
                && EX::receiver_of<TD::stop_receiver, completion_signatures>
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
                    TD::arg      arg;
                    EX::run_loop loop;
                    auto state = EX::connect(EX::schedule(loop.get_scheduler()),
                                             TD::receiver<true>(arg));
                    EX::start(state);
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
            TD::arg      arg;

            return EX::sender_of<sender_type>
                && EX::sender<sender_type>
                && noexcept(EX::get_completion_scheduler<EX::set_value_t>(sender))
                && KT::type<scheduler_type> == KT::type<decltype(EX::get_completion_scheduler<EX::set_value_t>(sender))>
                && loop.get_scheduler() == EX::get_completion_scheduler<EX::set_value_t>(sender)
                && noexcept(EX::connect(UT::move(sender), TD::receiver<true>(arg)))
                && not noexcept(EX::connect(UT::move(sender), TD::receiver<false>(arg)))
                ;
        }),
    KT::expect_success("run_loop operation state (running)", []{
            EX::run_loop loop;
            TD::arg arg;
            auto sender = EX::schedule(loop.get_scheduler());
            auto state = EX::connect(UT::move(sender), TD::receiver<true>(arg));
            EX::start(state);
            loop.finish();
            loop.run();

            return arg.result == TD::result::value
                ;
        }),
    KT::expect_success("run_loop operation state (stopped)", []{
            EX::run_loop loop;
            ST::in_place_stop_source source;
            source.stop();
            TD::arg arg;
            auto sender = EX::schedule(loop.get_scheduler());
            auto state = EX::connect(UT::move(sender), TD::stop_receiver(source, arg));
            EX::start(state);
            loop.finish();
            loop.run();

            return arg.result == TD::result::stopped
                ;
        }),
};

static KT::add_tests suite("nstd/execution/run_loop", ::tests);
