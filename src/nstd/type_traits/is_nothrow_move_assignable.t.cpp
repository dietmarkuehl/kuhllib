// nstd/type_traits/is_nothrow_move_assignable.t.cpp                  -*-C++-*-
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

#include "nstd/type_traits/is_nothrow_move_assignable.hpp"
#include "kuhl/test.hpp"

namespace NT = nstd::type_traits;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    struct nothrow_move_assignable {
        nothrow_move_assignable(nothrow_move_assignable const&) noexcept(false);
        nothrow_move_assignable(nothrow_move_assignable&&) noexcept(false);
        ~nothrow_move_assignable() noexcept(false);
        nothrow_move_assignable& operator=(nothrow_move_assignable const&) noexcept(false);
        nothrow_move_assignable& operator=(nothrow_move_assignable&&) noexcept(true);
    };

    struct move_assignable {
        move_assignable(move_assignable const&) noexcept(true);
        move_assignable(move_assignable&&) noexcept(true);
        ~move_assignable() noexcept(true);
        move_assignable& operator=(move_assignable const&) noexcept(true);
        move_assignable& operator=(move_assignable&&) noexcept(false);
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("void", [](KT::context& c)->bool{
            constexpr bool value = NT::is_nothrow_move_assignable<void>::value;
            KT::use(value);
            return KT::assert_false(c, "void yields false", value);
        }),
    KT::expect_success("nothrow_move_assignable", [](KT::context& c)->bool{
            constexpr bool value = NT::is_nothrow_move_assignable<nothrow_move_assignable>::value;
            KT::use(value);
            return KT::assert_true(c, "nothrow_move_assignable yields true",
                                   NT::is_nothrow_move_assignable<nothrow_move_assignable>::value)
                && KT::assert_false(c, "nothrow_move_assignable const yields true",
                                   NT::is_nothrow_move_assignable<nothrow_move_assignable const >::value)
                && KT::assert_false(c, "nothrow_move_assignable volatile yields true",
                                   NT::is_nothrow_move_assignable<nothrow_move_assignable volatile>::value)
                && KT::assert_false(c, "nothrow_move_assignable const volatile yields true",
                                   NT::is_nothrow_move_assignable<nothrow_move_assignable const volatile>::value)
                ;
        }),
    KT::expect_success("move_assignable", [](KT::context& c)->bool{
            constexpr bool value = NT::is_nothrow_move_assignable<move_assignable>::value;
            KT::use(value);
            return KT::assert_false(c, "move_assignable yields true",
                                    NT::is_nothrow_move_assignable<move_assignable>::value)
                && KT::assert_false(c, "move_assignable const yields true",
                                    NT::is_nothrow_move_assignable<move_assignable const >::value)
                && KT::assert_false(c, "move_assignable volatile yields true",
                                    NT::is_nothrow_move_assignable<move_assignable volatile>::value)
                && KT::assert_false(c, "move_assignable const volatile yields true",
                                    NT::is_nothrow_move_assignable<move_assignable const volatile>::value)
                ;
        }),
};

static KT::add_tests suite("type_traits::is_nothrow_move_assignable", ::tests);
