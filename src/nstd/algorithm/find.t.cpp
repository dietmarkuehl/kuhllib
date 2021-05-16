// nstd/algorithm/find.t.cpp                                          -*-C++-*-
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

#include "nstd/algorithm/find.hpp"
#include "nstd/cursor/single_pass.hpp"
#include "nstd/cursor/model_single_pass.hpp"
#include "nstd/projection/model_readable.hpp"
#include "nstd/projection/model_value.hpp"
#include "nstd/utility/equality_comparable.hpp"
#include "kuhl/test.hpp"

namespace NA = nstd::algorithm;
namespace NC = nstd::cursor;
namespace NP = nstd::projection;
namespace NU = nstd::utility;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    struct marker {};
    struct put {
        int value;
        explicit put(int value, marker): value(value) {}
        put(put const&) = delete;
        put(put&&) = delete;
        auto operator=(put const&) -> void = delete;
        auto operator=(put&&) -> void = delete;
    };
    bool operator== (NP::model_value<int> const& value, put const& other) {
        return value.get_value() == other.value;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("find()'s value doesn't have to be copyable or movable",
                       [](KT::context& c)->bool{
            int array[] = { 1, 2, 3 };
            auto it0(NC::single_pass_begin(array));
            auto it1(it0);
            NC::step(it1);
            auto it2(it1);
            NC::step(it2);
            return KT::assert_true(c, "first",
                                   NC::at_same_pos(NA::find(NP::model_readable<>(),
                                                            NC::single_pass_begin(array), NC::single_pass_end(array),
                                                            put(1, marker())),
                                                   NC::single_pass_begin(array)))
                && KT::assert_true(c, "second",
                                   NC::at_same_pos(NA::find(NP::model_readable<>(),
                                                            NC::single_pass_begin(array), NC::single_pass_end(array),
                                                            put(2, marker())),
                                                   it1))
                && KT::assert_true(c, "third",
                                   NC::at_same_pos(NA::find(NP::model_readable<>(),
                                                            NC::single_pass_begin(array), NC::single_pass_end(array),
                                                            put(3, marker())),
                                                   it2))
                && KT::assert_true(c, "not found",
                                   NC::at_same_pos(NA::find(NP::model_readable<>(),
                                                            NC::single_pass_begin(array), NC::single_pass_end(array),
                                                            put(4, marker())),
                                                   NC::single_pass_end(array)))
                ;
        }),
    KT::expect_success("find() on empty range finds end", [](KT::context& c)->bool{
            int array[] = { 1 };
            auto begin = NC::single_pass_begin(array);
            NC::step(begin);
            auto end = NC::single_pass_end(array);
            auto it = NA::find(NP::model_readable<>(), begin, end, NP::model_value<int, NU::equality_comparable>(17));
            return KT::assert_true(c, "cursor at end", NC::at_same_pos(it, end))
                && KT::assert_type<NC::model_single_pass<int>, decltype(it)>(c, "a cursor is returned")
                ;
        }),
    KT::expect_success("find() on range without match find end", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3, 4, 5, 6, 7 };
            auto begin = NC::single_pass_begin(array);
            auto end = NC::single_pass_end(array);
            auto it = NA::find(NP::model_readable<>(), begin, end, NP::model_value<int, NU::equality_comparable>(17));
            return KT::assert_true(c, "cursor at end", NC::at_same_pos(it, end))
                && KT::assert_type<NC::model_single_pass<int>, decltype(it)>(c, "a cursor is returned")
                ;
        }),
    KT::expect_success("find() on range where first element matches finds begin", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3, 4, 5, 6, 7 };
            auto begin = NC::single_pass_begin(array);
            auto end = NC::single_pass_end(array);
            auto it = NA::find(NP::model_readable<>(), begin, end, NP::model_value<int, NU::equality_comparable>(1));
            return KT::assert_equal(c, "cursor at beginning", begin.get_pointer(), it.get_pointer())
                && KT::assert_type<NC::model_single_pass<int>, decltype(it)>(c, "a cursor is returned")
                ;
        }),
    KT::expect_success("find() on range where middle element matches finds middle", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3, 4, 5, 6, 7 };
            auto begin = NC::single_pass_begin(array);
            auto end = NC::single_pass_end(array);
            auto it = NA::find(NP::model_readable<>(), begin, end, NP::model_value<int, NU::equality_comparable>(4));
            return KT::assert_equal(c, "cursor at beginning", begin.get_pointer() + 3, it.get_pointer())
                && KT::assert_type<NC::model_single_pass<int>, decltype(it)>(c, "a cursor is returned")
                ;
        }),
    KT::expect_success("find() on range where last element matches finds last", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3, 4, 5, 6, 7 };
            auto begin = NC::single_pass_begin(array);
            auto end = NC::single_pass_end(array);
            auto it = NA::find(NP::model_readable<>(), begin, end, NP::model_value<int, NU::equality_comparable>(7));
            return KT::assert_equal(c, "cursor at beginning", begin.get_pointer() + 6, it.get_pointer())
                && KT::assert_type<NC::model_single_pass<int>, decltype(it)>(c, "a cursor is returned")
                ;
        }),
    //-dk:TODO verify that the appropriate number of increments and tests are used
};

static KT::add_tests suite("algorithm::find", ::tests);
