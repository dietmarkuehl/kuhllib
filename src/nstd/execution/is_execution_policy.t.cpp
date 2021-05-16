// nstd/execution/is_execution_policy.t.cpp                           -*-C++-*-
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

#include "nstd/execution/is_execution_policy.hpp"
#include "nstd/type_traits/integral_constant.hpp"
#include "kuhl/test.hpp"

namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct test_policy {
    };
}

namespace nstd {
    template <>
    struct is_execution_policy<test_policy>
        : ::nstd::type_traits::true_type {
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("int is not an execution policy",
                       [](KT::context& c)->bool{
           return KT::assert_constexpr_false<nstd::is_execution_policy<int>::value>(c, "is_execution_policy<int>::value")
               && KT::assert_constexpr_false<nstd::is_execution_policy<int>::value>(c, "is_execution_policy_v<int>")
                   ;
        }),
    KT::expect_success("is_execution_policy can be specialized",
                       [](KT::context& c)->bool{
           return KT::assert_constexpr_true<nstd::is_execution_policy<test_policy>::value>(c, "is_execution_policy<test_policy>::value")
               && KT::assert_constexpr_true<nstd::is_execution_policy<test_policy>::value>(c, "is_execution_policy_v<test_policy>")
                   ;
        }),
};

static KT::add_tests suite("execution::is_execution_policy", ::tests);
