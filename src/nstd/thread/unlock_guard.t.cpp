// nstd/thread/unlock_guard.t.cpp                                     -*-C++-*-
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

#include "nstd/thread/unlock_guard.hpp"
#include "kuhl/test.hpp"
#include <stdexcept>

namespace KT = ::kuhl::test;
namespace NT = ::nstd::thread;

// ----------------------------------------------------------------------------

namespace {
    struct lockable {
        int d_count = 0;
        bool locked() const { return this->d_count; }
        void lock()   {
            if (1 != ++this->d_count) {
                throw std::runtime_error("locked too often");
            }
        }
        void unlock() {
            if (--this->d_count != 0) {
                throw std::runtime_error("unlocked too often");
            }
        }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("test facility", [](KT::context& c)->bool{
            lockable l;
            return KT::assert_false(c, "lockable initial state", l.locked())
                && (l.lock(), true)
                && KT::assert_true(c, "lockable after being locked", l.locked())
                && (l.unlock(), true)
                && KT::assert_false(c, "lockable after being unlocked", l.locked())
                ;
        }),
    KT::expect_success("automatic unlock/lock", [](KT::context& c)->bool{
            lockable l;
            l.lock();
            {
                NT::unlock_guard<lockable> kerberos(l);
                if (!KT::assert_false(c, "unlocked after construction of unlock_guard",
                                      l.locked())) {
                    return false;
                }
            }
            return KT::assert_true(c, "locked after destruction of unlock_guard",
                                   l.locked());
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("nstd::thread::unlock_guard", ac, av, ::tests);
}
