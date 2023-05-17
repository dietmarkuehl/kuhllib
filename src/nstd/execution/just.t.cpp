// nstd/execution/just.t.cpp                                          -*-C++-*-
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

#include "nstd/execution/just.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/receiver_of.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/hidden_names/valid_completion_for.hpp"
#include "nstd/utility/forward.hpp"
#include <exception>
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = test_declarations;
namespace EX = ::nstd::execution;
namespace HN = ::nstd::hidden_names;
namespace UT = ::nstd::utility;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct env {};
        template <typename...> struct var {};
        template <typename...> struct tup {};

        struct non_movable {
            non_movable(non_movable&&) = delete;
            non_movable(non_movable const&) = delete;
        };

        struct ctor_throws {
            ctor_throws() = default;
            ctor_throws(ctor_throws const&) {}
        };

        struct move_only
        {
            int value;
            explicit move_only(int value): value(value) {}
            move_only(move_only&&) = default;
            move_only(move_only&) = delete;
            move_only(move_only const&) = delete;
            auto operator== (move_only const&) const -> bool = default;
        };

        enum class result { none, value, error, done };
        struct error {};
        template <typename... T>
        struct receiver
        {
            TD::result* const  result;
            ::std::tuple<T...> values;
            receiver(TD::result* result, T&&... a): result(result), values(UT::forward<T>(a)...) {}
            friend auto tag_invoke(EX::set_value_t, receiver&& r, T... v) noexcept {
                if (r.values == ::std::tuple<T...>(UT::forward<T>(v)...)) {
                    *r.result = TD::result::value;
                }
            }
            friend auto tag_invoke(EX::set_error_t, receiver&& r, ::std::exception_ptr ex) noexcept {
                try { ::std::rethrow_exception(ex); }
                catch (TD::error const&) { *r.result = TD::result::error; }
                catch (...) {}
            }
            friend auto tag_invoke(EX::set_stopped_t, receiver&& r) noexcept {
                *r.result = TD::result::done;
            }
	    friend auto tag_invoke(EX::get_env_t,receiver const&) -> TD::env { return {}; }
        };

        template <typename T>
        concept has_just
            = requires(T v){ EX::just(v); }
            ;

        template <typename S, typename R>
        concept has_connect
            = requires(S&& s, R&& r){ EX::connect(UT::forward<S>(s), UT::forward<R>(r)); }
            ;

        template <typename CPO, typename S, typename R>
        concept has_tag_invoke
            = requires(S&& s, R&& r){ tag_invoke(CPO(), UT::forward<S>(s), UT::forward<R>(r)); }
            ;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("TD::receiver is a receiver", []{
            return EX::receiver<TD::receiver<bool, int>>;
        }),
    KT::expect_success("set_value_t(bool, int) is a valid_completion_for TD::receiver", []{
            return HN::valid_completion_for<EX::set_value_t(bool, int), TD::receiver<bool, int>>;
        }),
    KT::expect_success("TD::receiver is a receiver_of", []{
            return EX::receiver_of<TD::receiver<bool, int>, EX::completion_signatures<EX::set_value_t(bool, int)>>;
        }),
    KT::expect_success("just(non_movable) doesn't exist", []{
            return not TD::has_just<TD::non_movable>
                && not TD::has_just<TD::non_movable&>
                && not TD::has_just<TD::non_movable&&>
                && not TD::has_just<TD::non_movable const>
                ;
        }),
    KT::expect_success("just(...)'s noexcept depends on its args", []{
            TD::ctor_throws ct;
            TD::ctor_throws cct;
            return noexcept(EX::just(0))
                && not noexcept(EX::just(TD::ctor_throws()))
                && not noexcept(EX::just(cct))
                && not noexcept(EX::just(ct))
                ;
        }),
    KT::expect_success("r-value just(...) has tag_invoke(connect) for receivers", []{
            return TD::has_connect<decltype(EX::just(TD::move_only(0))), TD::receiver<TD::move_only>>
                && TD::has_tag_invoke<EX::connect_t, decltype(EX::just(TD::move_only(0))), TD::receiver<TD::move_only>>
                ;
        }),
    KT::expect_success("l-value just(...) has tag_invoke(connect) for receivers", []{
            auto just = EX::just(0);
            return KT::use(just)
                && TD::has_connect<decltype((just)), TD::receiver<int>>
                && TD::has_tag_invoke<EX::connect_t, decltype((just)), TD::receiver<int>>
                ;
        }),
    KT::expect_success("r-value just(...) does not have tag_invoke(connect) for non-receivers", []{
            return not TD::has_connect<decltype(EX::just(TD::move_only(0))), int>
                && not TD::has_connect<decltype(EX::just(TD::move_only(0))), TD::receiver<int>>
                && not TD::has_tag_invoke<EX::connect_t, decltype(EX::just(TD::move_only(0))), int>
                && not TD::has_tag_invoke<EX::connect_t, decltype(EX::just(TD::move_only(0))), TD::receiver<int>>
                ;
        }),
    KT::expect_success("l-value just(...) does not have tag_invoke(connect) for non-receivers", []{
            auto just = EX::just(0);
            return KT::use(just)
                && not TD::has_connect<decltype((just)), int>
                && not TD::has_connect<decltype((just)), TD::receiver<TD::move_only>>
                && not TD::has_tag_invoke<EX::connect_t, decltype(EX::just((just))), int>
                && not TD::has_tag_invoke<EX::connect_t, decltype(EX::just((just))), TD::receiver<TD::move_only>>
                ;
        }),
    KT::expect_success("just(true, 0) yields a sender", []{
            return EX::sender<decltype(EX::just(true, 0))>;
        }),
    KT::expect_success("just(true, 0) yields a sender", []{
            return EX::sender<decltype(EX::just(true, 0))>;
        }),
    KT::expect_success("just(true, 0)::completion_signatures is C<set_value_t(bool, int)>", []{
            return KT::type<decltype(EX::just(true, 0))::completion_signatures>
                == KT::type<EX::completion_signatures<
                    EX::set_value_t(bool, int),
                    EX::set_error_t(::std::exception_ptr)>>
		;
        }),
    KT::expect_success("just const j; connect(j, receiver) works", []{
            TD::result result{};
            auto const just = EX::just(true, 0);
            (void)just;
            (void)result;
            return false;
#if 0
            auto state = EX::connect(just, TD::receiver<bool, int>(&result, true, 0));
            EX::start(state);
            return EX::operation_state<decltype(state)>
                && result == TD::result::value
                ;
#endif
        }),
    KT::expect_success("connect(just(), receiver) works", []{
            TD::result result{};
            (void)result;
            return false;
            #if 0
            //-dk:TODO
            auto state = EX::connect(EX::just(TD::move_only{42}, 0),
                                     TD::receiver<TD::move_only, int>(&result, TD::move_only{42}, 0));
            EX::start(state);
            return EX::operation_state<decltype(state)>
                && result == TD::result::value
                ;
                #endif
        }),
};

static KT::add_tests suite("just", ::tests);
