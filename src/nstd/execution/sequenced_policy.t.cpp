// nstd/execution/sequenced_policy.t.cpp                              -*-C++-*-
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

#include "nstd/execution/sequenced_policy.hpp"
#include "nstd/execution/is_execution_policy.hpp"
#include "kuhl/test.hpp"

namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("sequenced_policy is an execution policy",
                       [](KT::context& c)->bool{
        return KT::assert_constexpr_true<nstd::is_execution_policy<nstd::execution::sequenced_policy>::value>(
                  c, "is_execution_policy<sequenced_policy>::value")
            && KT::assert_constexpr_true<nstd::is_execution_policy_v<nstd::execution::sequenced_policy>>(
                  c, "is_execution_policy_v<sequenced_policy>")
            ;
                       }),
    KT::expect_success("seq is an object of type sequenced_policy",
                       [](KT::context& c)->bool{
        return KT::assert_type<nstd::execution::sequenced_policy>(c, nstd::execution::seq, "seq")
            ;
                       }),
};

static KT::add_tests suite("execution::sequenced_policy", ::tests);
