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
#include <cstddef>
#include "kuhl/test.hpp"

namespace KT = kuhl::test;
namespace NA = nstd::algorithm;
namespace NC = nstd::cursor;

// ----------------------------------------------------------------------------

namespace
{
    struct single_pass_cursor: NC::single_pass {
        using difference_type = unsigned char;
        int index;
        explicit single_pass_cursor(int index): index(index) {}
    };
    auto cursor_at_same_pos(single_pass_cursor const& c0, single_pass_cursor const& c1) -> bool {
        return c0.index == c1.index;
    }
    auto cursor_step(single_pass_cursor& c) -> void {
        ++c.index;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("single_pass cursors rvalues", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3 };
            auto dist(NA::distance(NC::single_pass_begin(array), NC::single_pass_end(array)));
            return assert_equal(c, "three elements", dist, 3u)
                && KT::assert_type<std::size_t, decltype(dist)>(c, "difference_type")
                ;
        }),
    KT::expect_success("single_pass cursors lvalues", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3, 4 };
            auto it(NC::single_pass_begin(array));
            auto end(NC::single_pass_end(array));
            auto dist(NA::distance(it, end));
            return assert_equal(c, "four elements", dist, 4u)
                && assert_equal(c, "it didn't move", it.get_pointer(), NC::single_pass_begin(array).get_pointer())
                && KT::assert_type<std::size_t, decltype(dist)>(c, "difference_type")
                ;
        }),
    KT::expect_success("custom cursor", [](KT::context& c)->bool {
            single_pass_cursor it(0), end(17);
            auto dist(NA::distance(it, end));
            return assert_equal(c, "17 elements", dist, 17u)
                && KT::assert_type<unsigned char, decltype(dist)>(c, "difference_type")
                ;
        }),
    KT::expect_success("length of empty sequence", [](KT::context& c)->bool {
            single_pass_cursor it(17), end(17);
            auto dist(NA::distance(it, end));
            return assert_equal(c, "sequence is empty", dist, 0u)
                && KT::assert_type<unsigned char, decltype(dist)>(c, "difference_type")
                ;
        }),
};

static KT::add_tests suite("algorithm::distance", ::tests);
