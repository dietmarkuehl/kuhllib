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
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = test_declarations;
namespace EX = ::nstd::execution;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        template <typename Receiver>
        concept has_get_stop_token
            = requires(Receiver const& r) { EX::get_stop_token(r); }
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

        template <typename StopToken, bool Noexcept, bool IsReceiver = true>
        struct receiver {
            bool* const value;
            friend auto tag_invoke(EX::set_done_t, receiver&&) noexcept(IsReceiver) {}
            friend auto tag_invoke(EX::set_error_t, receiver&&, std::exception_ptr) noexcept {}
            friend auto tag_invoke(EX::get_stop_token_t, receiver const& r) noexcept(Noexcept) {
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
    KT::expect_success("test declarations behave as expected", []{
            return EX::receiver<TD::receiver<TD::stop_token, true>>
                && EX::receiver<TD::receiver<TD::stop_token, false>>
                && !EX::receiver<TD::receiver<TD::stop_token, true, false>>
                ;
        }),
    KT::expect_success("receiver returning a stop_token has get_stop_token", []{
            bool                              value(false);
            TD::receiver<TD::stop_token, true> r{&value};
            EX::get_stop_token(r);
            return TD::has_get_stop_token<TD::receiver<TD::stop_token, true>>
                && KT::type<decltype(EX::get_stop_token(r))> == KT::type<TD::stop_token>
                && value
                ;
        }),
    KT::expect_success("non-receiver doesn't have get_stop_token", []{
            return !TD::has_get_stop_token<TD::receiver<TD::stop_token, true, false>>
                ;
        }),
    KT::expect_success("throwing get_stop_token isn't allowed ", []{
            return !TD::has_get_stop_token<TD::receiver<TD::stop_token, false>>
                ;
        }),
    KT::expect_success("return a non-stop_token from get_stop_token isn't allowed ", []{
            return !TD::has_get_stop_token<TD::receiver<TD::non_stop_token, true>>
                ;
        }),
};

static KT::add_tests suite("get_stop_token", ::tests);