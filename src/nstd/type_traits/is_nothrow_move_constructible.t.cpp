// nstd/type_traits/is_nothrow_move_constructible.t.cpp               -*-C++-*- // ----------------------------------------------------------------------------
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

#include "nstd/type_traits/is_nothrow_move_constructible.hpp"
#include "kuhl/test.hpp"

namespace NT = nstd::type_traits;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    struct movable {
        movable(movable&&) noexcept(false) {};
        movable(movable const&) = delete;
    };
    struct copyable {
        copyable(copyable&&) = delete;
        copyable(copyable const&) noexcept(false) {};
    };
    struct nothrow_movable {
        nothrow_movable(nothrow_movable&&) noexcept(true) = default;
        nothrow_movable(nothrow_movable const&) = delete;
    };
    struct nothrow_copyable {
        nothrow_copyable(nothrow_copyable&&) = delete;
        nothrow_copyable(nothrow_copyable const&) noexcept(true) = default;
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("void", [](KT::context& c)->bool{
            return KT::assert_false(c, "void()", NT::is_nothrow_move_constructible<void>::value)
                ;
        }),
    KT::expect_success("int", [](KT::context& c)->bool{
            return KT::assert_true(c, "int", NT::is_nothrow_move_constructible<int>::value)
                && KT::assert_true(c, "int const", NT::is_nothrow_move_constructible<int const>::value)
                && KT::assert_true(c, "int&", NT::is_nothrow_move_constructible<int&>::value)
                ;
        }),
    KT::expect_success("movable", [](KT::context& c)->bool{
            return KT::assert_false(c, "movable", NT::is_nothrow_move_constructible<movable>::value)
                && KT::assert_true(c, "movable const&", NT::is_nothrow_move_constructible<movable const&>::value)
                && KT::assert_true(c, "movable&&", NT::is_nothrow_move_constructible<movable&&>::value)
                ;
        }),
    KT::expect_success("copyable", [](KT::context& c)->bool{
            return KT::assert_false(c, "copyable", NT::is_nothrow_move_constructible<copyable>::value)
                && KT::assert_false(c, "copyable const", NT::is_nothrow_move_constructible<copyable const>::value)
                && KT::assert_true(c, "copyable&&", NT::is_nothrow_move_constructible<copyable&&>::value)
                ;
        }),
    KT::expect_success("nothrow_movable", [](KT::context& c)->bool{
            return KT::assert_true(c, "nothrow_movable", NT::is_nothrow_move_constructible<nothrow_movable>::value)
                && KT::assert_true(c, "nothrow_movable const&", NT::is_nothrow_move_constructible<nothrow_movable const&>::value)
                && KT::assert_true(c, "nothrow_movable&&", NT::is_nothrow_move_constructible<nothrow_movable&&>::value)
                ;
        }),
    KT::expect_success("nothrow_copyable", [](KT::context& c)->bool{
            return KT::assert_false(c, "nothrow_copyable", NT::is_nothrow_move_constructible<nothrow_copyable>::value)
                && KT::assert_true(c, "nothrow_copyable const&", NT::is_nothrow_move_constructible<nothrow_copyable const&>::value)
                && KT::assert_true(c, "nothrow_copyable&&", NT::is_nothrow_move_constructible<nothrow_copyable&&>::value)
                ;
        }),
};

static KT::add_tests suite("type_traits::is_nothrow_move_constructible", ::tests);
