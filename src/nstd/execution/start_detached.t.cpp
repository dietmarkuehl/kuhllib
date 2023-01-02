// nstd/execution/start_detached.t.cpp                                -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2021 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/execution/start_detached.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/operation_state.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/type_traits/is_same.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/thread/sync_wait.hpp"
#include "kuhl/test.hpp"
#include <memory>

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;
namespace EX = ::nstd::execution;
namespace UT = ::nstd::utility;
namespace NT = ::nstd::this_thread;
namespace TT = ::nstd::type_traits;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        template <EX::receiver Receiver>
        struct state
        {
            TT::remove_cvref_t<Receiver> d_receiver;
            int* result;
            int  value;
            bool* destroyed;
            ~state(){ *this->destroyed = true; }
            friend auto tag_invoke(EX::start_t, state& s) noexcept -> void {
                *s.result = s.value;
                EX::set_value(UT::move(s.d_receiver));
            }
        };

        static_assert(EX::operation_state<TD::state<EX::start_detached_t::receiver>>);

        struct scheduler {
            struct state {
                friend auto tag_invoke(EX::start_t, state&) noexcept {}
            };
            struct sender {
                using completion_signatures = EX::completion_signatures<>;

                template <typename Tag>
                friend auto tag_invoke(EX::get_completion_scheduler_t<Tag>, sender const&) noexcept
                    -> scheduler {
                    return {};
                }
                template <EX::receiver Receiver>
                friend auto tag_invoke(EX::connect_t, sender&&, Receiver&&)
                    -> state {
                    return {};
                }
            };
            auto operator== (scheduler const&) const -> bool = default;
            friend auto tag_invoke(EX::schedule_t, scheduler&&) noexcept -> sender {
                return {};
            }
        };
        static_assert(EX::operation_state<TD::scheduler::state>);
        static_assert(EX::sender<TD::scheduler::sender>);
        static_assert(EX::scheduler<TD::scheduler>);

        struct sender
        {
            using completion_signatures = EX::completion_signatures<>;

            int*  result;
            int   value;
            bool* destroyed;

            template <EX::receiver Receiver>
            friend auto tag_invoke(EX::connect_t, sender&& sndr, Receiver&& receiver) {
                return TD::state<Receiver>{ UT::forward<Receiver>(receiver), sndr.result, sndr.value, sndr.destroyed };
            }
        };
        static_assert(EX::sender<TD::sender>);

        template <typename RC>
        struct detached_sender
        {
            using completion_signatures = EX::completion_signatures<>;

            int*  result;
            int   value;
            bool* destroyed;
            bool* started_detached;

            template <EX::receiver Receiver>
            friend auto tag_invoke(EX::connect_t, detached_sender&& sndr, Receiver&& receiver) {
                return TD::state<Receiver>{ UT::forward<Receiver>(receiver), sndr.result, sndr.value, sndr.destroyed };
            }
            friend auto tag_invoke(EX::start_detached_t, detached_sender&& sndr) -> RC {
                *sndr.started_detached = true;
                NT::sync_wait(UT::move(sndr));
                if constexpr (not TT::is_same_v<RC, void>) {
                    return RC{};
                }
            }
        };
        static_assert(EX::sender<TD::detached_sender<void>>);
        static_assert(EX::sender<TD::detached_sender<int>>);

        template <typename RC>
        struct completion_sender
        {
            using completion_signatures = EX::completion_signatures<>;

            template <template <typename...> class T, template <typename...> class V>
            using value_types = V<T<>>;
            template <template <typename...> class V>
            using error_types = V<::std::exception_ptr>;
            static constexpr bool sends_done = false;

            int*  result;
            int   value;
            bool* destroyed;
            bool* started_detached;

            template <EX::receiver Receiver>
            friend auto tag_invoke(EX::connect_t, completion_sender&& sndr, Receiver&& receiver) {
                return TD::state<Receiver>{ UT::forward<Receiver>(receiver), sndr.result, sndr.value, sndr.destroyed };
            }
            friend auto tag_invoke(EX::get_completion_scheduler_t<EX::set_value_t>, completion_sender const&)
                noexcept -> TD::scheduler {
                return TD::scheduler{};
            }
            friend auto tag_invoke(EX::start_detached_t, TD::scheduler&&, completion_sender&& sndr) -> RC {
                *sndr.started_detached = true;
                NT::sync_wait(UT::move(sndr));
                if constexpr (not TT::is_same_v<RC, void>) {
                    return RC{};
                }
            }
        };
        static_assert(EX::sender<TD::completion_sender<void>>);
        static_assert(EX::sender<TD::completion_sender<int>>);

        template <typename RC>
        struct detached_completion_sender
        {
            using completion_signatures = EX::completion_signatures<>;

            template <template <typename...> class T, template <typename...> class V>
            using value_types = V<T<>>;
            template <template <typename...> class V>
            using error_types = V<::std::exception_ptr>;
            static constexpr bool sends_done = false;

            int*  result;
            int   value;
            bool* destroyed;
            bool* started_detached;

            template <EX::receiver Receiver>
            friend auto tag_invoke(EX::connect_t, detached_completion_sender&& sndr, Receiver&& receiver) {
                return TD::state<Receiver>{ UT::forward<Receiver>(receiver), sndr.result, sndr.value, sndr.destroyed };
            }
            friend auto tag_invoke(EX::get_completion_scheduler_t<EX::set_value_t>, detached_completion_sender const&)
                noexcept -> TD::scheduler {
                return TD::scheduler{};
            }
            friend auto tag_invoke(EX::start_detached_t, detached_completion_sender&& sndr) -> RC {
                NT::sync_wait(UT::move(sndr));
                if constexpr (not TT::is_same_v<RC, void>) {
                    return RC{};
                }
            }
            friend auto tag_invoke(EX::start_detached_t, TD::scheduler&&, detached_completion_sender&& sndr) -> RC {
                *sndr.started_detached = true;
                NT::sync_wait(UT::move(sndr));
                if constexpr (not TT::is_same_v<RC, void>) {
                    return RC{};
                }
            }
        };
        static_assert(EX::sender<TD::detached_completion_sender<void>>);
        static_assert(EX::sender<TD::detached_completion_sender<int>>);
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("simple sender gets run", []{
            int result{};
            int value{123};
            bool destroyed{false};
            EX::start_detached(TD::sender{&result, value, &destroyed});
            return result == value
                && destroyed
                && KT::type<decltype(EX::start_detached(TD::sender{&result, value, &destroyed}))>
                    == KT::type<void>
                ;
        }),
    KT::expect_success("sender with start_detached return non-void doesn't get run", []{
            int result{};
            int value{123};
            bool destroyed{false};
            bool started_detached{false};
            EX::start_detached(TD::detached_sender<int>{&result, value, &destroyed, &started_detached});
            return result == value
                && destroyed
                && not started_detached
                && KT::type<decltype(EX::start_detached(TD::detached_sender<int>{&result, value, &destroyed, &started_detached}))>
                    == KT::type<void>
                ;
        }),
    KT::expect_success("sender with start_detached gets run", []{
            int result{};
            int value{123};
            bool destroyed{false};
            bool started_detached{false};
            EX::start_detached(TD::detached_sender<void>{&result, value, &destroyed, &started_detached});
            return result == value
                && destroyed
                && started_detached
                && KT::type<decltype(EX::start_detached(TD::detached_sender<void>{&result, value, &destroyed, &started_detached}))>
                    == KT::type<void>
                ;
        }),
    KT::expect_success("sender with get_completion_scheduler and no-void start_detached doesn't get run", []{
            int result{};
            int value{123};
            bool destroyed{false};
            bool started_detached{false};
            EX::start_detached(TD::completion_sender<int>{&result, value, &destroyed, &started_detached});
            return result == value
                && destroyed
                && not started_detached
                && KT::type<decltype(EX::start_detached(TD::completion_sender<int>{&result, value, &destroyed, &started_detached}))>
                    == KT::type<void>
                ;
        }),
    KT::expect_success("sender with get_completion_scheduler gets run", []{
            int result{};
            int value{123};
            bool destroyed{false};
            bool started_detached{false};
            EX::start_detached(TD::completion_sender<void>{&result, value, &destroyed, &started_detached});
            return result == value
                && destroyed
                && started_detached
                && KT::type<decltype(EX::start_detached(TD::completion_sender<void>{&result, value, &destroyed, &started_detached}))>
                    == KT::type<void>
                ;
        }),
    KT::expect_success("sender with get_completion_scheduler and start_detached gets run", []{
            int result{};
            int value{123};
            bool destroyed{false};
            bool started_detached{false};
            EX::start_detached(TD::detached_completion_sender<void>{&result, value, &destroyed, &started_detached});
            return result == value
                && destroyed
                && started_detached
                && KT::type<decltype(EX::start_detached(TD::detached_completion_sender<void>{&result, value, &destroyed, &started_detached}))>
                    == KT::type<void>
                ;
        }),
};

static KT::add_tests suite("start_detached", ::tests);
