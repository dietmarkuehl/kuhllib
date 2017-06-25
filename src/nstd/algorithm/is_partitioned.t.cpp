// nstd/algorithm/is_partitioned.t.cpp                                -*-C++-*-
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

#include "kuhl/test.hpp"
#include "nstd/algorithm/is_partitioned.hpp"
#include "nstd/cursor/array.hpp"

namespace KT = ::kuhl::test;
namespace NA = ::nstd::algorithm;
namespace NC = ::nstd::cursor;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("an empty range is partitioned",
                       [](KT::context& c)->bool {
            int array[] = { 0 };
            unsigned int count(0);
            return KT::assert_true(c, "is_partitioned() with an empty range",
                                   NA::is_partitioned(array, array,
                                                      [&](auto){ ++count; return true; }))
                && KT::assert_equal(c, "zero predicate uses", 0u, count)
                ;
        }),
    KT::expect_success("a range with all matching elements is partitioned",
                       [](KT::context& c)->bool {
            int array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            unsigned int count(0);
            return KT::assert_true(c, "is_partitioned()",
                                   NA::is_partitioned(NC::begin(array), NC::end(array),
                                                      [&](auto){ ++count; return true; }))
                && KT::assert_equal(c, "one predicate use per element", NC::size(array), count)
                ;
        }),
    KT::expect_success("a range with all mismatching elements is partitioned",
                       [](KT::context& c)->bool {
            int array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            unsigned int count(0);
            return KT::assert_true(c, "is_partitioned()",
                                   NA::is_partitioned(NC::begin(array), NC::end(array),
                                                      [&](auto){ ++count; return false; }))
                && KT::assert_equal(c, "one predicate use per element", NC::size(array), count)
                ;
        }),
    KT::expect_success("a range with all matching preceding non-matching elements is partitioned",
                       [](KT::context& c)->bool {
            int array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            unsigned int count(0);
            return KT::assert_true(c, "is_partitioned()",
                                   NA::is_partitioned(NC::begin(array), NC::end(array),
                                                      [&](auto v){ ++count; return v < 5; }))
                && KT::assert_equal(c, "one predicate use per element", NC::size(array), count)
                ;
        }),
    KT::expect_success("a range a non-matching preceding a matching element is not partitioned",
                       [](KT::context& c)->bool {
            int array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            unsigned int count(0);
            return KT::assert_false(c, "is_partitioned()",
                                    NA::is_partitioned(NC::begin(array), NC::end(array),
                                                       [&](auto v){ ++count; return v != 2 &&  v < 5; }))
                && KT::assert_equal(c, "predicate use to inconsistency", 4u, count)
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("nstd::algorithm::is_partitioned", ac, av, ::tests);
}
