// nstd/hidden_names/operation.t.cpp                                  -*-C++-*-
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

#include "nstd/hidden_names/operation.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/thread/sync_wait.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"
#include <thread>

namespace test_declarations {}
namespace EX = ::nstd::execution;
namespace TD = ::test_declarations;
namespace HN = ::nstd::hidden_names;
namespace TT = ::nstd::this_thread;
namespace UT = ::nstd::utility;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        template <typename...> struct variant;
        template <typename...> struct tuple;
        template <typename...> struct var;
        template <typename...> struct tup;
        template <int> struct action_t {
            template <typename R>
            struct base {
                R d_receiver;
            };
        };

        template <typename R>
        auto tag_invoke(action_t<1>, R&& receiver, int arg) {
            EX::set_value(UT::move(receiver.d_receiver), arg);
        } 
        inline constexpr HN::operation<action_t<1>, TD::variant<TD::tuple<int>>, TD::variant<std::exception_ptr>>
            action1;

        template <int> struct type {
            int value = 0;
            auto operator== (type const&) const -> bool = default;
        };
        template <typename R>
        auto tag_invoke(action_t<2>, R&& receiver, TD::type<0> a0, TD::type<1> a1, TD::type<2> a2) {
            EX::set_value(UT::move(receiver.d_receiver), a0, a1, a2);
        } 
        inline constexpr HN::operation<action_t<2>, TD::variant<TD::tuple<TD::type<0>, TD::type<1>, TD::type<2>>>, TD::variant<std::exception_ptr>>
            action2;

        template <typename R>
        auto tag_invoke(TD::action_t<3>, R&& receiver, int value) {
            ::std::thread([ptr = &receiver, value]{
                EX::set_value(UT::move(ptr->d_receiver), value * 2);
            }).detach();
        }
        inline constexpr HN::operation<TD::action_t<3>, TD::variant<TD::tuple<int>>, TD::variant<::std::exception_ptr>>
            action3;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("factory creation of a sender", []{
            auto sender = TD::action1(17);
            /*auto res =*/ TT::sync_wait(UT::move(sender));
            return KT::use(sender)
                && EX::sender<decltype(sender)>
                && KT::type<decltype(sender)::value_types<TD::var, TD::tup>>
                    == KT::type<TD::var<TD::tup<int>>>
                && KT::type<decltype(sender)::error_types<TD::var>>
                    == KT::type<TD::var<std::exception_ptr>>
                ;
        }),
    KT::expect_success("direct creation with a sender", []{
            auto sender = TD::action1(EX::just(17));
            /*auto res =*/ TT::sync_wait(UT::move(sender));
            return KT::use(sender)
                && EX::sender<decltype(sender)>
                ;
        }),
    KT::expect_success("combining sender with a pipe", []{
            auto sender = EX::just(17) | TD::action1();
            /*auto res =*/ TT::sync_wait(UT::move(sender));
            return KT::use(sender)
                //&& EX::sender<decltype(sender)>
                ;
        }),
    KT::expect_success("factory creation of a 3 arg sender", []{
            auto sender = TD::action2(TD::type<0>{42}, TD::type<1>{1}, TD::type<2>{17});
            TT::sync_wait(UT::move(sender)| EX::then([](auto, auto, auto){ return 0; }));
            return EX::sender<decltype(sender)>
                && KT::type<decltype(sender)::value_types<TD::var, TD::tup>>
                    == KT::type<TD::var<TD::tup<TD::type<0>, TD::type<1>, TD::type<2>>>>
                && KT::type<decltype(sender)::error_types<TD::var>>
                    == KT::type<TD::var<std::exception_ptr>>
                ;
        }),
    KT::expect_success("combining 1 arg sender with 2 args for a 3 arg sender", []{
            auto sender = EX::just(TD::type<2>{17}) | TD::action2(TD::type<0>{42}, TD::type<1>{1});
            TT::sync_wait(UT::move(sender)| EX::then([](auto, auto, auto){ return 0; }));
            return EX::sender<decltype(sender)>
                && KT::type<decltype(sender)::value_types<TD::var, TD::tup>>
                    == KT::type<TD::var<TD::tup<TD::type<0>, TD::type<1>, TD::type<2>>>>
                && KT::type<decltype(sender)::error_types<TD::var>>
                    == KT::type<TD::var<std::exception_ptr>>
                ;
        }),
    KT::expect_success("combining 2 arg sender with 1 args for a 3 arg sender", []{
            auto sender = EX::just(TD::type<1>{1}, TD::type<2>{17}) | TD::action2(TD::type<0>{42});
            TT::sync_wait(UT::move(sender)| EX::then([](auto, auto, auto){ return 0; }));
            return EX::sender<decltype(sender)>
                && KT::type<decltype(sender)::value_types<TD::var, TD::tup>>
                    == KT::type<TD::var<TD::tup<TD::type<0>, TD::type<1>, TD::type<2>>>>
                && KT::type<decltype(sender)::error_types<TD::var>>
                    == KT::type<TD::var<std::exception_ptr>>
                ;
        }),
    KT::expect_success("combining 3 arg sender with 0 args for a 3 arg sender", []{
            auto sender = EX::just(TD::type<0>{0}, TD::type<1>{1}, TD::type<2>{17}) | TD::action2();
            TT::sync_wait(UT::move(sender)| EX::then([](auto, auto, auto){ return 0; }));
            return EX::sender<decltype(sender)>
                && KT::type<decltype(sender)::value_types<TD::var, TD::tup>>
                    == KT::type<TD::var<TD::tup<TD::type<0>, TD::type<1>, TD::type<2>>>>
                && KT::type<decltype(sender)::error_types<TD::var>>
                    == KT::type<TD::var<std::exception_ptr>>
                ;
        }),
    KT::expect_success("test use of state in base class", []{
            auto sender = TD::action3(4);
            auto res = TT::sync_wait(UT::move(sender));
            return KT::use(res)
                && KT::type<decltype(res)> == KT::type<::std::optional<::std::variant<int>>>
                && res
                && ::std::get<0>(*res) == 8
                ;
        }),
};

static KT::add_tests suite("operation", ::tests);
