// nstd/algorithm/mismatch.t.cpp                                      -*-C++-*-
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

#include "nstd/algorithm/mismatch.hpp"
#include "nstd/cursor/model_single_pass.hpp"
#include "nstd/cursor/single_pass.hpp"
#include "nstd/functional/equal_to.hpp"
#include "nstd/projection/model_readable.hpp"
#include "nstd/utility/equality_comparable.hpp"
#include "nstd/utility/pair.hpp"
#include "kuhl/test.hpp"

namespace NA = nstd::algorithm;
namespace NC = nstd::cursor;
namespace NF = nstd::functional;
namespace NP = nstd::projection;
namespace NU = nstd::utility;
namespace KT = kuhl::test;

namespace {
    using model_readable = NP::model_readable<NU::equality_comparable>;
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("empty sequences", [](KT::context& c)->bool{
            int values0[] = { 1 };
            int values1[] = { 2 };
            auto cur0 = NC::single_pass_begin(values0);
            NC::step(cur0);
            auto cur1 = NC::single_pass_begin(values1);
            NC::step(cur1);
            return KT::assert_true(c, "first range is empty", NC::at_same_pos(cur0, NC::single_pass_end(values0)))
                && KT::assert_true(c, "second range is empty", NC::at_same_pos(cur1, NC::single_pass_end(values1)))
                && KT::assert_true(c, "mismatch yields pair of ends (no predicat)",
                                   NC::at_same_pos(NA::mismatch(model_readable(), cur0, NC::single_pass_end(values0),
                                                                model_readable(), cur1, NC::single_pass_end(values1)),
                                                   NU::make_pair(NC::single_pass_end(values0), NC::single_pass_end(values1))))
                && KT::assert_true(c, "mismatch yields pair of ends (predicate)",
                                   NC::at_same_pos(NA::mismatch(model_readable(), cur0, NC::single_pass_end(values0),
                                                                model_readable(), cur1, NC::single_pass_end(values1),
                                                                NF::equal_to<>()),
                                                   NU::make_pair(NC::single_pass_end(values0), NC::single_pass_end(values1))))
                ;
        }),
    KT::expect_success("non-empty/empty", [](KT::context& c)->bool{
            int values0[] = { 1 };
            int values1[] = { 2 };
            auto cur0 = NC::single_pass_begin(values0);
            auto cur1 = NC::single_pass_begin(values1);
            NC::step(cur1);
            return KT::assert_false(c, "first range is non-empty", NC::at_same_pos(cur0, NC::single_pass_end(values0)))
                && KT::assert_true(c, "second range is empty", NC::at_same_pos(cur1, NC::single_pass_end(values1)))
                && KT::assert_true(c, "mismatch yields pair of begin/end (no predicat)",
                                   NC::at_same_pos(NA::mismatch(model_readable(), cur0, NC::single_pass_end(values0),
                                                                model_readable(), cur1, NC::single_pass_end(values1)),
                                                   NU::make_pair(NC::single_pass_begin(values0), NC::single_pass_end(values1))))
                && KT::assert_true(c, "mismatch yields pair of begin/end (predicate)",
                                   NC::at_same_pos(NA::mismatch(model_readable(), cur0, NC::single_pass_end(values0),
                                                                model_readable(), cur1, NC::single_pass_end(values1),
                                                                NF::equal_to<>()),
                                                   NU::make_pair(NC::single_pass_begin(values0), NC::single_pass_end(values1))))
                ;
        }),
    KT::expect_success("empty/non-empty", [](KT::context& c)->bool{
            int values0[] = { 1 };
            int values1[] = { 2 };
            auto cur0 = NC::single_pass_begin(values0);
            NC::step(cur0);
            auto cur1 = NC::single_pass_begin(values1);
            return KT::assert_true(c, "first range is empty", NC::at_same_pos(cur0, NC::single_pass_end(values0)))
                && KT::assert_false(c, "second range is non-empty", NC::at_same_pos(cur1, NC::single_pass_end(values1)))
                && KT::assert_true(c, "mismatch yields pair of end/begin (no predicat)",
                                   NC::at_same_pos(NA::mismatch(model_readable(), cur0, NC::single_pass_end(values0),
                                                                model_readable(), cur1, NC::single_pass_end(values1)),
                                                   NU::make_pair(NC::single_pass_end(values0), NC::single_pass_begin(values1))))
                && KT::assert_true(c, "mismatch yields pair of end/begin (predicate)",
                                   NC::at_same_pos(NA::mismatch(model_readable(), cur0, NC::single_pass_end(values0),
                                                                model_readable(), cur1, NC::single_pass_end(values1),
                                                                NF::equal_to<>()),
                                                   NU::make_pair(NC::single_pass_end(values0), NC::single_pass_begin(values1))))
                ;
        }),
    KT::expect_success("non-empty/non-empty (first mismatching)", [](KT::context& c)->bool{
            int values0[] = { 1, 2, 3, 4, 5 };
            int values1[] = { 2, 2, 3, 4, 5 };
            auto cur0 = NC::single_pass_begin(values0);
            auto cur1 = NC::single_pass_begin(values1);
            return KT::assert_false(c, "first range is non-empty", NC::at_same_pos(cur0, NC::single_pass_end(values0)))
                && KT::assert_false(c, "second range is non-empty", NC::at_same_pos(cur1, NC::single_pass_end(values1)))
                && KT::assert_true(c, "mismatch yields pair of begins (no predicat)",
                                   NC::at_same_pos(NA::mismatch(model_readable(), cur0, NC::single_pass_end(values0),
                                                                model_readable(), cur1, NC::single_pass_end(values1)),
                                                   NU::make_pair(NC::single_pass_begin(values0), NC::single_pass_begin(values1))))
                && KT::assert_true(c, "mismatch yields pair of begins (predicate)",
                                   NC::at_same_pos(NA::mismatch(model_readable(), cur0, NC::single_pass_end(values0),
                                                                model_readable(), cur1, NC::single_pass_end(values1),
                                                                NF::equal_to<>()),
                                                   NU::make_pair(NC::single_pass_begin(values0), NC::single_pass_begin(values1))))
                ;
        }),
    KT::expect_success("non-empty/non-empty (middle mismatching)", [](KT::context& c)->bool{
            int values0[] = { 1, 2, 3, 4, 5 };
            int values1[] = { 1, 2, 4, 4, 5 };
            auto cur0 = NC::single_pass_begin(values0);
            auto expect0 = NC::single_pass_begin(values0);
            NC::step(expect0, 2);
            auto cur1 = NC::single_pass_begin(values1);
            auto expect1 = NC::single_pass_begin(values1);
            NC::step(expect1, 2);
            return KT::assert_false(c, "first range is non-empty", NC::at_same_pos(cur0, NC::single_pass_end(values0)))
                && KT::assert_false(c, "second range is non-empty", NC::at_same_pos(cur1, NC::single_pass_end(values1)))
                && KT::assert_true(c, "mismatch yields pair of middles (no predicat)",
                                   NC::at_same_pos(NA::mismatch(model_readable(), cur0, NC::single_pass_end(values0),
                                                                model_readable(), cur1, NC::single_pass_end(values1)),
                                                   NU::make_pair(expect0, expect1)))
                && KT::assert_true(c, "mismatch yields pair of middles (predicate)",
                                   NC::at_same_pos(NA::mismatch(model_readable(), cur0, NC::single_pass_end(values0),
                                                                model_readable(), cur1, NC::single_pass_end(values1),
                                                                NF::equal_to<>()),
                                                   NU::make_pair(expect0, expect1)))
                ;
        }),
    KT::expect_success("non-empty/non-empty (last mismatching)", [](KT::context& c)->bool{
            int values0[] = { 1, 2, 3, 4, 5 };
            int values1[] = { 1, 2, 3, 4, 6 };
            auto cur0 = NC::single_pass_begin(values0);
            auto expect0 = NC::single_pass_begin(values0);
            NC::step(expect0, 4);
            auto cur1 = NC::single_pass_begin(values1);
            auto expect1 = NC::single_pass_begin(values1);
            NC::step(expect1, 4);
            return KT::assert_false(c, "first range is non-empty", NC::at_same_pos(cur0, NC::single_pass_end(values0)))
                && KT::assert_false(c, "second range is non-empty", NC::at_same_pos(cur1, NC::single_pass_end(values1)))
                && KT::assert_true(c, "mismatch yields pair of lasts (no predicat)",
                                   NC::at_same_pos(NA::mismatch(model_readable(), cur0, NC::single_pass_end(values0),
                                                                model_readable(), cur1, NC::single_pass_end(values1)),
                                                   NU::make_pair(expect0, expect1)))
                && KT::assert_true(c, "mismatch yields pair of lasts (predicate)",
                                   NC::at_same_pos(NA::mismatch(model_readable(), cur0, NC::single_pass_end(values0),
                                                                model_readable(), cur1, NC::single_pass_end(values1),
                                                                NF::equal_to<>()),
                                                   NU::make_pair(expect0, expect1)))
                ;
        }),
    KT::expect_success("non-empty/non-empty (none mismatching)", [](KT::context& c)->bool{
            int values0[] = { 1, 2, 3, 4, 5 };
            int values1[] = { 1, 2, 3, 4, 5 };
            auto cur0 = NC::single_pass_begin(values0);
            auto expect0 = NC::single_pass_begin(values0);
            NC::step(expect0, 5);
            auto cur1 = NC::single_pass_begin(values1);
            auto expect1 = NC::single_pass_begin(values1);
            NC::step(expect1, 5);
            return KT::assert_false(c, "first range is non-empty", NC::at_same_pos(cur0, NC::single_pass_end(values0)))
                && KT::assert_false(c, "second range is non-empty", NC::at_same_pos(cur1, NC::single_pass_end(values1)))
                && KT::assert_true(c, "mismatch yields pair of ends (no predicat)",
                                   NC::at_same_pos(NA::mismatch(model_readable(), cur0, NC::single_pass_end(values0),
                                                                model_readable(), cur1, NC::single_pass_end(values1)),
                                                   NU::make_pair(expect0, expect1)))
                && KT::assert_true(c, "mismatch yields pair of ends (predicate)",
                                   NC::at_same_pos(NA::mismatch(model_readable(), cur0, NC::single_pass_end(values0),
                                                                model_readable(), cur1, NC::single_pass_end(values1),
                                                                NF::equal_to<>()),
                                                   NU::make_pair(expect0, expect1)))
                && KT::assert_true(c, "the expected cursor are at the end",
                                   NC::at_same_pos(NU::make_pair(expect0, expect1),
                                                   NU::make_pair(NC::single_pass_end(values0), NC::single_pass_end(values1))))
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("algorithm::mismatch", ac, av, ::tests);
}
