// nstd/sender/just.t.cpp                                             -*-C++-*-
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

#include "nstd/sender/just.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_done.hpp"
#include <exception>
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
        ::std::optional<int>* ptr;
        //void set_value(int v) && { *this->ptr = v; }
        //void set_error(::std::exception_ptr const&) && {}

        friend void tag_invoke(EX::set_value_t, receiver&& r, int v) { *r.ptr = v; }
        friend void tag_invoke(EX::set_error_t, receiver&&, ::std::exception_ptr const&) {}
    };

    struct string_receiver {
        ::std::optional<::std::string>* ptr;
        //void set_value(char const*) && {}
        //void set_value(::std::string_view v) && { *this->ptr = v; }
        //void set_error(::std::exception_ptr const&) && {}

        friend void tag_invoke(EX::set_value_t, string_receiver&&, char const*) {}
        friend void tag_invoke(EX::set_value_t, string_receiver&& r, ::std::string_view v) { *r.ptr = v; }
        friend void tag_invoke(EX::set_error_t, string_receiver&&, ::std::exception_ptr const&) {}
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("just usage", []{
            ::std::optional<int> value;
            auto state = NET::just(17).connect(TD::receiver{&value});
            state.start();
            return value.value_or(0) == 17
                ;
        }),
    KT::expect_success("just specialization", []{
            ::std::optional<::std::string> value;
            auto state = NET::just("foo").connect(TD::string_receiver{&value});
            state.start();
            return value.value_or("") == "foo"
                ;
        }),
};

static KT::add_tests suite("just", ::tests);
