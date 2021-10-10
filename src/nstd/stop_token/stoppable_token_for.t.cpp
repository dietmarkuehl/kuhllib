// nstd/stop_token/stoppable_token_for.t.cpp                          -*-C++-*-
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

#include "nstd/stop_token/stoppable_token_for.hpp"
#include <concepts>
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;
namespace ST = ::nstd::stop_token;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct initializer {};
        struct callback {
            callback(initializer) {}
            auto operator()() -> void {}
        };

        struct stop_token {
            stop_token(stop_token const&) noexcept = default;
            stop_token(stop_token&&) noexcept = default;
            auto operator== (stop_token const&) const { return true; }
            auto stop_requested() const noexcept -> bool { return false; }
            auto stop_possible() const noexcept -> bool { return false; }
            template <typename> struct callback_type {};
        };

        struct stop_token_with_nonconst_callback {
            stop_token_with_nonconst_callback(stop_token_with_nonconst_callback const&) noexcept = default;
            stop_token_with_nonconst_callback(stop_token_with_nonconst_callback&&) noexcept = default;
            auto operator== (stop_token_with_nonconst_callback const&) const { return true; }
            auto stop_requested() const noexcept -> bool { return false; }
            auto stop_possible() const noexcept -> bool { return false; }
            template <typename> struct callback_type {
                template<typename I>
                callback_type(stop_token_with_nonconst_callback&, I&&) {}
            };
        };

        struct stop_token_with_const_callback {
            stop_token_with_const_callback(stop_token_with_const_callback const&) noexcept = default;
            stop_token_with_const_callback(stop_token_with_const_callback&&) noexcept = default;
            auto operator== (stop_token_with_const_callback const&) const { return true; }
            auto stop_requested() const noexcept -> bool { return false; }
            auto stop_possible() const noexcept -> bool { return false; }
            template <typename> struct callback_type {
                template<typename I>
                callback_type(stop_token_with_const_callback const&, I&&) {}
            };
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("TD::callback is invokable", []{
            return ::std::invocable<TD::callback>;
        }),
    KT::expect_success("TD::callback can be constructed from initializer", []{
            return ::std::constructible_from<TD::callback, TD::initializer>;
        }),
    KT::expect_success("TD::callback can be constructed from callback", []{
            return ::std::constructible_from<TD::callback, TD::callback>;
        }),
    KT::expect_success("TD::stop_token is a stoppable_token", []{
            return ST::stoppable_token<TD::stop_token>;
        }),
    KT::expect_success("TD::stop_token isn't a stoppable_token_for<callback", []{
            return not ST::stoppable_token_for<TD::stop_token, TD::callback>
                && not ST::stoppable_token_for<TD::stop_token, TD::callback, TD::initializer>
                && not ST::stoppable_token_for<TD::stop_token, TD::callback, int>
                ;
        }),
    KT::expect_success("TD::stop_token_with_nonconst_callback isn't a stoppable_token_for<callback, ...>", []{
            return not ST::stoppable_token_for<TD::stop_token_with_nonconst_callback, TD::callback>
                && not ST::stoppable_token_for<TD::stop_token_with_nonconst_callback, TD::callback, TD::initializer>
                && not ST::stoppable_token_for<TD::stop_token_with_nonconst_callback, TD::callback, int>
                ;
        }),
    KT::expect_success("TD::stop_token_with_const_callback is a stoppable_token_for<callback, ...>", []{
            return ST::stoppable_token_for<TD::stop_token_with_const_callback, TD::callback>
                && ST::stoppable_token_for<TD::stop_token_with_const_callback, TD::callback, TD::initializer>
                && not ST::stoppable_token_for<TD::stop_token_with_const_callback, TD::callback, int>
                ;
        }),
    KT::expect_success("stoppable token with callback", []{
            return true
                ;
        }),
};

static KT::add_tests suite("stoppable_token_for", ::tests);
