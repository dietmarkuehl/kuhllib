// nstd/algorithm/partition.t.cpp                                     -*-C++-*-
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

#include "nstd/algorithm/partition.hpp"
#include "nstd/algorithm/is_partitioned.hpp"
#include "nstd/algorithm/partition_point.hpp"
#include "nstd/iterator/forward_iterator.hpp"
#include <vector>
#include "kuhl/test.hpp"

namespace NA = ::nstd::algorithm;
namespace NI = ::nstd::iterator;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("partition() of empty forward sequence",
                       [](KT::context& c)->bool{
           int array[] = { 0 };
           return KT::assert_true(c, "returns begin",
                                  NI::forward_begin(array)
                                      == NA::partition(NI::forward_begin(array),
                                                       NI::forward_begin(array),
                                                       [](auto value){ return value % 2; }))
               ;
        }),
    KT::expect_success("partition a larger sequence",
                       [](KT::context& c)->bool {
            std::vector<int> range(1000000);
            for (size_t i(0); i != range.size(); ++i) {
                range[i] = i;
            }
            auto pred = [](auto value){ return value % 3; };
            auto it = NA::partition(range.begin(), range.end(), pred);
            return KT::assert_true(c, "range is partitioned",
                                   NA::is_partitioned(range.begin(), range.end(), pred))
                && KT::assert_true(c, "the partition point is returend",
                               it == NA::partition_point(range.begin(), range.end(), pred))
                ;
        }),
    KT::expect_failure("placeholder", [](KT::context& c)->bool{
           return false;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("algorithm::partition()", ac, av, ::tests);
}
