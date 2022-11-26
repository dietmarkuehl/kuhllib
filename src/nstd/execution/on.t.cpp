// nstd/execution/on.t.cpp                                            -*-C++-*-
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

#include "nstd/execution/on.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/execution/schedule.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/operation_state.hpp"
#include "nstd/type_traits/conditional.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"

namespace test_declaration {}
namespace EX = ::nstd::execution;
namespace TT = ::nstd::type_traits;
namespace UT = ::nstd::utility;
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
        struct state {
            friend auto tag_invoke(EX::start_t, state&) noexcept -> void {
            }
        };
        struct scheduler;
        template <bool B>
        struct sender {
            static constexpr bool value = B;
            using completion_signatures = EX::completion_signatures<EX::set_value_t()>;
            friend auto tag_invoke(EX::connect_t, sender, auto) {
                return state{};
            }
        };
        struct scheduler {
            auto operator== (scheduler const&) const -> bool = default;
            friend auto tag_invoke(EX::schedule_t, scheduler) noexcept {
                return TD::sender<true>{};
            }
        };
        template <bool B>
        auto tag_invoke(EX::get_completion_scheduler_t<EX::set_value_t>, TD::sender<B>) {
            return TD::scheduler{};
        }

        template <EX::sender S>
        struct on_sender {
            using completion_signatures = TT::conditional_t<S::value, typename S::completion_signatures, int>;

            struct state {
                friend auto tag_invoke(EX::start_t, state&) noexcept -> void {}
            };

            S sender;
            friend auto tag_invoke(EX::connect_t, on_sender, auto) {
                return state{};
            }
        };
        auto tag_invoke(EX::on_t, TD::scheduler , auto s) {
            return TD::on_sender(UT::move(s));
        }
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("TD::state is an operation_state", []{
            return EX::operation_state<TD::state>;
        }),
    KT::expect_success("TD::sender<bool> is a sender", []{
            return EX::sender<TD::sender<true>>
                && EX::sender<TD::sender<false>>;
        }),
    KT::expect_success("TD::scheduler is a scheduler", []{
            return EX::scheduler<TD::scheduler>;
        }),
    KT::expect_success("TD::on_sender<TD::sender<true> is a sender", []{
            return EX::sender<TD::on_sender<TD::sender<true>>>
                && TD::sender<true>::value
                ;
        }),
    KT::expect_success("TD::on_sender<TD::sender<false> is not a sender", []{
            return not EX::sender<TD::on_sender<TD::sender<false>>>
                && not TD::sender<false>::value
                ;
        }),
    KT::expect_success("EX::on is of type EX::on_t", []{
            return KT::type<EX::on_t const> == KT::type<decltype(EX::on)>;
        }),
    KT::expect_success("customized on is used if a sender is returned", []{
            auto s = EX::on(TD::scheduler{}, TD::sender<true>{});

            return EX::sender<decltype(s)>
                && KT::type<decltype(s)> == KT::type<TD::on_sender<TD::sender<true>>>
                ;
        }),
    KT::expect_success("customized on is not used if no sender is returned", []{
            auto s = EX::on(TD::scheduler{}, TD::sender<false>{});

            return EX::sender<decltype(s)>
                && not std::same_as<decltype(s), TD::on_sender<TD::sender<false>>>
                ;
        }),
};

static KT::add_tests suite("nstd/execution/on", ::tests);
