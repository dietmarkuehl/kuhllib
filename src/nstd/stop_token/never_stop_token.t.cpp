// nstd/stop_token/never_stop_token.t.cpp                             -*-C++-*-
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

#include "nstd/stop_token/never_stop_token.hpp"
#include "nstd/stop_token/unstoppable_token.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;
namespace ST = ::nstd::stop_token_ns;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("never_stop_token is an unstoppable token", []{
            return ST::unstoppable_token<ST::never_stop_token>;
        }),
    KT::expect_success("never_stop_token::callback_type accepts lambdas", []{
            bool value = true;
            auto fun = [&value]{ value = false; };
            ST::never_stop_token token;
            {
                ST::never_stop_token::callback_type<decltype(fun)> cb(token, fun);
                (void)cb;
            }
            return value;
        }),
};

static KT::add_tests suite("never_stop_token", ::tests);
