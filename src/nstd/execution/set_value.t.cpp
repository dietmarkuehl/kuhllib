// nstd/execution/set_value.t.cpp                                     -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2021 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/execution/set_value.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD   = ::test_declarations;
namespace EX   = ::nstd::execution;
namespace UTIL = ::nstd::utility;
namespace KT   = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    template <bool Noexcept>
    struct member_set_values
    {
        int*    const ptr1;
        double* const ptr2;
        friend auto tag_invoke(EX::set_value_t, member_set_values&& r, int value1, double value2) noexcept(Noexcept) ->void {
            *r.ptr1 = value1;
            *r.ptr2 = value2;
        }
    };

    template <bool Noexcept>
    struct non_member_set_values
    {
        int*    const ptr1;
        double* const ptr2;
    };
    template <bool Noexcept>
    auto tag_invoke(EX::set_value_t, non_member_set_values<Noexcept>&& r, int value1, double value2) noexcept(Noexcept) -> void {
            *r.ptr1 = value1;
            *r.ptr2 = value2;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("member set_value noexcept(true)", []{
        int                         value1(17);
        double                      value2(1.25);
        TD::member_set_values<true> r{&value1, &value2};
        EX::set_value(UTIL::move(r), 42, 1.25);
        return value1 == 42
            && value2 == 1.25
            && noexcept(EX::set_value(UTIL::move(r), 42, 1.25))
            ;
        }),
    KT::expect_success("member set_value noexcept(false)", []{
        int                          value1(17);
        double                       value2(1.25);
        TD::member_set_values<false> r{&value1, &value2};
        EX::set_value(UTIL::move(r), 42, 1.25);
        return value1 == 42
            && value2 == 1.25
            && !noexcept(EX::set_value(UTIL::move(r), 42, 1.25))
            ;
        }),
    KT::expect_success("non-member set_value noexcept(true)", []{
        int                             value1(17);
        double                          value2(1.25);
        TD::non_member_set_values<true> r{&value1, &value2};
        EX::set_value(UTIL::move(r), 42, 1.25);
        return value1 == 42
            && value2 == 1.25
            && noexcept(EX::set_value(UTIL::move(r), 42, 1.25))
            ;
        }),
    KT::expect_success("non-member set_value noexcept(false)", []{
        int                              value1(17);
        double                           value2(1.25);
        TD::non_member_set_values<false> r{&value1, &value2};
        EX::set_value(UTIL::move(r), 42, 1.25);
        return value1 == 42
            && value2 == 1.25
            && !noexcept(EX::set_value(UTIL::move(r), 42, 1.25))
            ;
        }),
};

static KT::add_tests suite("set_value", ::tests);
