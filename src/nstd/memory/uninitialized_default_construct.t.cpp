// nstd/memory/uninitialized_default_construct.t.cpp                  -*-C++-*-
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

#include "nstd/memory/uninitialized_default_construct.hpp"
#include "nstd/execution/execution.hpp"
#include "nstd/atomic/atomic.hpp"
#include "kuhl/test.hpp"

namespace KT = ::kuhl::test;
namespace NM = ::nstd::memory;
namespace NA = ::nstd::atomic;

// ----------------------------------------------------------------------------

namespace {
    struct pod {
        int value;
    };
    struct counting {
        static NA::atomic<int> count;
        int value;
        counting(): value(17) { ++count; }
    };
    NA::atomic<int> counting::count;
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("plain: initializing an empty sequence does nothing",
                       [](KT::context& c)->bool {
        counting array[1] = {};
        array[0].value = 18;
        NM::uninitialized_default_construct(array + 0, array + 0);
        return KT::assert_equal(c, "count of constructions", counting::count, 1)
            && KT::assert_equal(c, "value is untouched", array[0].value, 18)
            ;
        }),
    KT::expect_success("plain: a sequence of PODs isn't changed them",
                       [](KT::context& c)->bool {
        pod array[3] = {};
        for (int i(0); i != 3; ++i) {
            array[i].value = i;
        }

        return KT::assert_equal(c, "array[0].value == 0", array[0].value, 0)
            && KT::assert_equal(c, "array[1].value == 1", array[1].value, 1)
            && KT::assert_equal(c, "array[2].value == 2", array[2].value, 2)
            && (NM::uninitialized_default_construct(array + 0, array + 3), true)
            && KT::assert_equal(c, "array[0].value == 0", array[0].value, 0)
            && KT::assert_equal(c, "array[1].value == 1", array[1].value, 1)
            && KT::assert_equal(c, "array[2].value == 2", array[2].value, 2)
            ;
        }),
    KT::expect_success("plain: a sequence of non-PODs is changed",
                       [](KT::context& c)->bool {
        counting::count = 0;
        counting array[3] = {};
        for (int i(0); i != 3; ++i) {
            array[i].value = i;
        }
        return KT::assert_equal(c, "counting::count == 3", counting::count, 3)
            && KT::assert_equal(c, "array[0].value == 0", array[0].value, 0)
            && KT::assert_equal(c, "array[1].value == 1", array[1].value, 1)
            && KT::assert_equal(c, "array[2].value == 2", array[2].value, 2)
            && (NM::uninitialized_default_construct(array + 0, array + 3), true)
            && KT::assert_equal(c, "counting::count == 6", counting::count, 6)
            && KT::assert_equal(c, "array[0].value == 17", array[0].value, 17)
            && KT::assert_equal(c, "array[1].value == 17", array[1].value, 17)
            && KT::assert_equal(c, "array[2].value == 17", array[2].value, 17)
            ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("uninitialized_default_construct", ac, av, ::tests);
}
