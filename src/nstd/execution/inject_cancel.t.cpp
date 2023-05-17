// nstd/execution/inject_cancel.t.cpp                                 -*-C++-*-
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

#include "nstd/execution/inject_cancel.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/execution/get_stop_token.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/stop_token/in_place_stop_token.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/thread/sync_wait.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"
#include <exception>

namespace test_declarations {}
namespace EX = ::nstd::execution;
namespace ST = ::nstd::stop_token_ns;
namespace TT = ::nstd::type_traits;
namespace NT = ::nstd::this_thread;
namespace UT = ::nstd::utility;
namespace KT = ::kuhl::test;
namespace TD = test_declarations;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    namespace
    {
        template <EX::receiver Receiver>
        struct state
        {
            TT::remove_cvref_t<Receiver> d_receiver;
            friend auto tag_invoke(EX::start_t, state& self) noexcept -> void
            {
                auto token{EX::get_stop_token(EX::get_env(self.d_receiver))};
                if (token.stop_requested()) {
                    EX::set_stopped(UT::move(self.d_receiver));
                }
                else try {
                    EX::set_value(UT::move(self.d_receiver));
                }
                catch (...) {
                    EX::set_error(UT::move(self.d_receiver), ::std::current_exception());
                }
            }
        };

        struct sender
        {
            using completion_signatures
                = ::nstd::execution::completion_signatures<
                    ::nstd::execution::set_value_t()
                    >;

            template <EX::receiver Receiver>
            friend auto tag_invoke(EX::connect_t, sender, Receiver&& receiver)
                -> TD::state<Receiver>
            {
                return { UT::forward<Receiver>(receiver) };
            }
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_failure("breathing", []{
            ST::in_place_stop_source source;
            bool then_called{false};
            bool upon_stopped_called{false};

            auto sender
                = TD::sender()
                | EX::inject_cancel(source.token())
                | EX::then([&]{ then_called = true; return true; })
                | EX::upon_stopped([&]{ upon_stopped_called = true; return true; })
                ;
            static_assert(EX::sender<decltype(sender)>);

            //-dk:TODO NT::sync_wait(UT::move(sender));
            (void)sender;
            return KT::use(sender)
                && then_called
                && not upon_stopped_called
                ;
        }),
    KT::expect_failure("canceled", []{
            ST::in_place_stop_source source;
            bool then_called{false};
            bool upon_stopped_called{false};

            auto sender
                = TD::sender()
                | EX::inject_cancel(source.token())
                | EX::then([&]{ then_called = true; return true; })
                | EX::upon_stopped([&]{ upon_stopped_called = true; return true; })
                ;
            source.stop();
            NT::sync_wait(UT::move(sender));
            return KT::use(sender)
                && not then_called
                && upon_stopped_called
                ;
        }),
};

static KT::add_tests suite("inject_cancel", ::tests);
