// nstd/execution/tbb_policy.t.cpp                                    -*-C++-*-
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

#include "nstd/execution/tbb_policy.hpp"
#include "nstd/execution/is_execution_policy.hpp"
#include "kuhl/test.hpp"

namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("tbb_policy is an execution policy",
                       [](KT::context& c)->bool{
        return KT::assert_constexpr_true<nstd::is_execution_policy<nstd::execution::tbb_policy>::value>(
                  c, "is_execution_policy<tbb_policy>::value")
            && KT::assert_constexpr_true<nstd::is_execution_policy_v<nstd::execution::tbb_policy>>(
                  c, "is_execution_policy_v<tbb_policy>")
            ;
                       }),
    KT::expect_success("tbb is an object of type tbb_policy",
                       [](KT::context& c)->bool{
        return KT::assert_type<nstd::execution::tbb_policy>(c, nstd::execution::tbb, "tbb")
            ;
                       }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("nstd:execution::tbb_policy", ac, av, ::tests);
}
