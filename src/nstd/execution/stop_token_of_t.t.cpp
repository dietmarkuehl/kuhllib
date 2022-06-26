// nstd/execution/stop_token_of_t.t.cpp                               -*-C++-*-
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

#include "nstd/execution/stop_token_of_t.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/stop_token/stoppable_token.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;
namespace ST = ::nstd::stop_token_ns;
namespace EX = ::nstd::execution;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct stop_token {
            auto operator== (stop_token const&) const -> bool = default;
            auto stop_possible() const noexcept -> bool { return true; }
            auto stop_requested() const noexcept -> bool { return true; }
            template <typename>
            struct callback_type 
            {
                template <typename F>
                callback_type(stop_token const&, F&&) {}
            };
        };
        static_assert(ST::stoppable_token<TD::stop_token>);

        struct env {};

        struct receiver {
            friend auto tag_invoke(EX::get_env_t, receiver const&) noexcept -> TD::env { return {}; }
            friend auto tag_invoke(EX::get_stop_token_t, receiver const&) noexcept -> TD::stop_token { return {}; }
            friend auto tag_invoke(EX::set_error_t, receiver&&, auto&&) noexcept -> void {}
        };
        static_assert(EX::receiver<TD::receiver>);
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("type", []{
            return KT::type<EX::stop_token_of_t<TD::receiver>> == KT::type<TD::stop_token>
                ;
        }),
};

static KT::add_tests suite("stop_token_of_t", ::tests);
