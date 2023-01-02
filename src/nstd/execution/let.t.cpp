// nstd/execution/let.t.cpp                                           -*-C++-*-
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

#include "nstd/execution/let.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/error_types_of_t.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/execution/on.hpp"
#include "nstd/execution/repeat_effect_until.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/schedule.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/hidden_names/print_completion_signatures.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/thread/sync_wait.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include <concepts>
#include "kuhl/test.hpp"

namespace HN = ::nstd::hidden_names;

namespace test_declarations {}
namespace TD = test_declarations;
namespace KT = ::kuhl::test;
namespace EX = ::nstd::execution;
namespace HN = ::nstd::hidden_names;
namespace TT = ::nstd::type_traits;
namespace TTh = ::nstd::this_thread;
namespace UT = ::nstd::utility;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        template <typename... T>
        using variant_t = ::nstd::execution::completion_signatures<::nstd::execution::set_error_t(T)...>;

        template <int Value>
        struct static_just {
            using completion_signatures = EX::completion_signatures<EX::set_value_t(int)>;

            template <EX::receiver Receiver>
            struct state {
                TT::remove_cvref_t<Receiver> receiver;
                friend auto tag_invoke(EX::start_t, state& self) noexcept -> void {
                    EX::set_value(UT::move(self.receiver), Value);
                }
            };
            template <EX::receiver Receiver>
            friend auto tag_invoke(EX::connect_t, static_just const&, Receiver&& receiver) -> state<Receiver> {
                return { UT::forward<Receiver>(receiver) };
            }
        };

        template <typename R0, typename R1>
        struct scheduler_sender;
        template <typename R0, typename R1>
        struct scheduler {
            friend auto tag_invoke(EX::schedule_t, scheduler) -> TD::scheduler_sender<R0, R1> { return {}; }
            auto operator== (scheduler const&) const -> bool = default;
        };
        template <typename R0, typename R1>
        struct scheduler_sender {
            using completion_signatures = EX::completion_signatures<>;
            friend auto tag_invoke(EX::get_completion_scheduler_t<EX::set_value_t>, scheduler_sender const&) noexcept -> TD::scheduler<R0, R1> { return {}; }

            template <typename Fun>
            friend auto tag_invoke(EX::let_value_t, scheduler<R0, R1>, scheduler_sender, Fun&&) -> R0 { return {}; }
            template <typename Fun>
            friend auto tag_invoke(EX::let_value_t, scheduler_sender, Fun&&) -> R1 { return {}; }
        };

        struct non_sender {};
        struct sender {
            using completion_signatures = EX::completion_signatures<>;
        };

        struct non_fun {};
        struct fun {
            template <typename... T>
            void operator()(T&&...) const noexcept {}
        };

        struct env {};
        struct receiver {
            friend env tag_invoke(EX::get_env_t, receiver const&) noexcept { return {}; }
            friend void tag_invoke(EX::set_value_t, receiver, auto&&...) noexcept {}
            friend void tag_invoke(EX::set_error_t, receiver, auto&&) noexcept {}
            friend void tag_invoke(EX::set_stopped_t, receiver) noexcept {}
        };
        static_assert(EX::receiver<TD::receiver>);

        template <typename... E>
        struct error_sender {
            using completion_signatures = EX::completion_signatures<
                EX::set_value_t(),
                EX::set_error_t(E)...
                >;
            
            template <EX::receiver R>
            struct state {
                TT::remove_cvref_t<R> receiver;
                friend auto tag_invoke(EX::start_t, state& self) noexcept -> void {
                    EX::set_value(UT::move(self.receiver));
                }
            };

            template <EX::receiver R>
            friend auto tag_invoke(EX::connect_t, error_sender const&, R&& r) {
                return state<R>{ UT::forward<R>(r) };
            }
        };

        template <int> struct error {};
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("TD::scheduler_sender is a sender", []{
            return EX::sender<TD::scheduler_sender<int, int>>
                && KT::type<TD::scheduler<int, int>> == KT::type<decltype(EX::get_completion_scheduler<EX::set_value_t>(TD::scheduler_sender<int, int>()))>
                ;
        }),
    KT::expect_success("TD::scheduler is a scheduler", []{
            return EX::scheduler<TD::scheduler<int, int>>
                ;
        }),
    KT::expect_success("TD::static_just a sender", []{
            return EX::sender<TD::static_just<1>>
                ;
        }),
    KT::expect_success("TD::non_sender is not a sender", []{
            return not EX::sender<TD::non_sender>
                ;
        }),
    KT::expect_success("TD::sender is a sender", []{
            return EX::sender<TD::sender>
                ;
        }),
    KT::expect_success("let_value with sender and fun exists", []{
            return ::std::invocable<EX::let_value_t const, TD::sender, TD::fun>
                && ::std::invocable<decltype(EX::let_value), TD::sender, TD::fun>
                ;
        }),
    //KT::expect_success("let_value with non-sender and fun does not exist", []{
    //        return not ::std::invocable<EX::let_value_t const, TD::non_sender, TD::fun>
    //            && not ::std::invocable<decltype(EX::let_value), TD::non_sender, TD::fun>
    //            ;
    //    }),
    KT::expect_success("let_error with sender and fun exists", []{
            return ::std::invocable<EX::let_error_t const, TD::sender, TD::fun>
                && ::std::invocable<decltype(EX::let_error), TD::sender, TD::fun>
                ;
        }),
    //KT::expect_success("let_error with non-sender and fun does not exist", []{
    //        return not ::std::invocable<EX::let_error_t const, TD::non_sender, TD::fun>
    //            && not ::std::invocable<decltype(EX::let_error), TD::non_sender, TD::fun>
    //            ;
    //    }),
    KT::expect_success("let_stopped with sender and fun exists", []{
            return ::std::invocable<EX::let_stopped_t const, TD::sender, TD::fun>
                && ::std::invocable<decltype(EX::let_stopped), TD::sender, TD::fun>
                ;
        }),
    //KT::expect_success("let_stopped with non-sender and fun does not exist", []{
    //        return not ::std::invocable<EX::let_stopped_t const, TD::non_sender, TD::fun>
    //            && not ::std::invocable<decltype(EX::let_stopped), TD::non_sender, TD::fun>
    //            ;
    //    }),
    // KT::expect_success("let_stopped with sender and non-fun does not exist", []{
    //         return not ::std::invocable<EX::let_stopped_t const, TD::sender, TD::non_fun>
    //             && not ::std::invocable<decltype(EX::let_stopped), TD::sender, TD::non_fun>
    //             ;
    //     }),
    KT::expect_success("custom let_value based on completion_scheduler", []{
            EX::sender auto sender = EX::let_value(TD::scheduler_sender<TD::static_just<1>, int>(), []{});
            return KT::type<decltype(sender)> == KT::type<TD::static_just<1>>
                ;
        }),
    KT::expect_success("custom let_value", []{
            EX::sender auto sender = EX::let_value(TD::scheduler_sender<int, TD::static_just<2>>(), []{});
            return KT::type<decltype(sender)> == KT::type<TD::static_just<2>>
                ;
        }),
    KT::expect_success("custom let_value with both approaches", []{
            EX::sender auto sender = EX::let_value(TD::scheduler_sender<TD::static_just<1>, TD::static_just<2>>(), []{});
            return KT::type<decltype(sender)> == KT::type<TD::static_just<1>>
                ;
        }),
