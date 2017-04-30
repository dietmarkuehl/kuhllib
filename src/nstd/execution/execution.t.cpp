// nstd/execution/execution.t.cpp                                     -*-C++-*-
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

#include "nstd/execution/execution.hpp"
#include "kuhl/test.hpp"

namespace KT = ::kuhl::test;
namespace NE = ::nstd::execution;
namespace NS = ::nstd;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("is_execution_policy", [](KT::context& c)->bool{
        return KT::assert_constexpr_false<NS::is_execution_policy<int>::value>(
                c, "is_execution_policy is defined")
            && KT::assert_constexpr_false<NS::is_execution_policy_v<int>>(
                c, "is_execution_policy_v is defined")
            ;
        }),
    KT::expect_success("seq is defined as sequenced_policy", [](KT::context& c)->bool{
        return KT::assert_type<NE::sequenced_policy>(c, NE::seq, "seq is defined")
            && KT::assert_constexpr_true<NS::is_execution_policy_v<NE::sequenced_policy>>(
                c, "sequenced_policy is an execution policy")
            ;
        }),
    KT::expect_success("par is defined as parallel_policy", [](KT::context& c)->bool{
        return KT::assert_type<NE::parallel_policy>(c, NE::par, "par is defined")
            && KT::assert_constexpr_true<NS::is_execution_policy_v<NE::parallel_policy>>(
                c, "parallel_policy is an execution policy")
            ;
        }),
    KT::expect_success("par_unseq is defined as parallel_unsequenced_policy", [](KT::context& c)->bool{
        return KT::assert_type<NE::parallel_unsequenced_policy>(c, NE::par_unseq, "par_unseq is defined")
            && KT::assert_constexpr_true<NS::is_execution_policy_v<NE::parallel_unsequenced_policy>>(
                c, "parallel_unsequenced_policy is an execution policy")
            ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("execution_header", ac, av, ::tests);
}
