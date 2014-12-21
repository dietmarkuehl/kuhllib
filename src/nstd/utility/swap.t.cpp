// nstd/utility/swap.t.cpp                                            -*-C++-*-
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

#include "nstd/utility/swap.hpp"
#include "kuhl/test.hpp"

namespace NU = nstd::utility;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    struct movable {
        int value;
        movable(int value): value(value) {}
        movable(movable&& other): value(other.value) { other.value = -1; }
        movable(movable const&) = delete;
        auto operator= (movable&& other) -> void {
            value = other.value;
            other.value = -1;
        }
        auto operator= (movable const&) -> void = delete;
    };
    struct nothrow_movable {
        int value;
        nothrow_movable(int value): value(value) {}
        nothrow_movable(nothrow_movable&& other) noexcept(true): value(other.value) { other.value = -1; }
        nothrow_movable(nothrow_movable const&) = delete;
        auto operator= (nothrow_movable&& other) noexcept(true) -> void {
            value = other.value;
            other.value = -1;
        }
        auto operator= (nothrow_movable const&) -> void = delete;
    };
    struct copyable {
        int value;
        copyable(int value): value(value) {}
        copyable(copyable const& other): value(other.value) { }
        auto operator= (copyable const& other) -> void {
            value = other.value;
        }
    };
    struct nothrow_copyable {
        int value;
        nothrow_copyable(int value): value(value) {}
        nothrow_copyable(nothrow_copyable const& other) noexcept(true): value(other.value) {}
        auto operator= (nothrow_copyable const& other) noexcept(true) -> void {
            value = other.value;
        }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("int", [](KT::context& c)->bool{
            int a{17}, b{43};
            NU::swap(a, b);
            return KT::assert_true(c, "noexcept", noexcept(NU::swap(a, b)))
                && KT::assert_equal(c, "a", 43, a)
                && KT::assert_equal(c, "b", 17, b)
                ;
        }),
    KT::expect_success("movable", [](KT::context& c)->bool{
            movable a{17}, b{43};
            NU::swap(a, b);
            return KT::assert_false(c, "noexcept", noexcept(NU::swap(a, b)))
                && KT::assert_equal(c, "a", 43, a.value)
                && KT::assert_equal(c, "b", 17, b.value)
                ;
        }),
    KT::expect_success("nothrow_movable", [](KT::context& c)->bool{
            nothrow_movable a{17}, b{43};
            NU::swap(a, b);
            return KT::assert_true(c, "noexcept", noexcept(NU::swap(a, b)))
                && KT::assert_equal(c, "a", 43, a.value)
                && KT::assert_equal(c, "b", 17, b.value)
                ;
        }),
    KT::expect_success("copyable", [](KT::context& c)->bool{
            copyable a{17}, b{43};
            NU::swap(a, b);
            return KT::assert_false(c, "noexcept", noexcept(NU::swap(a, b)))
                && KT::assert_equal(c, "a", 43, a.value)
                && KT::assert_equal(c, "b", 17, b.value)
                ;
        }),
    KT::expect_success("nothrow_copyable", [](KT::context& c)->bool{
            nothrow_copyable a{17}, b{43};
            NU::swap(a, b);
            return KT::assert_true(c, "noexcept", noexcept(NU::swap(a, b)))
                && KT::assert_equal(c, "a", 43, a.value)
                && KT::assert_equal(c, "b", 17, b.value)
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("utility::swap", ac, av, ::tests);
}
