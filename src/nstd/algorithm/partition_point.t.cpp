// nstd/algorithm/partition_point.t.cpp                               -*-C++-*-
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

#include "nstd/algorithm/partition_point.hpp"
#include "kuhl/test.hpp"
#include "nstd/cursor/array.hpp"

namespace KT = ::kuhl::test;
namespace KA = ::nstd::algorithm;
namespace KC = ::nstd::cursor;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("empty range", [](KT::context& c)->bool {
            int array[] = { 0 };
            return KT::assert_equal(c, "begin == partition_point(begin, begin, pred)",
                                    KC::begin(array),
                                    KA::partition_point(KC::begin(array), KC::begin(array),
                                                        [](auto){ return true; }))
                ;
        }),
    KT::expect_success("matching range", [](KT::context& c)->bool {
            int array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 , 9 };
            return KT::assert_equal(c, "end == partition_point(begin, end, pred)",
                                    KC::end(array),
                                    KA::partition_point(KC::begin(array), KC::end(array),
                                                        [](auto){ return true; }))
                ;
        }),
    KT::expect_success("mismatching range", [](KT::context& c)->bool {
            int array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 , 9 };
            return KT::assert_equal(c, "begin == partition_point(begin, end, pred)",
                                    KC::begin(array),
                                    KA::partition_point(KC::begin(array), KC::end(array),
                                                        [](auto){ return false; }))
                ;
        }),
    KT::expect_success("partly matching range", [](KT::context& c)->bool {
            int array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 , 9 };
            return KT::assert_equal(c, "begin + 4 == partition_point(begin, end, v < 4)",
                                    KC::begin(array) + 4,
                                    KA::partition_point(KC::begin(array), KC::end(array),
                                                        [](auto v){ return v < 4; }))
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("partition_point", ac, av, ::tests);
}
