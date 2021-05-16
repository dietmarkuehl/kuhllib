// nstd/algorithm/none_of.t.cpp                                       -*-C++-*-
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

#include "nstd/algorithm/none_of.hpp"
#include "nstd/cursor/model_single_pass.hpp"
#include "nstd/cursor/single_pass.hpp"
#include "nstd/functional/model_predicate.hpp"
#include "nstd/projection/model_readable.hpp"
#include "kuhl/test.hpp"

namespace NA = nstd::algorithm;
namespace NC = nstd::cursor;
namespace NF = nstd::functional;
namespace NP = nstd::projection;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("empty range yields true", [](KT::context& c)->bool{
            int array[] = { 1 };
            auto begin(NC::single_pass_begin(array));
            NC::step(begin);
            auto end(NC::single_pass_end(array));
            return KT::assert_true(c, "the cursors start out equal", NC::at_same_pos(begin, end))
                && KT::assert_true(c, NA::none_of(NP::model_readable<>(), begin, end, NF::model_predicate(17)))
                && KT::assert_type<bool, decltype(NA::none_of(NP::model_readable<>(), begin, end, NF::model_predicate(17)))>(c, "type")
                ;
        }),
    KT::expect_success("range with first value matching yields false", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            auto begin(NC::single_pass_begin(array));
            auto end(NC::single_pass_end(array));
            return KT::assert_false(c, NA::none_of(NP::model_readable<>(), begin, end, NF::model_predicate(1)))
                ;
        }),
    KT::expect_success("range with a middle value matching yields false", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            auto begin(NC::single_pass_begin(array));
            auto end(NC::single_pass_end(array));
            return KT::assert_false(c, NA::none_of(NP::model_readable<>(), begin, end, NF::model_predicate(4)))
                ;
        }),
    KT::expect_success("range with the last value mismatching yields false", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            auto begin(NC::single_pass_begin(array));
            auto end(NC::single_pass_end(array));
            return KT::assert_false(c, NA::none_of(NP::model_readable<>(), begin, end, NF::model_predicate(9)))
                ;
        }),
    KT::expect_success("range with the no value matching yields true", [](KT::context& c)->bool{
            int array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            auto begin(NC::single_pass_begin(array));
            auto end(NC::single_pass_end(array));
            return KT::assert_true(c, NA::none_of(NP::model_readable<>(), begin, end, NF::model_predicate(17)))
                ;
        }),
};

static KT::add_tests suite("algorithm::none_of", ::tests);
