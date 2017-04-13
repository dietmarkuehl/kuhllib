// nstd/execution/work_source.t.cpp                                   -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2016 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/execution/work_source.hpp"
#include "nstd/thread/mutex.hpp"
#include "kuhl/test.hpp"

namespace NE = ::nstd::execution;
namespace NT = ::nstd::thread;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct work_source
        : NE::work_source {
        using NE::work_source::unique_lock;
        bool d_called = false;
        
        ::std::size_t do_size(unique_lock&) const override {
            return 17u;;
        }
        void do_process_one(unique_lock&) override {
            this->d_called = true;
        }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("verify virtual interface", [](KT::context& c)->bool{
            work_source      ws;
            NE::work_source& aws(ws);
            
            NT::mutex                    mutex;
            NE::work_source::unique_lock kerberos(mutex);

            return KT::assert_false(c, "initial state", ws.d_called)
                && KT::assert_equal(c, "size() member", 17u, aws.size(kerberos))
                && (aws.process_one(kerberos)
                    , KT::assert_true(c, "override was called", ws.d_called))
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("work_source", ac, av, ::tests);
}
