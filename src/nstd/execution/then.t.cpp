// nstd/execution/then.t.cpp                                          -*-C++-*-
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

#include "nstd/execution/then.hpp"
#include "nstd/execution/lazy_then.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/typed_sender.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/thread/sync_wait.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace EX = ::nstd::execution;
namespace TT = ::nstd::this_thread;
namespace UT = ::nstd::utility;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        template <typename...> struct V;
        template <typename...> struct T;
        struct result { int value; };

        template <typename Sender, typename Tag>
        concept has_completion_scheduler
            = requires(Sender& sender) {
                EX::get_completion_scheduler<Tag>(sender);
            }
            ;

        template <bool, bool> struct scheduled_sender;
        template <bool Eager>
        struct scheduler {
            bool operator== (scheduler const&) const { return true; }
            template <bool E>
            friend auto tag_invoke(EX::schedule_t, scheduler<E>&& s) -> scheduled_sender<E, true>;
        };

        template <bool Eager, bool HasScheduler>
        struct scheduled_sender {
            template <template <typename...> class T, template <typename...> class V>
            using value_types = V<T<int>>;
            template <template <typename...> class V>
            using error_types = V<::std::exception_ptr>;
            static constexpr bool sends_done = false;

            friend auto tag_invoke(EX::get_completion_scheduler_t<EX::set_value_t>, scheduled_sender const&) noexcept(HasScheduler) {
                return TD::scheduler<Eager>();
            }
        };

        template <bool E>
        auto tag_invoke(EX::schedule_t, scheduler<E>&&) -> scheduled_sender<E, true> {
            return {};
        }

        template <int>
        struct lazy_sender {
            template <template <typename...> class T, template <typename...> class V>
            using value_types = V<T<int>>;
            template <template <typename...> class V>
            using error_types = V<::std::exception_ptr>;
            static constexpr bool sends_done = false;
        };

        template <EX::sender Sender, typename Fun>
        auto tag_invoke(EX::lazy_then_t, TD::scheduler<false>, Sender&&, Fun&&) {
            return TD::lazy_sender<3>();
        }

        template <typename Fun>
        auto tag_invoke(EX::lazy_then_t, TD::scheduled_sender<false, false>&&, Fun&&) {
            return TD::lazy_sender<2>();
        }

        template <int>
        struct eager_sender {
            template <template <typename...> class T, template <typename...> class V>
            using value_types = V<T<int>>;
            template <template <typename...> class V>
            using error_types = V<::std::exception_ptr>;
            static constexpr bool sends_done = false;
        };

        template <EX::sender Sender, typename Fun>
        auto tag_invoke(EX::then_t, TD::scheduler<true>, Sender&&, Fun&&) {
            return TD::eager_sender<3>();
        }

        template <bool Scheduled, typename Fun>
        auto tag_invoke(EX::then_t, TD::scheduled_sender<true, Scheduled>&&, Fun&&) {
            return TD::eager_sender<2>();
        }
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("TD::scheduler is a scheduler", []{
            return EX::scheduler<TD::scheduler<true>>
                && EX::scheduler<TD::scheduler<true>>
                ;
        }),
    KT::expect_success("TD::scheduled_sender is a sender", []{
            return EX::sender<TD::scheduled_sender<true, true>>
                && EX::sender<TD::scheduled_sender<true, false>>
                && EX::sender<TD::scheduled_sender<false, true>>
                && EX::sender<TD::scheduled_sender<false, false>>
                ;
        }),
    KT::expect_success("TD::eager_sender is a sender", []{
            return EX::sender<TD::eager_sender<0>>
                && EX::sender<TD::eager_sender<1>>
                && EX::sender<TD::eager_sender<2>>
                && EX::sender<TD::eager_sender<3>>
                ;
        }),
    KT::expect_success("TD::lazy_sender is a sender", []{
            return EX::sender<TD::lazy_sender<0>>
                && EX::sender<TD::lazy_sender<1>>
                && EX::sender<TD::lazy_sender<2>>
                && EX::sender<TD::lazy_sender<3>>
                ;
        }),
    KT::expect_success("TD::scheduled_sender<true> has get_completion_scheduler", []{
            return TD::has_completion_scheduler<TD::scheduled_sender<true, true>, EX::set_value_t>
                && TD::has_completion_scheduler<TD::scheduled_sender<false, true>, EX::set_value_t>
                ;
        }),
    KT::expect_success("TD::scheduled_sender<false> does not have get_completion_scheduler", []{
            return not TD::has_completion_scheduler<TD::scheduled_sender<true, false>, EX::set_value_t>
                && not TD::has_completion_scheduler<TD::scheduled_sender<false, false>, EX::set_value_t>
                ;
        }),
    KT::expect_success("custom then with scheduler", []{
            auto sender = EX::then(TD::scheduled_sender<true, true>(), []{});
            return KT::use(sender)
                && KT::type<decltype(sender)> == KT::type<TD::eager_sender<3>>
                ;
        }),
    KT::expect_success("custom then without scheduler", []{
            auto sender = EX::then(TD::scheduled_sender<true, false>(), []{});
            return KT::use(sender)
                && KT::type<decltype(sender)> == KT::type<TD::eager_sender<2>>
                ;
        }),
    KT::expect_success("custom lazy_then with scheduler", []{
            auto sender = EX::then(TD::scheduled_sender<false, true>(), []{});
            return KT::use(sender)
                && KT::type<decltype(sender)> == KT::type<TD::lazy_sender<3>>
                ;
        }),
    KT::expect_success("custom lazy_then without scheduler", []{
            auto sender = EX::then(TD::scheduled_sender<false, false>(), []{});
            return KT::use(sender)
                && KT::type<decltype(sender)> == KT::type<TD::lazy_sender<2>>
                ;
        }),
    KT::expect_success("then with just", []{
            auto sender = EX::then(EX::just(3, 4), [](int a, int b){ return TD::result{a * b}; });
            auto res = TT::sync_wait(UT::move(sender));
            return KT::use(sender)
                && EX::sender<decltype(sender)>
                && EX::typed_sender<decltype(sender)>
                && KT::use(res)
                && KT::type<decltype(res)> == KT::type<::std::optional<::std::variant<TD::result>>>
                && res
                && ::std::get<0>(*res).value == 12
                ;
        }),
    KT::expect_success("then can be chained", []{
            auto sender = EX::then(EX::then(EX::just(4, 3),
                                            [](auto a, auto b){ return TD::result{a * b}; }
                                            ),
                                   [](TD::result a){  return a.value; }
                                   //-dk:TODO support [](auto a){  return a.value; }
                                   );
            auto res = TT::sync_wait(UT::move(sender)); KT::use(res);

            return KT::use(sender)
                && EX::sender<decltype(sender)>
                && EX::typed_sender<decltype(sender)>
                && KT::type<decltype(sender)::value_types<TD::T, TD::V>> == KT::type<TD::V<TD::T<int>>>
                && ::std::get<0>(*res) == 12
                ;
        }),
    KT::expect_success("then can be piped", []{
            auto sender = EX::just(3, 4)
                        | EX::then([](int a, int b){ return TD::result{a * b}; })
                        | EX::then([](TD::result r){ return r.value; })
                        ;
            auto res = TT::sync_wait(UT::move(sender));
            return KT::use(sender)
                && EX::sender<decltype(sender)>
                && EX::typed_sender<decltype(sender)>
                && KT::type<decltype(res)> == KT::type<::std::optional<::std::variant<int>>>
                && res
                && ::std::get<0>(*res) == 12
                ;
        }),
};

static KT::add_tests suite("then", ::tests);
