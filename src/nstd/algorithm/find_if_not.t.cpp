// nstd/algorithm/find_if_not.t.cpp                                   -*-C++-*-
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

#include "nstd/algorithm/find_if_not.hpp"
#include "nstd/cursor/model_single_pass.hpp"
#include "nstd/projection/model_readable.hpp"
#include "kuhl/test.hpp"

namespace NA = nstd::algorithm;
namespace NC = nstd::cursor;
namespace NP = nstd::projection;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct predicate {
        int value;
        predicate(int value): value(value) {}
        auto operator()(NP::model_value<int> const& value) const -> bool {
            return value.get_value() < this->value;
        }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("no match", [](KT::context& c)->bool{
            int array[] = { 2, 4, 6 }; 
            return KT::assert_type<NC::model_single_pass<int>,
                                   decltype(NA::find_if_not(NP::model_readable<>(),
                                                            NC::single_pass_begin(array), NC::single_pass_end(array),
                                                            predicate(7)))>(c, "type")
                && KT::assert_true(c, "find end",
                                   NC::at_same_pos(NA::find_if_not(NP::model_readable<>(),
                                                                   NC::single_pass_begin(array), NC::single_pass_end(array),
                                                                   predicate(7)),
                                                   NC::single_pass_end(array)))
                ;
        }),
    KT::expect_success("match last", [](KT::context& c)->bool{
            int array[] = { 2, 4, 6 }; 
            auto it(NC::single_pass_begin(array));
            NC::step(it, 2u);
            return KT::assert_true(c, "find last",
                                   NC::at_same_pos(NA::find_if_not(NP::model_readable<>(),
                                                                   NC::single_pass_begin(array), NC::single_pass_end(array),
                                                                   predicate(5)),
                                                   it))
                ;
        }),
    KT::expect_success("match middle", [](KT::context& c)->bool{
            int array[] = { 2, 4, 6 }; 
            auto it(NC::single_pass_begin(array));
            NC::step(it, 1u);
            return KT::assert_true(c, "find middle",
                                   NC::at_same_pos(NA::find_if_not(NP::model_readable<>(),
                                                                   NC::single_pass_begin(array), NC::single_pass_end(array),
                                                                   predicate(3)),
                                                   it))
                ;
        }),
    KT::expect_success("match first", [](KT::context& c)->bool{
            int array[] = { 2, 4, 6 }; 
            auto it(NC::single_pass_begin(array));
            NC::step(it, 0u);
            return KT::assert_true(c, "find first",
                                   NC::at_same_pos(NA::find_if_not(NP::model_readable<>(),
                                                                   NC::single_pass_begin(array), NC::single_pass_end(array),
                                                                   predicate(1)),
                                                   it))
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("algorithm::find_if_not", ac, av, ::tests);
}
