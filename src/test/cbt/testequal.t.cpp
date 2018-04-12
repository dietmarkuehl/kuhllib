// cbt/testequal.t.cpp                                                -*-C++-*-
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

#include "cbt/testequal.hpp"
#include "cbt/test.hpp"
#include <iostream>

// ----------------------------------------------------------------------------

namespace {
    struct incomparable {};

    struct equal_comparable {
        int v;
        equal_comparable(int v): v(v) {}
    };
    bool operator== (equal_comparable const& e0, equal_comparable const& e1) {
        return e0.v == e1.v;
    }

    struct test_comparable {
        int v;
        test_comparable(int v): v(v) {}
    };
    bool test_equal(test_comparable const& t0, test_comparable const& t1) {
        return t0.v == t1.v;
    }
}

// ----------------------------------------------------------------------------

int main()
{
    incomparable     i0, i1;
    equal_comparable e0(0), e1(1);
    test_comparable  t0(0), t1(1);
    return cbt::main_test({ cbt::TestEqual(std::cout, "incomparable", i0, i1)
                          , cbt::TestEqual(std::cout, "equal_comparable", e0, e0)
                          , !cbt::TestEqual(std::cout, "equal_comparable", e0, e1)
                          , cbt::TestEqual(std::cout, "test_comparable", t0, t0)
                          , !cbt::TestEqual(std::cout, "test_comparable", t0, t1)
                          , true
                          });
}
