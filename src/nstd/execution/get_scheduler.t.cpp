// nstd/execution/get_scheduler.t.cpp                                 -*-C++-*-
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

#include "nstd/execution/get_scheduler.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/scheduler.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = test_declarations;
namespace EX = ::nstd::execution;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    template <typename Receiver>
    concept has_get_scheduler
        = requires(Receiver const& r) { EX::get_scheduler(r); }
        ;

    struct non_scheduler {};
    struct scheduler {
        bool operator== (scheduler const&) const = default;
        friend auto tag_invoke(EX::schedule_t, scheduler&&) {}
    };

    template <typename Scheduler, bool Noexcept, bool IsReceiver = true>
    struct receiver {
        bool* const value;
        friend auto tag_invoke(EX::set_done_t, receiver&&) noexcept(IsReceiver) {}
        friend auto tag_invoke(EX::set_error_t, receiver&&, std::exception_ptr) noexcept {}
        friend auto tag_invoke(EX::get_scheduler_t, receiver const& r) noexcept(Noexcept) {
            *r.value = true;
            return Scheduler();
        }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("get_scheduler breathing", []{
        return KT::assert_type_exists<EX::get_scheduler_t>
            && KT::type<EX::get_scheduler_t const> == KT::type<decltype(EX::get_scheduler)>
            ;
    }),
    KT::expect_success("test declarations behave as expected", []{
           return  EX::receiver<TD::receiver<TD::non_scheduler, true>>
                && EX::receiver<TD::receiver<TD::non_scheduler, false>>
                && EX::receiver<TD::receiver<TD::scheduler, true>>
                && EX::receiver<TD::receiver<TD::scheduler, false>>
                && !EX::receiver<TD::receiver<TD::scheduler, true, false>>
                && !EX::scheduler<TD::non_scheduler>
                && EX::scheduler<TD::scheduler>
                ;
        }),
    KT::expect_success("receiver returning a scheduler has get_scheduler", []{
            bool                              value(false);
            TD::receiver<TD::scheduler, true> r{&value};
            EX::get_scheduler(r);
            return TD::has_get_scheduler<TD::receiver<TD::scheduler, true>>
                && KT::type<decltype(EX::get_scheduler(r))> == KT::type<TD::scheduler>
                && value
                ;
        }),
    KT::expect_success("non-receiver doesn't have get_scheduler", []{
            return !TD::has_get_scheduler<TD::receiver<TD::scheduler, true, false>>
                ;
        }),
    KT::expect_success("throwing get_scheduler isn't allowed ", []{
            return !TD::has_get_scheduler<TD::receiver<TD::scheduler, false>>
                ;
        }),
    KT::expect_success("get_scheduler returning non-scheduler isn't allowed ", []{
            return !TD::has_get_scheduler<TD::receiver<TD::non_scheduler, true>>
                ;
        }),
};

static KT::add_tests suite("get_scheduler", ::tests);
