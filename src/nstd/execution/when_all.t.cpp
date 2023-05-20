// nstd/execution/when_all.t.cpp                                      -*-C++-*-
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

#include "nstd/concepts/invocable.hpp"
#include "nstd/execution/when_all.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/thread/sync_wait.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
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
        struct env {};

        template <int... I>
        struct static_just {
            template <EX::receiver R>
            struct state {
                TT::remove_cvref_t<R> d_receiver;
                friend auto tag_invoke(EX::start_t, state& self) noexcept -> void {
                    EX::set_value(UT::move(self.d_receiver), I...);
                }
            };
            friend auto tag_invoke(EX::get_completion_signatures_t, static_just const&, auto&&) noexcept
                -> EX::completion_signatures<EX::set_value_t(decltype(I)...)> {
                return {};
            }
            template <EX::receiver R>
            friend auto tag_invoke(EX::connect_t, static_just const&, R&& receiver) noexcept
                -> state<R>
            {
                return { UT::forward<R>(receiver) };
            }
        };

        struct error {
            int i;
        };

        template <typename T, T I>
        struct static_just_error {
            template <EX::receiver R>
            struct state {
                TT::remove_cvref_t<R> d_receiver;
                friend auto tag_invoke(EX::start_t, state& self) noexcept -> void {
                    EX::set_error(UT::move(self.d_receiver), I);
                }
            };
            friend auto tag_invoke(EX::get_completion_signatures_t, static_just_error const&, auto&&) noexcept
                -> EX::completion_signatures<EX::set_value_t(), EX::set_error_t(T)> {
                return {};
            }
            template <EX::receiver R>
            friend auto tag_invoke(EX::connect_t, static_just_error const&, R&& receiver) noexcept
                -> state<R>
            {
                return { UT::forward<R>(receiver) };
            }
        };

        template <typename T>
        struct static_just_error_default {
            template <EX::receiver R>
            struct state {
                TT::remove_cvref_t<R> d_receiver;
                friend auto tag_invoke(EX::start_t, state& self) noexcept -> void {
                    EX::set_error(UT::move(self.d_receiver), T());
                }
            };
            friend auto tag_invoke(EX::get_completion_signatures_t, static_just_error_default const&, auto&&) noexcept
                -> EX::completion_signatures<EX::set_value_t(), EX::set_error_t(T)> {
                return {};
            }
            template <EX::receiver R>
            friend auto tag_invoke(EX::connect_t, static_just_error_default const&, R&& receiver) noexcept
                -> state<R>
            {
                return { UT::forward<R>(receiver) };
            }
        };

        auto tag_invoke(EX::when_all_t, TD::static_just<1>, TD::static_just<2, 3>) {
            return TD::static_just<2, 4, 6>();
        }
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("TD::static_just is a sender", []{
            static_assert(EX::sender<TD::static_just<>>);
            auto value = TR::sync_wait(TD::static_just<1, 2>());

            return EX::sender<TD::static_just<>>
                && EX::sender<TD::static_just<1>>
                && EX::sender<TD::static_just<1, 2>>
                && KT::use(value)
                && value
                && ::std::make_tuple(1, 2) == value
                ;
        }),
    KT::expect_success("when_all without arguments is ill-formed", []{
            return not ::nstd::concepts::invocable<EX::when_all_t>;
        }),
    KT::expect_success("when_all is a CPO", []{
            auto sender = EX::when_all(TD::static_just<1>(), TD::static_just<2, 3>());
            auto value  = TR::sync_wait(UT::move(sender));
            return KT::use(value)
                && value
                && ::std::make_tuple(2, 4, 6) == *value
                ;
        }),
    KT::expect_success("when_all with one sender", []{
            auto sender = EX::when_all(TD::static_just<1>());
            auto result = TR::sync_wait(::UT::move(sender));

            return KT::use(result)
                && KT::type<decltype(EX::get_completion_signatures(sender, TD::env()))>
                    == KT::type<EX::completion_signatures<EX::set_value_t(int), EX::set_stopped_t()>>
                ;
        }),
    KT::expect_success("when_all with two senders", []{
            auto sender = EX::when_all(TD::static_just<1, 2>(), TD::static_just<3>());
            auto result = TR::sync_wait(::UT::move(sender));

            return KT::use(result)
                && KT::type<decltype(EX::get_completion_signatures(sender, TD::env()))>
                    == KT::type<EX::completion_signatures<EX::set_value_t(int, int, int), EX::set_stopped_t()>>
                ;
        }),
    KT::expect_success("when_all with error", []{
            auto sender = EX::when_all(TD::static_just<1, 2>(), TD::static_just_error<int, 3>(), TD::static_just_error<TD::error, TD::error{1}>());
            bool result = false;
            try {
                TR::sync_wait(::UT::move(sender));
            }
            catch (int const&) {
                result = true;
            }

            return KT::use(result)
                && KT::type<decltype(EX::get_completion_signatures(sender, TD::env()))>
                    == KT::type<EX::completion_signatures<
                      EX::set_value_t(int, int),
                      EX::set_error_t(int),
                      EX::set_error_t(TD::error),
                      EX::set_stopped_t()
                      >>
                ;
        }),
    KT::expect_success("when_all with different errors", []{
            auto sender = EX::when_all(
                EX::just(1, 2, 3),
                EX::just(0, true),
                TD::static_just_error_default<::std::exception_ptr>(),
                TD::static_just_error_default<::std::error_code>()
                );
                TR::sync_wait(UT::move(sender));
            return KT::use(sender)
                && true
                ;
        }),
#if 0
    KT::expect_success("when_all without any sender", []{
            auto sender = EX::when_all();
            using Sender = decltype(sender);
            bool called = false;
            TR::sync_wait(UT::move(sender)
                          | EX::then([&]{ called = true; }));

            return KT::use(sender)
                && EX::sender<Sender>
                && KT::type<Sender::completion_signatures>
                   == KT::type<EX::completion_signatures<EX::set_value_t()>>
                && called
                ;
        }),
    KT::expect_success("when_all with one sender", []{
            ::std::size_t sum(0);
            auto sender = EX::when_all(EX::just(1));
            using Sender = TT::remove_cvref_t<decltype(sender)>;
            bool called = false;
            TR::sync_wait(UT::move(sender)
                          //| EX::then([&](auto&&...){ called = true; }));
            );

            return KT::use(sender)
                && EX::sender<Sender>
                && called
                && sum == 6u
                ;
        }),
    KT::expect_success("when_all with multiple senders", []{
            ::std::size_t sum(0);
            auto sender = EX::when_all(
                EX::just(1) | EX::then([&sum](int v){ sum += v; }),
                EX::just(2) | EX::then([&sum](int v){ sum += v; }),
                EX::just(3) | EX::then([&sum](int v){ sum += v; })
                );
            using Sender = TT::remove_cvref_t<decltype(sender)>;
            bool called = false;
            TR::sync_wait(UT::move(sender)
                          | EX::then([&](auto&&...){ called = true; }));

            return KT::use(sender)
                && EX::sender<Sender>
                && called
                && sum == 6u
                ;
        }),
#endif
};

static KT::add_tests suite("when_all", ::tests);
