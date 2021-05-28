// nstd/executor/async_result.t.cpp                                   -*-C++-*-
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

#include "nstd/executor/async_result.hpp"
#include <type_traits>
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace NET = ::nstd::net;
namespace TT  = ::std;
namespace KT  = ::kuhl::test;
namespace TD  = ::test_declarations;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    struct token {};
    struct value;
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("async_result types", []{
            return KT::type<TD::token>
                    == KT::type<NET::async_result<TD::token, void(TD::value)>::completion_handler_type>
                && KT::type<void>
                    == KT::type<NET::async_result<TD::token, void(TD::value)>::return_type>
                ;
        }),
    KT::expect_success("async_result structors", []{
            return TT::is_constructible_v<NET::async_result<TD::token, void(TD::value)>, TD::token&>
                && !TT::is_copy_constructible_v<NET::async_result<TD::token, void(TD::value)>>
                && !TT::is_copy_assignable_v<NET::async_result<TD::token, void(TD::value)>>
                ;
        }),
    KT::expect_success("async_result operations", []{
            TD::token                                     token;
            NET::async_result<TD::token, void(TD::value)> result(token);
            return KT::type<void> == KT::type<decltype(result.get())>
                ;
        }),
};

static KT::add_tests suite("async_result", ::tests);
