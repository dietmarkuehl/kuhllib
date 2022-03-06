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
#include "nstd/execution/completion_signatures.hpp"
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
        struct env {};
        struct error {};
        struct type {};

        template <bool ConnectNoexcept, bool StartNoexcept, typename T>
        struct sender
        {
            using completion_signatures = EX::completion_signatures<
                EX::set_error_t(TD::error),
                EX::set_stopped_t(),
                EX::set_value_t(T)
                >;
            
            template <EX::receiver Receiver>
            struct state {
                Receiver receiver;
                friend void tag_invoke(EX::start_t, state& s) noexcept(StartNoexcept) {
                    EX::set_stopped(UT::move(s.receiver));
                };
            };
            int* const ptr;
            explicit sender(int* ptr): ptr(ptr) {}
            template <EX::receiver Receiver>
            friend auto tag_invoke(EX::connect_t, sender&& sender, Receiver&& receiver) noexcept(ConnectNoexcept)
                -> state<Receiver> {
                *sender.ptr = 42;
                return { ::UT::forward<Receiver>(receiver) };
            }
        };

        template <typename T>
        struct receiver {
            friend auto tag_invoke(EX::get_env_t, receiver const&) -> TD::env { return {}; }
            friend auto tag_invoke(EX::set_error_t, receiver&&, TD::error) noexcept -> void {}
            friend auto tag_invoke(EX::set_stopped_t, receiver&&) noexcept -> void {}
            friend auto tag_invoke(EX::set_value_t, receiver&&, T) noexcept -> void {}
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

                EX::connect(UT::move(sender), Receiver());
                return value == 42
                    && Noexcept == noexcept(EX::connect(UT::move(sender), Receiver()))
                    ;
            }
            return false == HasConnect;
        }
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("placeholder", []{
            return true;
        }),
    KT::expect_success("sender<true, true, TD::type> is a sender", []{
            return EX::sender<TD::sender<true, true, TD::type>>;
        }),
    KT::expect_success("sender<true, true, int> is a sender", []{
            return EX::sender<TD::sender<true, true, int>>;
        }),
    KT::expect_success("sender<true, false, TD::type> is a sender", []{
            return EX::sender<TD::sender<true, false, TD::type>>;
        }),
    KT::expect_success("sender<true, false, int> is a sender", []{
            return EX::sender<TD::sender<true, false, int>>;
        }),
    KT::expect_success("sender<false, true, TD::type> is a sender", []{
            return EX::sender<TD::sender<false, true, TD::type>>;
        }),
    KT::expect_success("sender<false, false, TD::type> is a sender", []{
            return EX::sender<TD::sender<false, false, TD::type>>;
        }),
    KT::expect_success("receiver<TD::type> is a receiver", []{
            return EX::receiver<TD::receiver<TD::type>>;
        }),
    KT::expect_success("receiver<int> is a receiver", []{
            return EX::receiver<TD::receiver<int>>;
        }),
    KT::expect_success("sender<true, true, TD::type>::state<TD::receiver<TD::type>> is an operation_state", []{
            return EX::operation_state<TD::sender<true, true, TD::type>::state<TD::receiver<TD::type>>>;
        }),
    KT::expect_success("sender<true, true, int>::state<TD::receiver<int>> is an operation_state", []{
            return EX::operation_state<TD::sender<true, true, int>::state<TD::receiver<int>>>;
        }),
    KT::expect_success("sender<true, false, TD::type>::state<receiver<TD::type>> is not an operation_state", []{
            return not EX::operation_state<TD::sender<true, false, TD::type>::state<TD::receiver<TD::type>>>;
        }),
    KT::expect_success("receiver_of<receiver<int>, set_error_t(TD::error)> == true", []{
            return EX::receiver_of<TD::receiver<int>, EX::completion_signatures<EX::set_error_t(TD::error)>>;
        }),
    KT::expect_success("receiver_of<receiver<int>, set_stopped_t()> == true", []{
            return EX::receiver_of<TD::receiver<int>, EX::completion_signatures<EX::set_stopped_t()>>;
        }),
    KT::expect_success("receiver_of<receiver<int>, set_value_t(int)> == true", []{
            return EX::receiver_of<TD::receiver<int>, EX::completion_signatures<EX::set_value_t(int)>>;
        }),
    KT::expect_success("has_connect<sender<true, true, int>, receiver<int>> == true", []{
            return TD::has_connect< TD::sender<true, true, int>, TD::receiver<int>>;
        }),
    KT::expect_success("sender<true, true, TD::type>, receiver<TD::type> => connect() noexcept(true)", []{
            return TD::check_connect<true, true, TD::sender<true, true, int>, TD::receiver<int>>();
        }),
    KT::expect_success("sender<false, true, true>, receiver<true> => connect() noexcept(false)", []{
            return TD::check_connect<true, false, TD::sender<false, true, int>, TD::receiver<int>>();
        }),
    KT::expect_success("sender<true, false, int>, receiver<int> => no connect()", []{
            return TD::check_connect<false, true, TD::sender<true, false, int>, TD::receiver<int>>();
        }),
    KT::expect_success("sender<true, true, int>, receiver<TD::type> => no connect()", []{
            return TD::check_connect<false, true, TD::sender<true, true, int>, TD::receiver<TD::type>>();
        }),
};

static KT::add_tests suite("execution::connect", ::tests);
