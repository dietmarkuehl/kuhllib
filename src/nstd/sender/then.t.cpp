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
#include <optional>
#include "kuhl/test.hpp"

namespace NET = ::nstd::net;
namespace KT  = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("then usage", []{
            struct receiver {
                ::std::optional<bool>* ptr;
                void set_value() && { *this->ptr = true; }
                void set_error(::std::exception_ptr const&) && noexcept {}
                void set_done() && noexcept {}
            };

            ::std::optional<bool> value;
            auto just = NET::just(17);
            auto then = NET::then(just, [&value](auto v){ value = v; });
            auto state = then.connect(receiver{&value});
            state.start();
            return value.value_or(false);
        }),
    KT::expect_success("then pipeline", []{
            struct receiver {
                ::std::optional<bool>* ptr;
                void set_value() && { *this->ptr = true; }
                void set_error(::std::exception_ptr const&) && noexcept {}
                void set_done() && noexcept {}
            };

            ::std::optional<bool> value;
            auto then = NET::just(17)
                      | NET::then([&value](auto v){ value = v; })
                      ;
            auto state = then.connect(receiver{&value});
            state.start();
            return value.value_or(false);
        }),
};

static KT::add_tests suite("then", ::tests);
