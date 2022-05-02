// nstd/stop_token/in_place_stop_token.t.cpp                          -*-C++-*-
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

#include "nstd/stop_token/in_place_stop_token.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;
namespace ST = ::nstd::stop_token_ns;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("create state", []{
            ST::in_place_stop_source state;
            return not state.stop_requested()
                && KT::type<decltype(state.token())> == KT::type<ST::in_place_stop_token>
                ;
        }),
    KT::expect_success("token for unstopped state", []{
            ST::in_place_stop_source state;
            ST::in_place_stop_token token = state.token();
            return token == state.token()
                && token.stop_possible()
                && not token.stop_requested()
                ;
        }),
    KT::expect_success("token for stopped state", []{
            ST::in_place_stop_source state;
            ST::in_place_stop_token token = state.token();
            state.stop();
            return token == state.token()
                && token.stop_possible()
                && token.stop_requested()
                ;
        }),
    KT::expect_success("callbacks get called", []{
            ST::in_place_stop_source state;
            ST::in_place_stop_token token = state.token();
            bool value0{false};
            bool value1{false};
            bool value2{false};
            ST::in_place_stop_token::callback_type cb0(token, [&value0]{ value0 = true; });
            {
                ST::in_place_stop_token::callback_type cb1(token, [&value1]{ value1 = true; });
            }
            ST::in_place_stop_token::callback_type cb2(token, [&value2]{ value2 = true; });

            state.stop();
            return value0 == true
                && value1 == false
                && value2 == true
                ;
        }),
    KT::expect_success("callbacks get called (middle removed)", []{
            ST::in_place_stop_source state;
            ST::in_place_stop_token token = state.token();
            bool value0{false};
            bool value1{false};
            bool value2{false};
            ST::in_place_stop_token::callback_type cb0(token, [&value0]{ value0 = true; });
            auto cb1 = new ST::in_place_stop_token::callback_type(token, [&value1]{ value1 = true; });
            ST::in_place_stop_token::callback_type cb2(token, [&value2]{ value2 = true; });
            delete cb1;

            state.stop();
            return value0 == true
                && value1 == false
                && value2 == true
                ;
        }),
};

static KT::add_tests suite("in_place_stop_token", ::tests);
