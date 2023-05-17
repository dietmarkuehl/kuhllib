// nstd/execution/repeat_effect.t.cpp                                 -*-C++-*-
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

#include "nstd/execution/repeat_effect.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/thread/sync_wait.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;
namespace EX = ::nstd::execution;
namespace TR = ::nstd::this_thread;
namespace TT = ::nstd::type_traits;
namespace UT = ::nstd::utility;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct error {};

        template <EX::receiver Receiver>
        struct state
        {
            TT::remove_cvref_t<Receiver> d_receiver;
            int*                         d_value;
            friend auto tag_invoke(EX::start_t, state& s)
                noexcept -> void {
                switch (++*s.d_value) {
                default:
                    EX::set_value(UT::move(s.d_receiver));
                    break;
                case 5:
                    EX::set_error(UT::move(s.d_receiver), TD::error{});
                    break;
                case 8:
                    EX::set_stopped(UT::move(s.d_receiver));
                    break;
                }
            }
        };
        struct sender
        {
            using completion_signatures = EX::completion_signatures<>;

            int* d_value;

            template <EX::receiver Receiver>
            friend auto tag_invoke(EX::connect_t, sender const& s, Receiver&& receiver)
                noexcept -> TD::state<Receiver> {
                return TD::state<Receiver>{UT::forward<Receiver>(receiver), s.d_value };
            }
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("dummy", []{
        return false;
    }),
    #if 0
    KT::expect_success("run till error", []{
            int  value = 4;
            bool caught = false;
            auto sender = EX::repeat_effect(TD::sender{&value});
            try {
                TR::sync_wait(UT::move(sender));
            }
            catch (TD::error const&) {
                caught = true;
            }
            return value == 5
                && caught
                ;
        }),
    KT::expect_success("run till cancelled", []{
            int value = 6;
            auto sender = EX::repeat_effect(TD::sender{&value});
            auto rc = TR::sync_wait(UT::move(sender));
            return value == 8
                && !rc
                ;
        }),
        #endif
};

static KT::add_tests suite("repeat_effect", ::tests);
