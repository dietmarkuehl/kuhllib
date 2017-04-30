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
#include "nstd/execution/work_source.hpp"
#include "nstd/thread/condition_variable.hpp"
#include "kuhl/test.hpp"

namespace NE = ::nstd::execution;
namespace NT = ::nstd::thread;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct work_source
        : NE::work_source {
        using NE::work_source::unique_lock;

        NT::condition_variable d_done;
        bool                   d_called = false;

        void do_process_one(unique_lock&) override {
            this->d_called = true;
            this->d_done.notify_one();
        }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("breathing test", [](KT::context& c)->bool{
            NE::thread_pool pool(1u);
            pool.start(2u);
            
            work_source ws;
            auto kerberos(pool.get_lock());
            pool.add_work(&ws, kerberos);

            ws.d_done.wait(kerberos, [&](){ return ws.d_called; });

            return KT::assert_true(c, "the register work got called", ws.d_called)
                ;
        }),
    //-dk:TODO more thorough tests...
};

int main(int ac, char* av[])
{
    return KT::run_tests("thread_pool", ac, av, ::tests);
}
