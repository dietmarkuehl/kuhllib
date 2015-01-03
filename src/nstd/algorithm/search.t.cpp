// nstd/algorithm/search.t.cpp                                        -*-C++-*-
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

#include "nstd/algorithm/search.hpp"
#include "nstd/cursor/single_pass.hpp"
#include "nstd/cursor/model_single_pass.hpp"
#include "nstd/projection/model_readable.hpp"
#include "nstd/utility/equality_comparable.hpp"
#include "kuhl/test.hpp"

namespace NA = nstd::algorithm;
namespace NC = nstd::cursor;
namespace NP = nstd::projection;
namespace NU = nstd::utility;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct predicate {
        auto operator()(NP::model_value<int>  const& v0,
                        NP::model_value<long> const& v1) const
            -> bool {
            return v0.get_value() == v1.get_value();
        }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("prepare", [](KT::context& c)->bool{
            predicate pred;
            return KT::assert_true(c, "equal", pred(NP::model_value<int>(17), NP::model_value<long>(17)))
                && KT::assert_false(c, "not equal", pred(NP::model_value<int>(17), NP::model_value<long>(42)))
                ;
        }),
    KT::expect_success("empty second range yields first of first range", [](KT::context& c)->bool{
            int  array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9,  1, 2, 3, 4, 5, 6, 7, 8, 9 };
            long other[] = { 1l };
            auto oit(NC::single_pass_begin(other));
            NC::step(oit);
            return KT::assert_type<NC::model_single_pass<int>,
                                   decltype(NA::search(NP::model_readable<NU::equality_comparable>(),
                                                       NC::single_pass_begin(array), NC::single_pass_end(array),
                                                       NP::model_readable<NU::equality_comparable>(),
                                                       oit, NC::single_pass_end(other))) >(c, "type")
                && KT::assert_true(c, "equality",
                                   NC::at_same_pos(NA::search(NP::model_readable<NU::equality_comparable>(),
                                                              NC::single_pass_begin(array), NC::single_pass_end(array),
                                                              NP::model_readable<NU::equality_comparable>(),
                                                              oit, NC::single_pass_end(other)),
                                                   NC::single_pass_begin(array)))
                && KT::assert_type<NC::model_single_pass<int>,
                                   decltype(NA::search(NP::model_readable<>(),
                                                       NC::single_pass_begin(array), NC::single_pass_end(array),
                                                       NP::model_readable<>(),
                                                       oit, NC::single_pass_end(other),
                                                       predicate())) >(c, "type")
                && KT::assert_true(c, "predicate",
                                   NC::at_same_pos(NA::search(NP::model_readable<>(),
                                                              NC::single_pass_begin(array), NC::single_pass_end(array),
                                                              NP::model_readable<>(),
                                                              oit, NC::single_pass_end(other),
                                                              predicate()),
                                                   NC::single_pass_begin(array)))
                 ;
        }),
    KT::expect_success("match at the start", [](KT::context& c)->bool{
            int  array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9,  1, 2, 3, 4, 5, 6, 7, 8, 9 };
            long other[] = { 1l, 2l, 3l };
            return KT::assert_true(c, "equality",
                                   NC::at_same_pos(NA::search(NP::model_readable<NU::equality_comparable>(),
                                                              NC::single_pass_begin(array), NC::single_pass_end(array),
                                                              NP::model_readable<NU::equality_comparable>(),
                                                              NC::single_pass_begin(other), NC::single_pass_end(other)),
                                                   NC::single_pass_begin(array)))
                && KT::assert_true(c, "predicate",
                                   NC::at_same_pos(NA::search(NP::model_readable<>(),
                                                              NC::single_pass_begin(array), NC::single_pass_end(array),
                                                              NP::model_readable<>(),
                                                              NC::single_pass_begin(other), NC::single_pass_end(other),
                                                              predicate()),
                                                   NC::single_pass_begin(array)))
                 ;
        }),
    KT::expect_success("match at the second object", [](KT::context& c)->bool{
            int  array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9,  1, 2, 3, 4, 5, 6, 7, 8, 9 };
            long other[] = { 2l, 3l, 4l };
            auto expect(NC::single_pass_begin(array));
            NC::step(expect);
            return KT::assert_true(c, "equality",
                                   NC::at_same_pos(NA::search(NP::model_readable<NU::equality_comparable>(),
                                                              NC::single_pass_begin(array), NC::single_pass_end(array),
                                                              NP::model_readable<NU::equality_comparable>(),
                                                              NC::single_pass_begin(other), NC::single_pass_end(other)),
                                                   expect))
                && KT::assert_true(c, "predicate",
                                   NC::at_same_pos(NA::search(NP::model_readable<>(),
                                                              NC::single_pass_begin(array), NC::single_pass_end(array),
                                                              NP::model_readable<>(),
                                                              NC::single_pass_begin(other), NC::single_pass_end(other),
                                                              predicate()),
                                                   expect))
                 ;
        }),
    KT::expect_success("match ending at the end", [](KT::context& c)->bool{
            int  array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9,  1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
            long other[] = { 7l, 8l, 9l, 10l };
            auto expect(NC::single_pass_begin(array));
            NC::step(expect, 15);
            return KT::assert_true(c, "equality",
                                   NC::at_same_pos(NA::search(NP::model_readable<NU::equality_comparable>(),
                                                              NC::single_pass_begin(array), NC::single_pass_end(array),
                                                              NP::model_readable<NU::equality_comparable>(),
                                                              NC::single_pass_begin(other), NC::single_pass_end(other)),
                                                   expect))
                && KT::assert_true(c, "predicate",
                                   NC::at_same_pos(NA::search(NP::model_readable<>(),
                                                              NC::single_pass_begin(array), NC::single_pass_end(array),
                                                              NP::model_readable<>(),
                                                              NC::single_pass_begin(other), NC::single_pass_end(other),
                                                              predicate()),
                                                   expect))
                 ;
        }),
    KT::expect_success("no match yields end", [](KT::context& c)->bool{
            int  array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9,  1, 2, 3, 4, 5, 6, 7, 8, 9 };
            long other[] = { 2l, 3l, 5l };
            return KT::assert_true(c, "equality",
                                   NC::at_same_pos(NA::search(NP::model_readable<NU::equality_comparable>(),
                                                              NC::single_pass_begin(array), NC::single_pass_end(array),
                                                              NP::model_readable<NU::equality_comparable>(),
                                                              NC::single_pass_begin(other), NC::single_pass_end(other)),
                                                   NC::single_pass_end(array)))
                && KT::assert_true(c, "predicate",
                                   NC::at_same_pos(NA::search(NP::model_readable<>(),
                                                              NC::single_pass_begin(array), NC::single_pass_end(array),
                                                              NP::model_readable<>(),
                                                              NC::single_pass_begin(other), NC::single_pass_end(other),
                                                              predicate()),
                                                   NC::single_pass_end(array)))
                 ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("algorith::search", ac, av, ::tests);
}
