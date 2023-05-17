// nstd/execution/on.t.cpp                                            -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2022 Dietmar Kuehl http://www.dietmar-kuehl.de
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

#include "nstd/execution/on.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/schedule.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/thread/sync_wait.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"
#include <exception>
#include <system_error>

namespace test_declaration {}
namespace EX = ::nstd::execution;
namespace TY = ::nstd::type_traits;
namespace TR = ::nstd::this_thread;
namespace TT = ::nstd::type_traits;
namespace UT = ::nstd::utility;
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
        template <typename... T>
        using variant_t = ::nstd::execution::completion_signatures<::nstd::execution::set_error_t(T)...>;

        struct env {};
        struct type {};

        template <EX::receiver Receiver>
        struct state {
            TY::remove_cvref_t<Receiver> receiver;
            friend auto tag_invoke(EX::start_t, state& self) noexcept {
                EX::set_value(UT::move(self.receiver));
            }
        };

        template <int>
        struct sender {
            using completion_signatures = EX::completion_signatures<EX::set_value_t()>;

            template <EX::receiver Receiver>
            friend auto tag_invoke(EX::connect_t, sender, Receiver&& receiver) {
                return TD::state<Receiver>{ UT::forward<Receiver>(receiver) };
            }
        };

        struct scheduler_sender {
            using completion_signatures = EX::completion_signatures<EX::set_value_t()>;

            template <EX::receiver Receiver>
            friend auto tag_invoke(EX::connect_t, scheduler_sender, Receiver&& receiver) {
                return TD::state<Receiver>{ UT::forward<Receiver>(receiver) };
            }
        };
        struct scheduler {
            auto operator== (scheduler const&) const -> bool = default;
            friend auto tag_invoke(EX::schedule_t, scheduler) noexcept {
                return TD::scheduler_sender{};
            }

            friend auto tag_invoke(EX::on_t, scheduler, TD::sender<0> sender) {
                return sender;
            }
        };
        auto tag_invoke(EX::get_completion_scheduler_t<EX::set_value_t>, TD::scheduler_sender) noexcept {
            return TD::scheduler{};
        }

        template <typename... E>
        struct error_sender {
            using completion_signatures = EX::completion_signatures<
                EX::set_value_t(),
                EX::set_error_t(E)...
                >;
            
            template <EX::receiver R>
            struct state {
                TY::remove_cvref_t<R> receiver;
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
    KT::expect_success("dummy", []{
        return false;
    }),
#if 0
    KT::expect_success("TD::sender<...> is a sender", []{
        TR::sync_wait(TD::sender<0>{});
        return EX::sender<TD::sender<0>>;
    }),
    KT::expect_success("TD::scheduler is a scheduler", []{
        EX::get_completion_scheduler<EX::set_value_t>(TD::scheduler_sender{});
        TR::sync_wait(EX::schedule(TD::scheduler{}));
        return EX::scheduler<TD::scheduler>;
    }),
    KT::expect_success("on(TD::scheduler, TD::sender<0>) is customized", []{
        auto sender = EX::on(TD::scheduler{}, TD::sender<0>{});
        return KT::type<TD::sender<0>> == KT::type<decltype(sender)>;
    }),
    KT::expect_success("on(TD::scheduler, TD::sender<1>) is not customized", []{
        auto sender = EX::on(TD::scheduler{}, TD::sender<1>{});
        TR::sync_wait(sender);
        return EX::sender<decltype(sender)>
            && not std::same_as<TD::sender<1>, decltype(sender)>
            ;
    }),
    KT::expect_success("on(TD::scheduler, upstream) propagates upstream errors", []{
        auto sender = EX::on(TD::scheduler{}, TD::error_sender<::std::exception_ptr, ::std::error_code>());
        return EX::sender<decltype(sender)>
            && ::std::same_as<EX::error_types_of_t<decltype(sender), TD::env, TD::variant_t>,
                              EX::completion_signatures<
                                  EX::set_error_t(::std::exception_ptr),
                                  EX::set_error_t(::std::error_code)
                              >>
            ;
    }),
#endif
};

static KT::add_tests suite("nstd/execution/on", ::tests);
