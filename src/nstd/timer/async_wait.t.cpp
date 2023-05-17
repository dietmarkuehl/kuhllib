// nstd/timer/async_wait.t.cpp                                        -*-C++-*-
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

#include "nstd/timer/async_wait.hpp"
#include "nstd/execution/run.hpp"
#include "nstd/execution/then.hpp"
#include "kuhl/test.hpp"
#include <chrono>

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;
namespace NN = ::nstd::net;
namespace EX = ::nstd::execution;
namespace HN = ::nstd::hidden_names;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("timer", []{
            try {
                using namespace std::chrono_literals;
                using clock = ::std::chrono::system_clock;
                NN::io_context   context;
                clock::duration  d(13ms);
            
                auto before = clock::now();
                auto count = 0;
                NN::system_timer timer(context, d);
               EX::run(context, NN::async_wait(timer) | EX::then([before, &count](auto&&...){
                    auto dur = clock::now() - before;
                    count = int(::std::chrono::duration_cast<::std::chrono::milliseconds>(dur).count());
                }));
                return KT::use(before)
                    && 13 <= count;
            }
            catch (std::exception const& ex) {
                return false;
            }
        }),
            
};

static KT::add_tests suite("async_wait", ::tests);
