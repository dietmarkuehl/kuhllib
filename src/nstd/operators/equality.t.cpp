// nstd/operators/equality.t.cpp                                      -*-C++-*-
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

#include "nstd/operators/equality.hpp"
#include "kuhl/test.hpp"

namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct comparable
        : nstd::operators::equality::tag {
        int value;
        constexpr comparable(int value): value(value) {}
    };
    constexpr bool operator==(comparable const& c0, comparable const& c1) {
        return c0.value == c1.value;
    }

    template <typename>
    struct incomparable {
    };
    template <typename T>
    bool operator== (incomparable<T> const&, incomparable<T> const&) {
        return true;
    }
    template <typename T>
    bool operator!= (incomparable<T> const&, incomparable<T> const&) {
        return true;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("constexpr equality operators are created from compare()",
                       [](KT::context& c)->bool{
            constexpr ::comparable c0(0), c1(1);
            return KT::assert_type<bool, decltype(c0 != c1)>(c, "operator!= return type")
                && KT::assert_constexpr_false<c0 != c0>(c, "inequality on equal objects yields false")
                && KT::assert_constexpr_true<c0 != c1>(c, "inequality on non-equal objects yields true")
                && true;
        }),
    KT::expect_success("the inequality is not created for types not derived from the tag type",
                       [](KT::context& c)->bool{
            incomparable<comparable> ic;
            return KT::assert_true(c, "op!= on incomprable returns true", ic != ic)
                ;
    }),
    KT::expect_failure("placeholder", [](KT::context& c)->bool{
           return false;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("operators::equality", ac, av, ::tests);
}
