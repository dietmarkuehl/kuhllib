// nstd/execution/get_completion_scheduler.t.cpp                      -*-C++-*-
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

#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/schedule.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/sender_base.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include <type_traits> //-dk:TODO "nstd/type_traits/conditional.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = test_declarations;
namespace EX = ::nstd::execution;
namespace TT = ::std; //-dk:TODO ::nstd::type_traits;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct scheduler_sender
            : EX::sender_base
        {
            using completion_signatures = EX::completion_signatures<>;
        };

        struct scheduler
        {
            int value;
            explicit scheduler(int value): value(value) {}
            bool operator== (scheduler const&) const = default;
            friend auto tag_invoke(EX::schedule_t, scheduler) noexcept -> scheduler_sender {
                return {};
            }
        };

        struct empty {};
        struct custom_t {};

        template <bool Sender, bool Noexcept, bool Scheduler, typename CPO>
        struct sender
            : TT::conditional_t<Sender, EX::sender_base, empty>
        {
            using completion_signatures = TT::conditional_t<Sender, EX::completion_signatures<>, empty>;
            TT::conditional_t<Scheduler, TD::scheduler, int> scheduler;
            explicit sender(int id): scheduler(id) {}

            friend auto tag_invoke(EX::get_completion_scheduler_t<CPO>, sender&) noexcept(Noexcept) -> TD::scheduler = delete;
            friend auto tag_invoke(EX::get_completion_scheduler_t<CPO>, sender&&) noexcept(Noexcept) -> TD::scheduler = delete;
            friend auto tag_invoke(EX::get_completion_scheduler_t<CPO>, sender const& s) noexcept(Noexcept)
                -> TT::conditional_t<Scheduler, TD::scheduler, int>
            {
                return s.scheduler;
            }
        };

        template <bool HasGetCompletionScheduler, typename Sender, typename CPO>
        auto check_get_completion_scheduler() {
            if constexpr (requires(){ EX::get_completion_scheduler<CPO>; }) {
                if constexpr (requires(Sender&& s){ EX::get_completion_scheduler<CPO>(s); }) {
                    Sender        sender(17);
                    return true == HasGetCompletionScheduler
                        && EX::get_completion_scheduler<CPO>(sender) == TD::scheduler{17}
                        ;
                }
                return false == HasGetCompletionScheduler;
            }
            return false == HasGetCompletionScheduler;
        }
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("scheduler_sender is a sender", []{ return EX::sender<TD::scheduler_sender>; }),
    KT::expect_success("scheduler is a scheduler", []{ return EX::scheduler<TD::scheduler>; }),
    KT::expect_success("sender<true, true, true, set_value_t> is a sender", []{ return EX::sender<TD::sender<true, true, true, EX::set_value_t>>; }),
    KT::expect_success("sender<false, true, true, set_value_t> is not a sender", []{ return not EX::sender<TD::sender<false, true, true, EX::set_value_t>>; }),
    KT::expect_success("sender<true, false, true, set_value_t> is a sender", []{ return EX::sender<TD::sender<true, false, true, EX::set_value_t>>; }),
    KT::expect_success("sender<false, false, true, set_value_t> is not a sender", []{ return not EX::sender<TD::sender<false, false, true, EX::set_value_t>>; }),
    KT::expect_success("breathing", []{
            return KT::type<EX::get_completion_scheduler_t<EX::set_value_t> const>
                   == KT::type<decltype(EX::get_completion_scheduler<EX::set_value_t>)>
                && KT::type<EX::get_completion_scheduler_t<EX::set_error_t> const>
                   == KT::type<decltype(EX::get_completion_scheduler<EX::set_error_t>)>
                && KT::type<EX::get_completion_scheduler_t<EX::set_stopped_t> const>
                   == KT::type<decltype(EX::get_completion_scheduler<EX::set_stopped_t>)>
                ;
        }),
    // working cases
    KT::expect_success("sender<true, true, true, set_value_t> has get_completion_scheduler<set_value_t>", []{
            return TD::check_get_completion_scheduler<true, TD::sender<true, true, true, EX::set_value_t>, EX::set_value_t>();
        }),
    KT::expect_success("sender<true, true, true, set_error_t> has get_completion_scheduler<set_error_t>", []{
            return TD::check_get_completion_scheduler<true, TD::sender<true, true, true, EX::set_error_t>, EX::set_error_t>();
        }),
    KT::expect_success("sender<true, true, true, set_stopped_t> has get_completion_scheduler<set_stopped_t>", []{
            return TD::check_get_completion_scheduler<true, TD::sender<true, true,true,  EX::set_stopped_t>, EX::set_stopped_t>();
        }),
    // wrong completion channel
    KT::expect_success("sender<true, true, true, set_value_t> does not have get_completion_scheduler<set_value_t const>", []{
            return TD::check_get_completion_scheduler<false, TD::sender<true, true, true, EX::set_value_t>, EX::set_value_t const>();
        }),
    KT::expect_success("sender<true, true, true, set_value_t> does not have get_completion_scheduler<set_value_t&>", []{
            return TD::check_get_completion_scheduler<false, TD::sender<true, true, true, EX::set_value_t>, EX::set_value_t&>();
        }),
    KT::expect_success("sender<true, true, true, custom_t> does not have get_completion_scheduler<custom_t>", []{
            return TD::check_get_completion_scheduler<false, TD::sender<true, true, true, TD::custom_t>, TD::custom_t>();
        }),

    // sender isn't a sender
    KT::expect_success("sender<false, true, true, set_value_t> does not have get_completion_scheduler<set_value_t>", []{
            return TD::check_get_completion_scheduler<false, TD::sender<false, true, true, EX::set_value_t>, EX::set_value_t>();
        }),
    KT::expect_success("sender<false, true, true, set_error_t> does not have get_completion_scheduler<set_error_t>", []{
            return TD::check_get_completion_scheduler<false, TD::sender<false, true, true, EX::set_error_t>, EX::set_error_t>();
        }),
    KT::expect_success("sender<false, true, true, set_stopped_t> does not have get_completion_scheduler<set_stopped_t>", []{
            return TD::check_get_completion_scheduler<false, TD::sender<false, true, true, EX::set_stopped_t>, EX::set_stopped_t>();
        }),

    // not noexcept
    KT::expect_success("sender<true, false, true, set_value_t> does not have get_completion_scheduler<set_value_t>", []{
            return TD::check_get_completion_scheduler<false, TD::sender<true, false, true, EX::set_value_t>, EX::set_value_t>();
        }),
    KT::expect_success("sender<true, false, true, set_error_t> does not have get_completion_scheduler<set_error_t>", []{
            return TD::check_get_completion_scheduler<false, TD::sender<true, false, true, EX::set_error_t>, EX::set_error_t>();
        }),
    KT::expect_success("sender<true, false, true, set_stopped_t> does not have get_completion_scheduler<set_stopped_t>", []{
            return TD::check_get_completion_scheduler<false, TD::sender<true, false, true, EX::set_stopped_t>, EX::set_stopped_t>();
        }),

    // result isn't a scheduler
    KT::expect_success("sender<true, true, false, set_value_t> does not have get_completion_scheduler<set_value_t>", []{
            return TD::check_get_completion_scheduler<false, TD::sender<true, true, false, EX::set_value_t>, EX::set_value_t>();
        }),
    KT::expect_success("sender<true, true, false, set_error_t> does not have get_completion_scheduler<set_error_t>", []{
            return TD::check_get_completion_scheduler<false, TD::sender<true, true, false, EX::set_error_t>, EX::set_error_t>();
        }),
    KT::expect_success("sender<true, true, false, set_stopped_t> does not have get_completion_scheduler<set_stopped_t>", []{
            return TD::check_get_completion_scheduler<false, TD::sender<true, true, false,  EX::set_stopped_t>, EX::set_stopped_t>();
        }),
};

static KT::add_tests suite("get_completion_scheduler", ::tests);
