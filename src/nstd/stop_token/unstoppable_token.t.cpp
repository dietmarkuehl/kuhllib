// nstd/stop_token/unstoppable_token.t.cpp                            -*-C++-*-
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

#include "nstd/stop_token/unstoppable_token.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;
namespace ST = ::nstd::stop_token;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct stop_token
        {
            stop_token(stop_token const&) noexcept = default;
            stop_token(stop_token&&) noexcept = default;
            auto operator== (stop_token const&) const { return true; }
            auto stop_requested() const noexcept -> bool { return false; }
            constexpr auto stop_possible() const noexcept -> bool { return false; }
            template <typename> struct callback_type {};
        };

        struct non_constexpr_unstop_token
        {
            non_constexpr_unstop_token(non_constexpr_unstop_token const&) noexcept = default;
            non_constexpr_unstop_token(non_constexpr_unstop_token&&) noexcept = default;
            auto operator== (non_constexpr_unstop_token const&) const { return true; }
            auto stop_requested() const noexcept -> bool { return false; }
            static auto stop_possible() noexcept -> bool { return false; }
            template <typename> struct callback_type {};
        };

        struct unstop_token
        {
            unstop_token(unstop_token const&) noexcept = default;
            unstop_token(unstop_token&&) noexcept = default;
            auto operator== (unstop_token const&) const { return true; }
            auto stop_requested() const noexcept -> bool { return false; }
            static constexpr auto stop_possible() noexcept -> bool { return false; }
            template <typename> struct callback_type {};
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("stop_token is a stoppable token ", []{
            return ST::stoppable_token<TD::stop_token>;
        }),
    KT::expect_success("stop_token isn't an unstoppable token", []{
            return not ST::unstoppable_token<TD::stop_token>;
        }),
    KT::expect_success("non_constexpr_unstop_token is a stoppable token ", []{
            return ST::stoppable_token<TD::non_constexpr_unstop_token>;
        }),
    KT::expect_success("non_constexpr_unstop_token isn't an unstoppable token", []{
            return not ST::unstoppable_token<TD::non_constexpr_unstop_token>;
        }),
    KT::expect_success("unstop_token is a stoppable token", []{
            return ST::stoppable_token<TD::unstop_token>;
        }),
    KT::expect_success("unstop_token is a unstoppable token", []{
            return ST::unstoppable_token<TD::unstop_token>;
        }),
};

static KT::add_tests suite("unstoppable_token", ::tests);
