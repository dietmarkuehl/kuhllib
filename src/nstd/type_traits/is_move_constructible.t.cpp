// nstd/type_traits/is_move_constructible.t.cpp                       -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2017 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/type_traits/is_move_constructible.hpp"
#include "kuhl/test.hpp"

namespace KT = ::kuhl::test;
namespace NT = ::nstd::type_traits;

// ----------------------------------------------------------------------------

namespace
{
    struct copyable
    {
        copyable(copyable&) = default;
        copyable(copyable&&) = delete;
    };
    struct movable
    {
        movable(movable&) = delete;
        movable(movable&&) = default;
    };
    class private_movable
    {
        private_movable(private_movable&) = delete;
        private_movable(private_movable&&) = default;
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("void isn't move constructible", [](KT::context& c)->bool{
            constexpr bool value = NT::is_move_constructible<void>::value;
            constexpr bool value_v = NT::is_move_constructible_v<void>;
            KT::use(value);
            KT::use(value_v);
            return KT::assert_false(c, "void", NT::is_move_constructible<void>::value)
                && KT::assert_false(c, "void const", NT::is_move_constructible<void const>::value)
                && KT::assert_false(c, "void volatile", NT::is_move_constructible<void volatile>::value)
                && KT::assert_false(c, "void const volatile", NT::is_move_constructible<void const volatile>::value)
                && KT::assert_false(c, "void", NT::is_move_constructible_v<void>)
                && KT::assert_false(c, "void const", NT::is_move_constructible_v<void const>)
                && KT::assert_false(c, "void volatile", NT::is_move_constructible_v<void volatile>)
                && KT::assert_false(c, "void const volatile", NT::is_move_constructible_v<void const volatile>)
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("is_move_constructible", ac, av, ::tests);
}