#if 0
    KT::expect_success("custom let_value with pipe", []{
            //-dk:TODO enable pipe for let
            EX::sender auto sender = TD::scheduler_sender<int, TD::static_just<2>>() | EX::let_value([]{});
            return KT::type<decltype(sender)> == KT::type<TD::static_just<2>>
                ;
        }),
#endif
    KT::expect_success("TD::receiver is a receiver", []{
            TD::receiver r;
            auto env = EX::get_env(r);
            EX::set_value(UT::move(r), 1, 2);
            EX::set_error(UT::move(r), 1);
            EX::set_stopped(UT::move(r));
            return KT::use(env)
                ;
        }),
    KT::expect_success("send value", []{
            auto sender = EX::let_value(EX::just(::std::string("hello, "), ::std::string("world")),
                                        [](auto&&... a){
                                            return EX::just((a + ...));
                                            }
                                        );
            static_assert(EX::sender<decltype(sender)>);
            static_assert(EX::receiver_of<TD::receiver, decltype(EX::get_completion_signatures(sender, EX::get_env(TD::receiver())))>);

            auto value = TTh::sync_wait(UT::move(sender));
            return KT::use(sender)
                && value
                && ::std::get<0>(*value) == "hello, world"
                ;
        }),
    KT::expect_success("send value using pipe", []{
            auto sender = EX::just(::std::string("hello, "), ::std::string("world")) | EX::let_value(
                                    [](auto&&... a){
                                        return EX::just((a + ...)); })
                                    | EX::then([](::std::string s){ return s; })
                                    ;
            static_assert(EX::sender<decltype(sender)>);
            static_assert(EX::receiver_of<TD::receiver, decltype(EX::get_completion_signatures(sender, EX::get_env(TD::receiver())))>);

            auto value = TTh::sync_wait(UT::move(sender));
            return KT::use(sender)
                && value
                && ::std::get<0>(*value) == "hello, world"
                ;
        }),
    KT::expect_success("send stopped", []{
            auto sender = EX::let_stopped(EX::just_stopped(),
                                    [](){ return EX::just(std::string("stopped")); })
                                    ;
            static_assert(EX::sender<decltype(sender)>);
            static_assert(EX::receiver_of<TD::receiver, decltype(EX::get_completion_signatures(sender, EX::get_env(TD::receiver())))>);

            auto value = TTh::sync_wait(UT::move(sender));

            return KT::use(sender)
                && value
                //&& value->index() == 0
                && ::std::get<0>(*value) == "stopped"
                ;
        }),
    KT::expect_success("let_value can be copied", []{
            auto sender = EX::on(TD::scheduler<int, int>(), EX::just() | EX::let_value([]{ return EX::just(); }));
            auto copy(sender);
            return true
                && KT::use(sender)
                && KT::use(copy)
                ;
        }),
    KT::expect_success("repeat send one value", []{
            auto sender =
                EX::repeat_effect_until(
                      EX::just(::std::string("hello, "), ::std::string("world"))
                    | EX::let_value(
                                    [](auto&&... a){ return EX::just((a + ...)); })
                    , []{ return true; }
                )
                ;
            TTh::sync_wait(UT::move(sender));

            return KT::use(sender)
                ;
        }),
    KT::expect_success("let_value(upstream, fun) propagates upstreams errors", []{
        auto sender = EX::let_value(
            TD::error_sender<TD::error<0>>(),
            []{ return TD::error_sender<TD::error<1>>(); });
        // HN::print_completion_signatures(sender);
        return EX::sender<decltype(sender)>
            && ::std::same_as<EX::error_types_of_t<decltype(sender), TD::env, TD::variant_t>,
                              EX::completion_signatures<
                                  EX::set_error_t(TD::error<1>),
                                  EX::set_error_t(TD::error<0>)
                              >>
            ;
        })
};

static KT::add_tests suite("let_*", ::tests);
