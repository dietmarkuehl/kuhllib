// nstd/algorithm/distance.t.cpp                                      -*-C++-*-
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

#include "nstd/algorithm/distance.hpp"
#include "nstd/cursor/model_single_pass.hpp"
#include "nstd/type_traits/is_same.hpp"
#include <cstddef>
#include "kuhl/test.hpp"

namespace KT = kuhl::test;
namespace NA = nstd::algorithm;
namespace NC = nstd::cursor;
namespace NT = nstd::type_traits;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("single_pass cursors rvalues", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3 };
            auto dist(NA::distance(NC::single_pass_begin(array), NC::single_pass_end(array)));
            return assert_equal(c, dist, 3u)
                // && NT::is_same<std::size_t, decltype(dist)>::value
                ;
        }),
    KT::expect_failure("placeholder", []()->bool{ return false; }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("algorithm::distance", ac, av, ::tests);
}
