// nstd/execution/scheduler.t.cpp                                     -*-C++-*-
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

#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/schedule.hpp"
#include "nstd/execution/sender_base.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace EX = ::nstd::execution;
namespace TD = ::test_declarations;
namespace UT = ::nstd::utility;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    namespace
    {
        struct scheduler;
	template <typename Scheduler>
        struct sender {
            using completion_signatures = EX::completion_signatures<>;
        };
	template <typename Scheduler>
        auto tag_invoke(EX::get_completion_scheduler_t<EX::set_value_t> const&, sender<Scheduler> const&) -> Scheduler;

        struct scheduler {
            scheduler(scheduler const&) = default;
            auto operator== (scheduler const&) const -> bool { return true; }
            friend auto tag_invoke(::nstd::execution::schedule_t, scheduler) -> sender<scheduler> { return sender<scheduler>{}; } 
        };

        struct non_copyable_scheduler {
            non_copyable_scheduler() = default;
            non_copyable_scheduler(non_copyable_scheduler&&) = default;
            non_copyable_scheduler(non_copyable_scheduler const&) = delete;
            auto operator== (non_copyable_scheduler const&) const -> bool { return true; }
            friend auto tag_invoke(::nstd::execution::schedule_t, non_copyable_scheduler) -> sender<non_copyable_scheduler> { return sender<non_copyable_scheduler>{}; } 
        };

        struct non_comparable_scheduler {
            non_comparable_scheduler() = default;
            non_comparable_scheduler(non_comparable_scheduler const&) = default;
            friend auto tag_invoke(::nstd::execution::schedule_t, non_comparable_scheduler) -> sender<non_comparable_scheduler> { return sender<non_comparable_scheduler>{}; } 
        };

        struct non_scheduling_scheduler {
            non_scheduling_scheduler(non_scheduling_scheduler const&) = default;
            auto operator== (non_scheduling_scheduler const&) const -> bool { return true; }
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("scheduler gets classified correctly", []{
        return EX::scheduler<TD::scheduler>
            ;
    }),
    KT::expect_success("non-copyable scheduler isn't a scheduler", [](KT::context& ){
        tag_invoke(EX::schedule, TD::non_copyable_scheduler());
        return !EX::scheduler<TD::non_copyable_scheduler>
            ;
    }),
    KT::expect_success("non-comparable scheduler isn't a scheduler", [](KT::context& ){
        tag_invoke(EX::schedule, TD::non_comparable_scheduler());
        return !EX::scheduler<TD::non_comparable_scheduler>
            ;
    }),
    KT::expect_success("non-scheduling scheduler isn't a scheduler", [](KT::context& ){
        return !EX::scheduler<TD::non_scheduling_scheduler>
            ;
    }),
};

static KT::add_tests suite("scheduler", ::tests);
