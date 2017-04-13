// nstd/thread/thread_pool.t.cpp                                      -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2017 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/execution/thread_pool.hpp"
#include "kuhl/test.hpp"

namespace KT = ::kuhl::test;
namespace NE = ::nstd::execution;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("breathing test", [](KT::context& c)->bool{
            NE::thread_pool          pool;
            NE::thread_pool_executor executor(pool);

            pool.start(1u);
            executor.add([](){});
            executor.add([](NE::thread_pool_executor&){});
            executor.add_front([](NE::thread_pool_executor&){});
            executor.process();
            return true;
        }),
    KT::expect_success("breathing test", [](KT::context& c)->bool{
            NE::thread_pool_executor executor;

            executor.add([](){});
            executor.add([](NE::thread_pool_executor&){});
            executor.add_front([](NE::thread_pool_executor&){});
            executor.process();
            return true;
        }),
    //-dk:TODO more thorough tests...
};

int main(int ac, char* av[])
{
    return KT::run_tests("TODO", ac, av, ::tests);
}
