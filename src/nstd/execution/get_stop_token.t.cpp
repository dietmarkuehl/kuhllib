// nstd/execution/get_stop_token.t.cpp                                -*-C++-*-
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

#include "nstd/execution/get_stop_token.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/stop_token/never_stop_token.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace EX = ::nstd::execution;
namespace ST = ::nstd::stop_token_ns;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        template <typename Env>
        concept has_get_stop_token
            = requires(Env const& e) { EX::get_stop_token(e); }
            ;

        struct non_stop_token {};

        struct stop_token
        {
            template <typename T>
            using callback_type = void(T);

            stop_token() {}
            stop_token(stop_token const&) noexcept {}
            stop_token(stop_token &&) noexcept {}
            auto operator== (stop_token const&) const -> bool = default;
            auto stop_requested() const noexcept -> bool;
            auto stop_possible() const noexcept -> bool;
        };

        template <typename StopToken, bool Noexcept>
        struct env {
            bool* const value;
            friend auto tag_invoke(EX::get_stop_token_t, env const& r) noexcept(Noexcept) {
                *r.value = true;
                return StopToken();
            }
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("get_stop_token breathing", []{
        return KT::assert_type_exists<EX::get_stop_token_t>
            && KT::type<EX::get_stop_token_t const> == KT::type<decltype(EX::get_stop_token)>
            ;
    }),
    KT::expect_success("env returning a stop_token has get_stop_token", []{
            bool                          value(false);
            TD::env<TD::stop_token, true> e{&value};
            EX::get_stop_token(e);
            return TD::has_get_stop_token<TD::env<TD::stop_token, true>>
                && KT::type<decltype(EX::get_stop_token(e))> == KT::type<TD::stop_token>
                && value
                ;
        }),
    KT::expect_success("throwing get_stop_token isn't considered ", []{
            return TD::has_get_stop_token<TD::env<TD::stop_token, false>>
                && KT::type<decltype(EX::get_stop_token(TD::env<TD::stop_token, false>{nullptr}))>
                    == KT::type<ST::never_stop_token>
                ;
        }),
    KT::expect_success("return a non-stop_token from get_stop_token isn't considered ", []{
            return TD::has_get_stop_token<TD::env<TD::non_stop_token, true>>
                && KT::type<decltype(EX::get_stop_token(TD::env<TD::non_stop_token, true>{nullptr}))>
                    == KT::type<ST::never_stop_token>
                ;
        }),
};

static KT::add_tests suite("get_stop_token", ::tests);