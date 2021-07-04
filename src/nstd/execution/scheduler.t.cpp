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
#include "nstd/execution/schedule.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace EX = ::nstd::execution;
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    struct sender {};

    struct scheduler {
        scheduler(scheduler const&) = default;
        bool operator== (scheduler const&) const { return true; }
        friend sender tag_invoke(::nstd::execution::schedule_t, scheduler) { return sender{}; } 
    };

    struct non_copyable_scheduler {
        non_copyable_scheduler(non_copyable_scheduler&&) = default;
        non_copyable_scheduler(non_copyable_scheduler const&) = delete;
        bool operator== (non_copyable_scheduler const&) const { return true; }
        friend sender tag_invoke(::nstd::execution::schedule_t, non_copyable_scheduler) { return sender{}; } 
    };

    struct non_comparable_scheduler {
        non_comparable_scheduler(non_comparable_scheduler const&) = default;
        friend sender tag_invoke(::nstd::execution::schedule_t, non_comparable_scheduler) { return sender{}; } 
    };

    struct non_scheduling_scheduler {
        non_scheduling_scheduler(non_scheduling_scheduler const&) = default;
        bool operator== (non_scheduling_scheduler const&) const { return true; }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("scheduler gets classified correctly", [](KT::context& ){
        return EX::scheduler<TD::scheduler>
            ;
    }),
    KT::expect_success("non-copyable scheduler isn't a scheduler", [](KT::context& ){
        return !EX::scheduler<TD::non_copyable_scheduler>
            ;
    }),
    KT::expect_success("non-comparable scheduler isn't a scheduler", [](KT::context& ){
        return !EX::scheduler<TD::non_comparable_scheduler>
            ;
    }),
    KT::expect_success("non-scheduling scheduler isn't a scheduler", [](KT::context& ){
        return !EX::scheduler<TD::non_scheduling_scheduler>
            ;
    }),
};

static KT::add_tests suite("scheduler", ::tests);
