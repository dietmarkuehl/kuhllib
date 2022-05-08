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

#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/get_scheduler.hpp"
#include "nstd/execution/no_env.hpp"
#include "nstd/execution/schedule.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_value.hpp"

#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = test_declarations;
namespace EX = ::nstd::execution;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    template <typename Env>
    concept has_get_scheduler
        = requires(Env const& r) { EX::get_scheduler(r); }
        ;

    struct scheduler;
    struct non_scheduler { int* object; };
    struct sender {
        using completion_signatures = EX::completion_signatures<>;

        friend auto tag_invoke(EX::get_completion_scheduler_t<EX::set_value_t>, sender&&)
            -> scheduler;
    };
    struct scheduler {
        int* object;
        bool operator== (scheduler const&) const = default;
        friend auto tag_invoke(EX::schedule_t, scheduler&&) -> sender { return {}; }
    };
    auto tag_invoke(EX::get_completion_scheduler_t<EX::set_value_t>, sender&&)
        -> scheduler {
        return {};
    }

    template <bool Noexcept, typename Scheduler>
    struct env {
        int* object;
        friend auto tag_invoke(EX::get_scheduler_t, env const& self) noexcept(Noexcept)
            -> Scheduler {
            return { self.object };
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
    KT::expect_success("TD::sender is a sender", []{
            return EX::sender<TD::sender>;
        }),
    KT::expect_success("TD::non_scheduler is not a scheduler", []{
            return not EX::scheduler<TD::non_scheduler>;
        }),
    KT::expect_success("TD::scheduler is a scheduler", []{
            return EX::scheduler<TD::scheduler>;
        }),
    KT::expect_success("no_env doesn't have get_scheduler", []{
            return not TD::has_get_scheduler<::nstd::hidden_names::exec_envs::no_env>
                && not TD::has_get_scheduler<::nstd::hidden_names::exec_envs::no_env const>
                && not TD::has_get_scheduler<::nstd::hidden_names::exec_envs::no_env volatile>
                && not TD::has_get_scheduler<::nstd::hidden_names::exec_envs::no_env const volatile>
                ;
        }),
    KT::expect_success("get_scheduler on an object supporting it", []{
            int object;
            return TD::has_get_scheduler<TD::env<true, TD::scheduler>>
                && noexcept(EX::get_scheduler(TD::env<true, TD::scheduler>{&object}))
                && TD::scheduler{&object} == EX::get_scheduler(TD::env<true, TD::scheduler>{&object})
                ;
        }),
    KT::expect_success("get_scheduler can't throw", []{
            return not TD::has_get_scheduler<TD::env<false, TD::scheduler>>
                ;
        }),
    KT::expect_success("get_scheduler returns a scheduler", []{
            return not TD::has_get_scheduler<TD::env<true, TD::non_scheduler>>
                ;
        }),
};

static KT::add_tests suite("[exec.queries.get_scheduler]", ::tests);
