// nstd/algorithm/max.t.cpp                                           -*-C++-*-
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

#include "nstd/algorithm/max.hpp"
#include "kuhl/test.hpp"
#include <iostream>

namespace NA = ::nstd::algorithm;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("max() using <", [](KT::context& c)->bool{
            int x(1), y(1), z(2);

            return KT::assert_equal(c, "return unique max 1st", &z, &NA::max(z, x))
                && KT::assert_equal(c, "return unique max 2nd", &z, &NA::max(x, z))
                && KT::assert_equal(c, "return stable max ", &y, &NA::max(x, y))
                ;
        }),
    KT::expect_success("max() using comparator", [](KT::context& c)->bool{
            int x(1), y(1), z(2);

            return KT::assert_equal(c, "return unique max 1st",
                                    &z, &NA::max(z, x, [](auto a, auto b){ return a < b; }))
                && KT::assert_equal(c, "return unique max 2nd",
                                    &z, &NA::max(x, z, [](auto a, auto b){ return a < b; }))
                && KT::assert_equal(c, "return stable max ",
                                    &y, &NA::max(x, y, [](auto a, auto b){ return a < b; }))
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("max()", ac, av, ::tests);
}
