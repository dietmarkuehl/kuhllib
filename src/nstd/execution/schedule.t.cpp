// nstd/execution/schedule.t.cpp                                      -*-C++-*-
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

#include "nstd/execution/schedule.hpp"
#include "nstd/execution/sender_base.hpp"
#include "nstd/utility/forward.hpp"
#include <type_traits> //-dk:TODO "nstd/utility/conditional.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {};
namespace TD = ::test_declarations;
namespace EX = ::nstd::execution;
namespace TT = ::std; //-dk:TODO ::nstd::type_traits;
namespace UT = ::nstd::utility;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    namespace
    {
        struct empty {};
        template <bool Sender>
        struct sender
            : TT::conditional_t<Sender, EX::sender_base, empty>
        {
            int value;
        };

        template <bool Sender>
        struct scheduler {
            int value;
            bool operator== (scheduler const&) const { return true; }
            friend TD::sender<Sender> tag_invoke(EX::schedule_t, scheduler s) {
                return TD::sender<Sender>{{}, s.value};
            } 
        };

        template <typename Scheduler>
        concept has_schedule
            = requires(Scheduler&& s) { EX::schedule(UT::forward<Scheduler>(s)); }
            ;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("sender<true> is a sender", []{
        return EX::sender<TD::sender<true>>;
    }),
    KT::expect_success("sender<false> is not a sender", []{
        return not EX::sender<TD::sender<false>>;
    }),
    KT::expect_success("scheduler has schedule()", []{
        return TD::has_schedule<TD::scheduler<true>>;
    }),
    KT::expect_success("schedule invokes the scheduler's tag_invoke method", []{
        return EX::schedule(TD::scheduler<true>{17}).value == 17
            ;
    }),
    KT::expect_success("schedule doesn't exist for scheduler returning non-sender", []{
        return not TD::has_schedule<TD::scheduler<false>>;
    }),
};

static KT::add_tests suite("schedule", ::tests);
