// nstd/thread/sync_wait.t.cpp                                        -*-C++-*-
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

#include "nstd/thread/sync_wait.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/get_env.hpp"
#include "nstd/execution/get_scheduler.hpp"
#include "nstd/execution/get_delegatee_scheduler.hpp"
#include "nstd/type_traits/declval.hpp"
#include "kuhl/test.hpp"
#include <optional>
#include <variant>
#include <system_error>
#include <exception>
#include <stdexcept>

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace EX = ::nstd::execution;
namespace NF = ::nstd;
namespace HN = ::nstd::hidden_names::sync_wait;
namespace NT = ::nstd::this_thread;
namespace TT = ::nstd::type_traits;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        template <typename... S>
        struct sender {
            using completion_signatures
                = EX::completion_signatures<S...>;

        };

        struct scheduler;
        struct sender_with_scheduler {
            using completion_signatures = EX::completion_signatures<EX::set_value_t(int)>;

            friend auto tag_invoke(EX::get_completion_scheduler_t<EX::set_value_t>, sender_with_scheduler const&) noexcept -> scheduler;
            friend auto tag_invoke(NT::sync_wait_t, TD::scheduler const&, sender_with_scheduler const&) -> ::std::optional<::std::tuple<int>> {
                return 42;
            }
            friend auto tag_invoke(NT::sync_wait_t, sender_with_scheduler const&) -> ::std::optional<::std::tuple<int>> {
                return 17;
            }
        };

        struct scheduler {
            auto operator== (scheduler const&) const -> bool = default;
            friend auto tag_invoke(EX::schedule_t, scheduler const&) -> TD::sender_with_scheduler {
                return {};
            }
        };
        auto tag_invoke(EX::get_completion_scheduler_t<EX::set_value_t>, sender_with_scheduler const&) noexcept -> scheduler {
            return {};
        }

        struct sender_with_sync_wait {
            using completion_signatures = EX::completion_signatures<EX::set_value_t(int)>;

            friend auto tag_invoke(NT::sync_wait_t, sender_with_sync_wait const&) -> ::std::optional<::std::tuple<int>> {
                return 17;
            }
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("TD::sender", []{
            return EX::sender<TD::sender<>>
                && EX::sender<TD::sender<EX::set_value_t()>>
                && EX::sender<TD::sender<EX::set_value_t(int, bool&)>>
                && EX::sender<TD::sender<EX::set_value_t(int, bool&)>, EX::set_value_t(float)>
                && EX::sender<TD::sender<EX::set_error_t(int), EX::set_stopped_t()>>
                ;
        }),
    KT::expect_success("TD::scheduler", []{
            return EX::scheduler<TD::scheduler>
                && KT::type<TD::sender_with_scheduler> == KT::type<decltype(EX::schedule(TD::scheduler()))>
                ;
        }),
    KT::expect_success("TD::sender_with_scheduler", []{
            return EX::sender<TD::sender_with_scheduler>
                && KT::type<TD::scheduler> == KT::type<decltype(EX::get_completion_scheduler<EX::set_value_t>(TD::sender_with_scheduler()))>
                && KT::type<::std::optional<::std::tuple<int>>> == KT::type<decltype(NF::tag_invoke(NT::sync_wait, TD::scheduler(), TD::sender_with_scheduler()))>
                ;
        }),
    KT::expect_success("TD::sender_with_sender", []{
            return EX::sender<TD::sender_with_sync_wait>
                && KT::type<::std::optional<::std::tuple<int>>> == KT::type<decltype(NF::tag_invoke(NT::sync_wait, TD::sender_with_sync_wait()))>
                ;
        }),
    KT::expect_success("sync_wait receiver", []{
        return EX::receiver<HN::receiver>
            && EX::scheduler<decltype(EX::get_scheduler(EX::get_env(TT::declval<HN::receiver const&>())))>
            && EX::scheduler<decltype(EX::get_delegatee_scheduler(EX::get_env(TT::declval<HN::receiver const&>())))>
            ;
        }),
    KT::expect_success("sync_wait type", []{
        return KT::type<::std::optional<::std::tuple<>>>
                == KT::type<HN::return_type<TD::sender<EX::set_value_t()>>>
            && KT::type<::std::optional<::std::tuple<int, bool, float>>>
                == KT::type<HN::return_type<TD::sender<EX::set_value_t(int&&, bool&, float const&)>>>
            && KT::type<::std::optional<::std::tuple<int, bool, float>>>
                == KT::type<HN::return_type<TD::sender<EX::set_value_t(int&&, bool&, float const&), EX::set_error_t(int)>>>
            && KT::type<::std::optional<::std::tuple<int, bool, float>>>
                == KT::type<HN::return_type<TD::sender<EX::set_value_t(int&&, bool&, float const&), EX::set_error_t(int), EX::set_stopped_t()>>>
            && KT::type<::std::optional<::std::monostate>>
                == KT::type<HN::return_type<TD::sender<EX::set_error_t(int), EX::set_stopped_t()>>>
            ;
        }),
    KT::expect_success("special sync_wait for scheduler/sender", []{
            auto[value] =  *NT::sync_wait(TD::sender_with_scheduler());
            return KT::type<::std::optional<::std::tuple<int>>> == KT::type<decltype(NT::sync_wait(TD::sender_with_scheduler()))>
                && value == 42
                ;
        }),
    KT::expect_success("special sync_wait for sender", []{
            auto[value] =  *NT::sync_wait(TD::sender_with_sync_wait());
            return KT::type<::std::optional<::std::tuple<int>>> == KT::type<decltype(NT::sync_wait(TD::sender_with_sync_wait()))>
                && value == 17
                ;
        }),
    KT::expect_success("sync_wait usage", []{
            auto res = NT::sync_wait(EX::just(64));
            return KT::type<decltype(res)> == KT::type<::std::optional<::std::tuple<int>>>
                && res
                && ::std::get<0>(*res) == 64
                ;
        }),
#if 0
    KT::expect_success("sync_wait error_code", []{
        try {
            NT::sync_wait(EX::just_error(::std::error_code(0, ::std::generic_category())));
            return false;
        }
        catch (::std::error_code const&) {
            //-dk:TODO error_code should become a system_error
            return true;
        }
        catch (::std::system_error const&) {
            return true;
        }
        catch (...) {
            return false;
        }
    }),
    KT::expect_success("sync_wait exception object", []{
        try {
            NT::sync_wait(EX::just_error(::std::runtime_error("error")));
            return false;
        }
        catch (::std::runtime_error const&) {
            return true;
        }
        catch (...) {
            return false;
        }
    }),
    KT::expect_success("sync_wait exception_ptr", []{
        try {
            NT::sync_wait(EX::just_error(::std::make_exception_ptr(::std::logic_error("error"))));
            return false;
        }
        catch (::std::logic_error const&) {
            return true;
        }
        catch (...) {
            return false;
        }
    }),
    KT::expect_success("sync_wait cancel", []{
        try {
            NT::sync_wait(EX::just_done());
            return true;
        }
        catch (...) {
            return false;
        }
    }),
    KT::expect_success("sync_wait with void result", []{
        NT::sync_wait(EX::just());
        return true;
    }),
#endif
};

static KT::add_tests suite("[exec.sync_wait]", ::tests);
