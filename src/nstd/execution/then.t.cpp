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
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include <optional>
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace EX = ::nstd::execution;
namespace NF = ::nstd;
namespace UT = ::nstd::utility;
namespace TT = ::nstd::type_traits;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        template <typename... T>
        using variant_t = ::nstd::execution::completion_signatures<::nstd::execution::set_error_t(T)...>;

        template <int> struct type {};
        struct env {};

        enum class result { none, value, error, stopped};

        template <typename Value>
        struct receiver {
            ::std::optional<Value>& d_called;
            TD::result&             d_result;
            receiver(::std::optional<Value>& called, TD::result& result): d_called(called), d_result(result) {}
            friend auto tag_invoke(EX::set_value_t, receiver&& self, Value&& value) noexcept -> void {
                self.d_called = value;
                self.d_result = result::value;
            }
            friend auto tag_invoke(EX::set_error_t, receiver&& self, auto&& ) noexcept -> void {
                self.d_result = result::error;
            }
            friend auto tag_invoke(EX::set_stopped_t, receiver&& self) noexcept -> void {
                self.d_result = result::stopped;
            }
            friend auto tag_invoke(EX::get_env_t, receiver const&) -> TD::env { return {}; }
        };

        template <>
        struct receiver<void> {
            TD::result& d_result;
            receiver(TD::result& result): d_result(result) {}
            friend auto tag_invoke(EX::set_value_t, receiver&& self) noexcept -> void {
                self.d_result = result::value;
            }
            friend auto tag_invoke(EX::set_error_t, receiver&& self, auto&& ) noexcept -> void {
                self.d_result = result::error;
            }
            friend auto tag_invoke(EX::set_stopped_t, receiver&& self) noexcept -> void {
                self.d_result = result::stopped;
            }
            friend auto tag_invoke(EX::get_env_t, receiver const&) -> TD::env { return {}; }
        };

        template <EX::sender Sender, EX::receiver Receiver>
        auto run(Sender&& sender, Receiver&& receiver) -> void {
            auto state(EX::connect(UT::forward<Sender>(sender), UT::forward<Receiver>(receiver)));
            EX::start(state);
        };

        template <typename Value>
        struct sender_with_then: EX::sender_tag {
            using completion_signatures = EX::completion_signatures<EX::set_value_t(Value)>;

            Value d_value;
            sender_with_then(Value value): d_value(value) {}

            template <typename Fun>
            friend auto tag_invoke(EX::then_t, sender_with_then&& self, Fun&&) {
                return EX::just(self.d_value);
            }
        };

        struct sender;
        struct scheduler {
            auto operator== (scheduler const&) const -> bool = default;
            friend auto tag_invoke(EX::schedule_t, scheduler const&) -> TD::sender;
        };
        struct sender {
            using completion_signatures = EX::completion_signatures<EX::set_value_t()>;
            friend auto tag_invoke(EX::get_completion_scheduler_t<EX::set_value_t>, sender const&) noexcept -> TD::scheduler { return {}; }
        };
        auto tag_invoke(EX::schedule_t, scheduler const&) -> TD::sender { return {}; }

        template <typename Value0, typename Value1>
        struct sender_with_scheduler_then: EX::sender_tag {
            using completion_signatures = EX::completion_signatures<EX::set_value_t(Value1)>;

            Value0 d_value0;
            Value1 d_value1;
            sender_with_scheduler_then(Value0 value0, Value1 value1): d_value0(value0), d_value1(value1) {}

            template <typename Fun>
            friend auto tag_invoke(EX::then_t, sender_with_scheduler_then&& self, Fun&&) {
                return EX::just(self.d_value0);
            }
            template <typename Fun>
            friend auto tag_invoke(EX::then_t, TD::scheduler, sender_with_scheduler_then&& self, Fun&&) {
                return EX::just(self.d_value1);
            }

            friend auto tag_invoke(EX::get_completion_scheduler_t<EX::set_value_t>, sender_with_scheduler_then const&) noexcept {
                return TD::scheduler();
            }
        };

        template <typename ConstSigs, typename RvalueSigs = ConstSigs>
        struct type_sender {
            template <typename E>
            friend auto tag_invoke(EX::get_completion_signatures_t, type_sender const&, E)
                -> ConstSigs {
                return {};
            }
            template <typename E>
            friend auto tag_invoke(EX::get_completion_signatures_t, type_sender&&, E)
                -> RvalueSigs {
                return {};
            }
        };

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
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("TD::run(): value", []{
        ::std::optional<int> value;
        TD::result           result(TD::result::none);
        TD::run(EX::just(17), TD::receiver(value, result));
        return value.has_value()
            && *value == 17
            && result == TD::result::value
            ;
    }),
    KT::expect_success("TD::run(): error", []{
        ::std::optional<int> value;
        TD::result           result(TD::result::none);
        TD::run(EX::just_error(17), TD::receiver(value, result));
        return not value.has_value()
            && result == TD::result::error
            ;
    }),
    KT::expect_success("TD::run(): stopped", []{
        ::std::optional<int> value;
        TD::result           result(TD::result::none);
        TD::run(EX::just_stopped(), TD::receiver(value, result));
        return not value.has_value()
            && result == TD::result::stopped
            ;
    }),
    KT::expect_success("TD::run(): void receiver/value", []{
        TD::result           result(TD::result::none);
        TD::run(EX::just(), TD::receiver<void>(result));
        return result == TD::result::value
            ;
    }),
    KT::expect_success("TD::run(): void receiver/error", []{
        TD::result           result(TD::result::none);
        TD::run(EX::just_error(17), TD::receiver<void>(result));
        return result == TD::result::error
            ;
    }),
    KT::expect_success("TD::run(): void receiver/stopped", []{
        TD::result           result(TD::result::none);
        TD::run(EX::just_stopped(), TD::receiver<void>(result));
        return result == TD::result::stopped
            ;
    }),
    KT::expect_success("TD::sender", []{
        return EX::sender<TD::sender>
            && KT::type<TD::scheduler> == KT::type<decltype(NF::tag_invoke(EX::get_completion_scheduler<EX::set_value_t>, TD::sender()))>
            && KT::type<TD::scheduler> == KT::type<decltype(EX::get_completion_scheduler<EX::set_value_t>(TD::sender()))>
            ;
    }),
    KT::expect_success("TD::scheduler", []{
        return EX::scheduler<TD::scheduler>
            && KT::type<TD::sender> == KT::type<decltype(NF::tag_invoke(EX::schedule, TD::scheduler()))>
            && KT::type<TD::sender> == KT::type<decltype(EX::schedule(TD::scheduler()))>
            ;
    }),
    KT::expect_success("TD::sender_with_then", []{
        ::std::optional<int> value;
        TD::result           result(TD::result::none);
        TD::run(EX::then(TD::sender_with_then(17), [](int){ return 0; }), TD::receiver(value, result));

        return EX::sender<TD::sender_with_then<int>>
            && *value == 17
            && result == TD::result::value
            ;
    }),
    KT::expect_success("TD::sender_with_then piped", []{
        ::std::optional<int> value;
        TD::result           result(TD::result::none);
        TD::run(TD::sender_with_then(17) | EX::then([](int){ return 0; }), TD::receiver(value, result));

        return EX::sender<TD::sender_with_then<int>>
            && *value == 17
            && result == TD::result::value
            ;
    }),
    KT::expect_success("TD::sender_with_scheduler_then", []{
        ::std::optional<int> value;
        TD::result           result(TD::result::none);
        TD::run(EX::then(TD::sender_with_scheduler_then(17, 42), [](int){ return 0; }), TD::receiver(value, result));

        return EX::sender<TD::sender_with_scheduler_then<int, int>>
            && *value == 42
            && result == TD::result::value
            ;
    }),
    KT::expect_success("TD::sender_with_scheduler_then piped", []{
        ::std::optional<int> value;
        TD::result           result(TD::result::none);
        TD::run(TD::sender_with_scheduler_then(17, 42) | EX::then([](int){ return 0; }), TD::receiver(value, result));

        return EX::sender<TD::sender_with_scheduler_then<int, int>>
            && *value == 42
            && result == TD::result::value
            ;
    }),
    KT::expect_success("TD::type_sender is a sender", []{
        return EX::sender<TD::type_sender<EX::completion_signatures<EX::set_value_t()>>>
            ;
    }),
    KT::expect_success("normal then with void function", []{
        double               number(3.125);
        TD::result           result(TD::result::none);
        TD::run(EX::then(EX::just(&number, 3), [](double* n, int v){ *n *= v; }), TD::receiver<void>(result));

        return EX::sender<decltype(EX::then(EX::just(&number, 3), [](double*, int){}))>
            && number == 9.375
            && result == TD::result::value
            ;
    }),
    KT::expect_success("normal then with void function piped", []{
        TD::result           result(TD::result::none);
        double               number(3.125);
        TD::run(EX::just(3, &number) | EX::then([](int v, double* n){ *n *= v; }), TD::receiver<void>(result));

        return EX::sender<decltype(EX::just(&number, 3) | EX::then([](double*, int){ return 0; }))>
            && number == 9.375
            && result == TD::result::value
            ;
    }),
    KT::expect_success("normal then with throwing void function", []{
        TD::result           result(TD::result::none);
        double               number(3.125);
        TD::run(EX::just(3, &number) | EX::then([](int v, double* n){ *n *= v; throw int(); }), TD::receiver<void>(result));

        return EX::sender<decltype(EX::just(&number, 3) | EX::then([](double*, int){ return 0; }))>
            && number == 9.375
            && result == TD::result::error
            ;
    }),
    KT::expect_success("normal then with non-void function", []{
        ::std::optional<int> value;
        double               number(3.125);
        TD::result           result(TD::result::none);
        TD::run(EX::then(EX::just(&number, 3), [](double* n, int v){ *n *= v; return 2 * v; }), TD::receiver(value, result));

        return EX::sender<decltype(EX::then(EX::just(&number, 3), [](double*, int){ return 0; }))>
            && *value == 6
            && number == 9.375
            && result == TD::result::value
            ;
    }),
    KT::expect_success("normal then with non-void function piped", []{
        ::std::optional<int> value;
        TD::result           result(TD::result::none);
        double               number(3.125);
        TD::run(EX::just(3, &number) | EX::then([](int v, double* n){ *n *= v; return 2 * v; }), TD::receiver(value, result));

        return EX::sender<decltype(EX::just(&number, 3) | EX::then([](double*, int){ return 0; }))>
            && *value == 6
            && number == 9.375
            && result == TD::result::value
            ;
    }),
    KT::expect_success("normal then with throwing non-void function", []{
        ::std::optional<int> value;
        TD::result           result(TD::result::none);
        double               number(3.125);
        TD::run(EX::just(3, &number) | EX::then([](int v, double* n){ *n *= v; if (v == 3) throw 0; return 2 * v; }), TD::receiver(value, result));

        return EX::sender<decltype(EX::just(&number, 3) | EX::then([](double*, int v){ if (v ==1) throw 0; return 0; }))>
            && not value
            && number == 9.375
            && result == TD::result::error
            ;
    }),
    KT::expect_success("normal then with non-throwing fun has no errors", []{
        auto sender = TD::error_sender<>() | EX::then([]() noexcept {});
        return EX::sender<decltype(sender)>
            && ::std::same_as<EX::error_types_of_t<decltype(sender), TD::env, TD::variant_t>,
                              EX::completion_signatures<>>
            ;
    }),
    KT::expect_success("then propagates upstream errors", []{
        auto sender = TD::error_sender<::std::exception_ptr, ::std::error_code>()
                    | EX::then([]() noexcept {});
        return EX::sender<decltype(sender)>
            && ::std::same_as<EX::error_types_of_t<decltype(sender), TD::env, TD::variant_t>,
                              EX::completion_signatures<
                                  EX::set_error_t(::std::exception_ptr),
                                  EX::set_error_t(::std::error_code)
                              >>
            ;
    }),
#if 0
    KT::expect_success("normal then with throwing fun may set_error exception_ptr", []{
        auto sender = TD::error_sender<>() | EX::then([]() noexcept(false) {});
            static_assert(::std::same_as<EX::error_types_of_t<decltype(sender), TD::env, TD::variant_t>,
                              EX::completion_signatures<EX::set_error_t(::std::exception_ptr)>>);
        return EX::sender<decltype(sender)>
            && ::std::same_as<EX::error_types_of_t<decltype(sender), TD::env, TD::variant_t>,
                              EX::completion_signatures<EX::set_error_t(::std::exception_ptr)>>
            ;
    }),
#endif
#if 0
    //-dk:TODO fix this test
    KT::expect_success("then() const type", []{
            auto sender = TD::type_sender<EX::completion_signatures<>>()
                | EX::then([](auto x){ return x; })
                ;
            using type = decltype(EX::get_completion_signatures(sender, TD::env{}));
            return KT::type<void> == KT::type<type>
                ;
    }),
#endif
};

static KT::add_tests suite("[exec.then]", ::tests);
