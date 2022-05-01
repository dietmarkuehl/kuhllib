// nstd/stop_token/stoppable_token.t.cpp                              -*-C++-*-
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

#include "nstd/stop_token/stoppable_token.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace ST = ::nstd::stop_token_ns;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct boolean {
            explicit operator bool() const { return false; }
        };

        template <bool Move, bool Copy, bool Requested, bool Possible>
        struct stop_token
        {
            template <typename T>
            using callback_type = void(T);

            stop_token(stop_token const&) noexcept(Copy) {}
            stop_token(stop_token &&) noexcept(Move) {}
            auto operator== (stop_token const&) const -> bool = default;
            auto stop_requested() const noexcept(Requested) -> TD::boolean;
            auto stop_possible() const noexcept(Possible) -> TD::boolean;
        };

        struct stop_token_without_equality
        {
            template <typename T>
            using callback_type = void(T);

            stop_token_without_equality(stop_token_without_equality const&) noexcept {}
            stop_token_without_equality(stop_token_without_equality &&) noexcept {}
            auto stop_requested() const noexcept -> TD::boolean;
            auto stop_possible() const noexcept -> TD::boolean;
        };

        struct stop_token_without_callback
        {
            stop_token_without_callback(stop_token_without_callback const&) noexcept {}
            stop_token_without_callback(stop_token_without_callback &&) noexcept {}
            auto operator== (stop_token_without_callback const&) const -> bool = default;
            auto stop_requested() const noexcept -> TD::boolean;
            auto stop_possible() const noexcept -> TD::boolean;
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("class with the expected interface is a stoppable_token", []{
            return ST::stoppable_token<TD::stop_token<true, true, true, true>>
                ;
        }),
    KT::expect_success("class with throwing move ctor isn't a stoppable_token", []{
            return !ST::stoppable_token<TD::stop_token<false, true, true, true>>
                ;
        }),
    KT::expect_success("class with throwing copy ctor isn't a stoppable_token", []{
            return !ST::stoppable_token<TD::stop_token<true, false, true, true>>
                ;
        }),
    KT::expect_success("class with throwing stop_requested ctor isn't a stoppable_token", []{
            return !ST::stoppable_token<TD::stop_token<true, true, false, true>>
                ;
        }),
    KT::expect_success("class with throwing stop_possible ctor isn't a stoppable_token", []{
            return !ST::stoppable_token<TD::stop_token<true, true, true, false>>
                ;
        }),
    KT::expect_success("class without equality isn't stoppable_token", []{
            return !ST::stoppable_token<TD::stop_token_without_equality>
                ;
        }),
    KT::expect_success("class without callback_type isn't stoppable_token", []{
            return !ST::stoppable_token<TD::stop_token_without_callback>
                ;
        }),
};

static KT::add_tests suite("stoppable_token", ::tests);
