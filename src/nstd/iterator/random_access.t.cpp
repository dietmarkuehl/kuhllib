// nstd/iterator/random_access.t.cpp                                  -*-C++-*-
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

#include "nstd/iterator/random_access.hpp"
#include "nstd/container/list.hpp"
#include "nstd/container/vector.hpp"
#include "kuhl/test.hpp"

namespace KT = ::kuhl::test;
namespace NI = ::nstd::iterator;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("is_random_access<int...>", [](KT::context& c)->bool{
            return KT::assert_false(c, "int",
                                   NI::is_random_access<int>::value)
                && KT::assert_false(c, "int const",
                                   NI::is_random_access<int const>::value)
                && KT::assert_false(c, "int volatile",
                                   NI::is_random_access<int volatile>::value)
                && KT::assert_false(c, "int const volatile",
                                   NI::is_random_access<int const volatile>::value)
                ;
        }),
    KT::expect_success("is_random_access<int*...>", [](KT::context& c)->bool{
            return KT::assert_true(c, "int*",
                                   NI::is_random_access<int*>::value)
                && KT::assert_true(c, "int* const",
                                   NI::is_random_access<int* const>::value)
                && KT::assert_true(c, "int const*",
                                   NI::is_random_access<int const*>::value)
                && KT::assert_true(c, "int volatile*",
                                   NI::is_random_access<int volatile*>::value)
                && KT::assert_true(c, "int const volatile*",
                                   NI::is_random_access<int const volatile*>::value)
                ;
        }),
    KT::expect_success("is_random_access<vector<int>::iterator...>", [](KT::context& c)->bool{
            using iterator = ::nstd::container::vector<int>::iterator;

            return KT::assert_true(c, "iterator",
                                   NI::is_random_access<iterator>::value)
                && KT::assert_true(c, "iterator const",
                                   NI::is_random_access<iterator const>::value)
                && KT::assert_true(c, "iterator volatile",
                                   NI::is_random_access<iterator volatile>::value)
                && KT::assert_true(c, "iterator const volatile",
                                   NI::is_random_access<iterator const volatile>::value)
                ;
        }),
    KT::expect_success("is_random_access<list<int>::iterator...>", [](KT::context& c)->bool{
            using iterator = ::nstd::container::list<int>::iterator;

            return KT::assert_false(c, "iterator",
                                    NI::is_random_access<iterator>::value)
                && KT::assert_false(c, "iterator const",
                                    NI::is_random_access<iterator const>::value)
                && KT::assert_false(c, "iterator volatile",
                                    NI::is_random_access<iterator volatile>::value)
                && KT::assert_false(c, "iterator const volatile",
                                    NI::is_random_access<iterator const volatile>::value)
                ;
        }),
    //KT::expect_failure("placeholder", [](KT::context& c)->bool{
    //       return false;
    //    }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("iterator::random_access", ac, av, ::tests);
}
