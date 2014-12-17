// nstd/functional/model_predicate.t.cpp                              -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2014 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/functional/model_predicate.hpp"
#include "nstd/projection/model_value.hpp"
#include "kuhl/test.hpp"

namespace NF = nstd::functional;
namespace NP = nstd::projection;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("a matching value yields true", [](KT::context& c)->bool{
            return KT::assert_true(c, "value", NF::model_predicate(43)(NP::model_value<int>(43)))
                && KT::assert_type<bool, decltype(NF::model_predicate(43)(NP::model_value<int>(43)))>(c, "type")
                ;
        }),
    KT::expect_success("a mismatching value yields false", [](KT::context& c)->bool{
            return KT::assert_false(c, "value", NF::model_predicate(17)(NP::model_value<int>(43)))
                && KT::assert_type<bool, decltype(NF::model_predicate(17)(NP::model_value<int>(43)))>(c, "type")
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("functional::model_predicate", ac, av, ::tests);
}
