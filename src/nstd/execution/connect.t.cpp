// nstd/execution/connect.t.cpp                                       -*-C++-*-
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

#include "nstd/execution/connect.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/utility/move.hpp"
#include <type_traits> //-dk:TODO "nstd/type_traits/conditional.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
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

        template <bool Sender, bool Noexcept, bool State>
        struct sender
            : TT::conditional_t<Sender, EX::sender_base, TD::empty>
        {
            template <typename Receiver>
            struct state {
                Receiver receiver;
                friend void tag_invoke(EX::start_t, state& s) noexcept(State) {
                    EX::set_stopped(UT::move(s.receiver));
                };
            };
            int* const ptr;
            explicit sender(int* ptr): ptr(ptr) {}
            template <typename Receiver>
            friend auto tag_invoke(EX::connect_t, sender&& sender, Receiver&& receiver) noexcept(Noexcept)
                -> state<Receiver> {
                *sender.ptr = 42;
                return { ::UT::forward<Receiver>(receiver) };
            }
        };

        template <bool Receiver>
        struct receiver {
            friend auto tag_invoke(EX::set_error_t, receiver&&, ::std::exception_ptr) noexcept(Receiver) -> void {}
            friend auto tag_invoke(EX::set_stopped_t, receiver&&) noexcept -> void {}
        };

        template <typename Sender, typename Receiver>
        concept has_connect
            = requires(Sender&& sender, Receiver&& receiver) {
                EX::connect(UT::move(sender), UT::move(receiver));
            }
            ;

        template <bool HasConnect, bool Noexcept, typename Sender, typename Receiver>
        auto check_connect() -> bool {
            if constexpr (TD::has_connect<Sender, Receiver>) {
                int    value(17);
                Sender sender(&value);

                EX::connect(UT::move(sender), TD::receiver<true>());
                return value == 42
                    && Noexcept == noexcept(EX::connect(UT::move(sender), TD::receiver<true>()))
                    ;
            }
            return false == HasConnect;
        }
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("sender<true, true, true> is a sender", []{
            return EX::sender<TD::sender<true, true, true>>;
        }),
    KT::expect_success("sender<true, true, false> is a sender", []{
            return EX::sender<TD::sender<true, true, false>>;
        }),
    KT::expect_success("sender<true, false, true> is a sender", []{
            return EX::sender<TD::sender<true, false, true>>;
        }),
    KT::expect_success("sender<true, false, false> is a sender", []{
            return EX::sender<TD::sender<true, false, false>>;
        }),
    KT::expect_success("sender<false, true, true> is not a sender", []{
            return not EX::sender<TD::sender<false, true, true>>;
        }),
    KT::expect_success("sender<false, false, false> is not a sender", []{
            return not EX::sender<TD::sender<false, false, false>>;
        }),
    KT::expect_success("receiver<true> is a receiver", []{
            return EX::receiver<TD::receiver<true>>;
        }),
    KT::expect_success("receiver<false> is not a receiver", []{
            return not EX::receiver<TD::receiver<false>>;
        }),
    KT::expect_success("sender<true, true, true>::state<receiver<true>> is an operation_state", []{
            return EX::operation_state<TD::sender<true, true, true>::state<TD::receiver<true>>>;
        }),
    KT::expect_success("sender<true, true, false>::state<receiver<true>> is not an operation_state", []{
            return not EX::operation_state<TD::sender<true, true, false>::state<TD::receiver<true>>>;
        }),
    KT::expect_success("sender<true, true, true>, receiver<true> => connect() noexcept(true)", []{
            return TD::check_connect<true, true, TD::sender<true, true, true>, TD::receiver<true>>();
        }),
    KT::expect_success("sender<true, false, true>, receiver<true> => connect() noexcept(false)", []{
            return TD::check_connect<true, false, TD::sender<true, false, true>, TD::receiver<true>>();
        }),
    KT::expect_success("sender<true, true, false>, receiver<true> => no connect()", []{
            return TD::check_connect<false, false, TD::sender<true, true, false>, TD::receiver<true>>();
        }),
    KT::expect_success("sender<true, false, false>, receiver<true> => no connect()", []{
            return TD::check_connect<false, false, TD::sender<true, false, false>, TD::receiver<true>>();
        }),
    KT::expect_success("sender<false, true, true>, receiver<true> => no connect()", []{
            return TD::check_connect<false, true, TD::sender<false, true, true>, TD::receiver<true>>();
        }),
    KT::expect_success("sender<true, true, true>, receiver<false> => no connect()", []{
            return TD::check_connect<false, true, TD::sender<true, true, true>, TD::receiver<false>>();
        }),
    KT::expect_success("sender<false, true, true>, receiver<false> => no connect()", []{
            return TD::check_connect<false, true, TD::sender<false, true, true>, TD::receiver<false>>();
        }),
};

static KT::add_tests suite("connect", ::tests);
