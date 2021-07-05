// nstd/sender/then.t.cpp                                             -*-C++-*-
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

#include "nstd/sender/then.hpp"
#include "nstd/sender/just.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_done.hpp"
#include <optional>
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD  = test_declarations;
namespace EX  = ::nstd::execution;
namespace NET = ::nstd::net;
namespace KT  = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    struct receiver {
        ::std::optional<bool>* ptr;
        friend auto tag_invoke(EX::set_value_t, receiver&& r) -> void { *r.ptr = true; }
        friend auto tag_invoke(EX::set_error_t, receiver&&, ::std::exception_ptr const&) noexcept -> void {}
        friend auto tag_invoke(EX::set_done_t, receiver&&) noexcept -> void {}
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("then usage", []{
            ::std::optional<bool> value;
            auto then = NET::then(NET::just(17), [&value](auto v){ value = v; });
            auto state = then.connect(TD::receiver{&value});
            state.start();
            return value.value_or(false);
        }),
    KT::expect_success("then pipeline", []{
            ::std::optional<bool> value;
            auto then = NET::just(17)
                      | NET::then([&value](auto v){ value = v; })
                      ;
            auto state = then.connect(TD::receiver{&value});
            state.start();
            return value.value_or(false);
        }),
};

static KT::add_tests suite("then", ::tests);
