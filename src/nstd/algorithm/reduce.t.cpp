// nstd/algorithm/reduce.t.cpp                                        -*-C++-*-
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
#include "nstd/algorithm/reduce.hpp"
#include "kuhl/test.hpp"

namespace NA = ::nstd::algorithm;
namespace NE = ::nstd::execution;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("empty sequence/no default", [](KT::context& c)->bool{
            int array[] = { 1 };
            return KT::assert_equal(c, "sum of empty array/no default",
                                    0, NA::reduce(array, array))
                ;
        }),
    KT::expect_success("empty sequence/default", [](KT::context& c)->bool{
            int array[] = { 1 };
            return KT::assert_equal(c, "sum of empty array/no default",
                                    17, NA::reduce(array, array, 17))
                ;
        }),
    KT::expect_success("empty sequence/default + op", [](KT::context& c)->bool{
            int array[] = { 1 };
            return KT::assert_equal(c, "sum of empty array/no default",
                                    17, NA::reduce(array, array, 17,
                                                   [](auto a, auto b){ return a * b; }))
                ;
        }),
    KT::expect_success("non-empty sequence/no default", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3, 4 };
            return KT::assert_equal(c, "sum of non-empty array/no default",
                                    10, NA::reduce(array, array + 4))
                ;
        }),
    KT::expect_success("non-empty sequence/default", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3, 4 };
            return KT::assert_equal(c, "sum of non-empty array/no default",
                                    27, NA::reduce(array, array + 4, 17))
                ;
        }),
    KT::expect_success("empty sequence/default + op", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3, 4 };
            return KT::assert_equal(c, "sum of empty array/no default",
                                    408, NA::reduce(array, array + 4, 17,
                                                    [](auto a, auto b){ return a * b; }))
                ;
        }),

    KT::expect_success("seq empty sequence/no default", [](KT::context& c)->bool{
            int array[] = { 1 };
            return KT::assert_equal(c, "sum of empty array/no default",
                                    0, NA::reduce(NE::seq, array, array))
                ;
        }),
    KT::expect_success("seq empty sequence/default", [](KT::context& c)->bool{
            int array[] = { 1 };
            return KT::assert_equal(c, "sum of empty array/no default",
                                    17, NA::reduce(NE::seq, array, array, 17))
                ;
        }),
    KT::expect_success("seq empty sequence/default + op", [](KT::context& c)->bool{
            int array[] = { 1 };
            return KT::assert_equal(c, "sum of empty array/no default",
                                    17, NA::reduce(NE::seq, array, array, 17,
                                                   [](auto a, auto b){ return a * b; }))
                ;
        }),
    KT::expect_success("seq non-empty sequence/no default", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3, 4 };
            return KT::assert_equal(c, "sum of non-empty array/no default",
                                    10, NA::reduce(NE::seq, array, array + 4))
                ;
        }),
    KT::expect_success("seq non-empty sequence/default", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3, 4 };
            return KT::assert_equal(c, "sum of non-empty array/default",
                                    27, NA::reduce(NE::seq, array, array + 4, 17))
                ;
        }),
    KT::expect_success("seq non-empty sequence/default + op", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3, 4 };
            return KT::assert_equal(c, "sum of non-empty array/default",
                                    408, NA::reduce(NE::seq, array, array + 4, 17,
                                                    [](auto a, auto b){ return a * b; }))
                ;
        }),

    KT::expect_success("par empty sequence/no default", [](KT::context& c)->bool{
            int array[] = { 1 };
            return KT::assert_equal(c, "sum of empty array/no default",
                                    0, NA::reduce(NE::par(1), array, array))
                ;
        }),
    KT::expect_success("par empty sequence/default", [](KT::context& c)->bool{
            int array[] = { 1 };
            return KT::assert_equal(c, "sum of empty array/no default",
                                    17, NA::reduce(NE::par(1), array, array, 17))
                ;
        }),
    KT::expect_success("par empty sequence/default + op", [](KT::context& c)->bool{
            int array[] = { 1 };
            return KT::assert_equal(c, "sum of empty array/no default",
                                    17, NA::reduce(NE::par(1), array, array, 17,
                                                   [](auto a, auto b){ return a * b; }))
                ;
        }),
    KT::expect_success("par non-empty sequence/no default", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3, 4 };
            return KT::assert_equal(c, "sum of non-empty array/no default",
                                    10, NA::reduce(NE::par(1), array, array + 4))
                ;
        }),
    KT::expect_success("par non-empty sequence/default", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3, 4 };
            return KT::assert_equal(c, "sum of non-empty array/default",
                                    10, NA::reduce(NE::par(1), array, array + 4, 0))
                ;
        }),
    KT::expect_success("par non-empty sequence/default + op", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3, 4 };
            return KT::assert_equal(c, "sum of non-empty array/default + op",
                                    24, NA::reduce(NE::par(1), array, array + 4, 1,
                                                    [](auto a, auto b){ return a * b; }))
                ;
        }),

    KT::expect_success("par_unseq empty sequence/no default", [](KT::context& c)->bool{
            int array[] = { 1 };
            return KT::assert_equal(c, "sum of empty array/no default",
                                    0, NA::reduce(NE::par_unseq, array, array))
                ;
        }),
    KT::expect_success("par_unseq empty sequence/default", [](KT::context& c)->bool{
            int array[] = { 1 };
            return KT::assert_equal(c, "sum of empty array/no default",
                                    17, NA::reduce(NE::par_unseq, array, array, 17))
                ;
        }),
    KT::expect_success("par_unseq empty sequence/default + op", [](KT::context& c)->bool{
            int array[] = { 1 };
            return KT::assert_equal(c, "sum of empty array/no default",
                                    17, NA::reduce(NE::par_unseq, array, array, 17,
                                                   [](auto a, auto b){ return a * b; }))
                ;
        }),
    KT::expect_success("par_unseq non-empty sequence/no default", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3, 4 };
            return KT::assert_equal(c, "sum of non-empty array/no default",
                                    10, NA::reduce(NE::par_unseq, array, array + 4))
                ;
        }),
    KT::expect_success("par_unseq non-empty sequence/default", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3, 4 };
            return KT::assert_equal(c, "sum of non-empty array/default",
                                    10, NA::reduce(NE::par_unseq, array, array + 4, 0))
                ;
        }),
    KT::expect_success("par_unseq non-empty sequence/default + op", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3, 4 };
            return KT::assert_equal(c, "sum of non-empty array/no default",
                                    24, NA::reduce(NE::par_unseq, array, array + 4, 1,
                                                    [](auto a, auto b){ return a * b; }))
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("algorithm::reduce()", ac, av, ::tests);
}
