// nstd/operators/relational.t.cpp                                    -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2015 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/operators/relational.hpp"
#include "kuhl/test.hpp"

namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct ordered
        : nstd::operators::relational::tag {
        int value;
        constexpr ordered(int value): value(value) {}
    };
    constexpr auto operator< (ordered const& o0, ordered const& o1) noexcept -> bool {
        return o0.value < o1.value;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("constexpr operator>() is created from operator<()",
                       [](KT::context& c)->bool{
            constexpr ::ordered o0(0), o1(1);
            return KT::assert_type<bool, decltype(o0 > o1)>(c, "operator> return type")
                && KT::assert_constexpr_false<(o0 > o0)>(c, "greater than on equal objects yields false")
                && KT::assert_constexpr_false<(o0 > o1)>(c, "greater than on a smaller and a bigger object yields false")
                && KT::assert_constexpr_true<(o1 > o0)>(c, "greater than on a bigger and a smaller object yields true")
                ;
        }),
    KT::expect_success("constexpr operator<=() is created from operator<()",
                       [](KT::context& c)->bool{
            constexpr ::ordered o0(0), o1(1);
            return KT::assert_type<bool, decltype(o0 <= o1)>(c, "operator<= return type")
                && KT::assert_constexpr_true<(o0 <= o0)>(c, "less equal on equal objects yields true")
                && KT::assert_constexpr_true<(o0 <= o1)>(c, "less equal on a smaller and a bigger object yields true")
                && KT::assert_constexpr_false<(o1 <= o0)>(c, "less equal on a bigger and a smaller object yields false")
                ;
        }),
    KT::expect_success("constexpr operator>=() is created from operator<()",
                       [](KT::context& c)->bool{
            constexpr ::ordered o0(0), o1(1);
            return KT::assert_type<bool, decltype(o0 >= o1)>(c, "operator>= return type")
                && KT::assert_constexpr_true<(o0 >= o0)>(c, "greater equal on equal objects yields true")
                && KT::assert_constexpr_false<(o0 >= o1)>(c, "greater equal on a smaller and a bigger object yields false")
                && KT::assert_constexpr_true<(o1 >= o0)>(c, "greater equal on a bigger and a smaller object yields true")
                ;
        }),
};

static KT::add_tests suite("operators::relational", ::tests);
