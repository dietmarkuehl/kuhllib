// nstd/utility/pair.t.cpp                                            -*-C++-*-
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

#include "nstd/utility/pair.hpp"
#include "nstd/type_traits/declval.hpp"
#include "kuhl/test.hpp"

namespace NU = nstd::utility;
namespace NT = nstd::type_traits;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    struct movable {
        int value;
        explicit movable(int value): value(value) {}
        movable(movable&& other): value(other.value) { other.value = -1; }
        movable(movable const&) = delete;
        movable& operator=(movable&& other) {
            this->value = other.value;
            other.value = -1;
            return *this;
        }
        movable& operator=(movable&) = delete;
    };
    struct nothrow_movable {
        int value;
        explicit nothrow_movable(int value): value(value) {}
        nothrow_movable(nothrow_movable&& other) noexcept(true): value(other.value) { other.value = -1; }
        nothrow_movable(nothrow_movable const&) = delete;
        auto operator=(nothrow_movable&& other) noexcept(true) -> nothrow_movable& {
            this->value = other.value;
            other.value = -1;
            return *this;
        }
        auto operator=(nothrow_movable&) -> nothrow_movable& = delete;
    };
    struct copyable {
        int value;
        explicit copyable(int value): value(value) {}
        copyable(copyable const& other) noexcept(false): value(other.value) {}
        copyable& operator=(copyable const& other) noexcept(false) {
            this->value = other.value;
            return *this;
        }
    };
    struct nothrow_copyable {
        int value;
        explicit nothrow_copyable(int value): value(value) {}
        nothrow_copyable(nothrow_copyable const&) noexcept(true) = default;
        nothrow_copyable& operator=(nothrow_copyable const&) noexcept(true) = default;
    };
    struct foo {
        int value;
        foo(): value(17) {}
        explicit foo(int value): value(value) {}
        explicit foo(movable&& other): value(other.value) {}
        explicit foo(copyable const& other): value(other.value) {}
        foo& operator= (copyable const& other) { this->value = other.value; return *this; }
        foo& operator= (movable&& other) { this->value = other.value; return *this; }
    };
    struct bar {
        int value;
        bar(): value(43) {}
        explicit bar(int value): value(value) {}
        explicit bar(movable&& other): value(other.value) {}
        explicit bar(copyable const& other): value(other.value) {}
        bar& operator= (copyable const& other) { this->value = other.value; return *this; }
        bar& operator= (movable&& other) { this->value = other.value; return *this; }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("typedefs", [](KT::context&c) -> bool {
            return KT::assert_type<foo, NU::pair<foo, bar>::first_type>(c, "first_type")
                && KT::assert_type<bar, NU::pair<foo, bar>::second_type>(c, "second_type")
                ;
        }),
    KT::expect_success("public data members", [](KT::context& c)->bool{
            return KT::assert_type<foo, decltype(NT::declval<NU::pair<foo, bar>>().first)>(c, "type of first")
                && KT::assert_type<bar, decltype(NT::declval<NU::pair<foo, bar>>().second)>(c, "type of second")
                ; 
        }),
    KT::expect_success("default constructor", [](KT::context& c)->bool{
            constexpr NU::pair<int, bool> p0;
            NU::pair<foo, bar>            p1;
            return KT::assert_equal(c, "value of p0.first",   0, p0.first)
                && KT::assert_equal(c, "value of p0.second",  0, p0.second)
                && KT::assert_equal(c, "value of p1.first",  17, p1.first.value)
                && KT::assert_equal(c, "value of p1.second", 43, p1.second.value)
                ;
        }),
    KT::expect_success("member constructor (const&)", [](KT::context& c)->bool{
            constexpr int  i{ 17 };
            constexpr bool t{ true };
            constexpr NU::pair<int, bool> p0{ i, t };
            foo const f(27);
            bar const b(41);
            NU::pair<foo, bar> p1{ f, b };
            return KT::assert_equal(c, "value of p0.first",  17, p0.first)
                && KT::assert_true (c, "value of p0.second", p0.second)
                && KT::assert_equal(c, "value of p1.first",  27, p1.first.value)
                && KT::assert_equal(c, "value of p1.second", 41, p1.second.value)
                ;
        }),
    KT::expect_success("member constructor (template)", [](KT::context& c)->bool{
#ifndef KUHLLIB_INTEL
            constexpr
#else
            const
#endif
                NU::pair<int, bool> c0( 17, true );
            NU::pair<foo, bar> p0{ foo(23), bar(24) };
            NU::pair<foo, bar> p1{ movable(25), bar(26) };
            NU::pair<foo, bar> p2{ foo(27), movable(28) };
            NU::pair<foo, bar> p3{ movable(29), movable(30) };
            return KT::assert_equal(c, "value of c0.first",  17, c0.first)
                && KT::assert_true (c, "value of c0.second", c0.second)
                && KT::assert_equal(c, "value of p0.first",  23, p0.first.value)
                && KT::assert_equal(c, "value of p0.second", 24, p0.second.value)
                && KT::assert_equal(c, "value of p1.first",  25, p1.first.value)
                && KT::assert_equal(c, "value of p1.second", 26, p1.second.value)
                && KT::assert_equal(c, "value of p2.first",  27, p2.first.value)
                && KT::assert_equal(c, "value of p2.second", 28, p2.second.value)
                && KT::assert_equal(c, "value of p3.first",  29, p3.first.value)
                && KT::assert_equal(c, "value of p3.second", 30, p3.second.value)
                ;
        }),
    KT::expect_success("move constructor", [](KT::context& c)->bool{
            NU::pair<foo, bar>       p{ NU::pair<foo, bar>{ foo(23), bar(45) } };
            return KT::assert_equal(c, "value of p.first",  23, p.first.value)
                && KT::assert_equal(c, "value of p.second", 45, p.second.value)
                ;
        }),
    KT::expect_success("copy constructor", [](KT::context& c)->bool{
            NU::pair<foo, bar>       p0{ foo(23), bar(45) };
            NU::pair<foo, bar> const c0{ foo(24), bar(44) };
            NU::pair<foo, bar>       p1{ p0 };
            NU::pair<foo, bar>       c1{ c0 };
            return KT::assert_equal(c, "value of p0.first",  23, p0.first.value)
                && KT::assert_equal(c, "value of p0.second", 45, p0.second.value)
                && KT::assert_equal(c, "value of p1.first",  23, p1.first.value)
                && KT::assert_equal(c, "value of p1.second", 45, p1.second.value)
                && KT::assert_equal(c, "value of c0.first",  24, c0.first.value)
                && KT::assert_equal(c, "value of c0.second", 44, c0.second.value)
                && KT::assert_equal(c, "value of c1.first",  24, c1.first.value)
                && KT::assert_equal(c, "value of c1.second", 44, c1.second.value)
                ;
        }),
    KT::expect_success("conversion lvalue constructor", [](KT::context& c)->bool{
#ifndef KUHLLIB_INTEL
            constexpr
#else
            const
#endif
                NU::pair<int, bool>  ce0{ 23, true };
#ifndef KUHLLIB_INTEL
            constexpr
#else
            const
#endif
                NU::pair<long, bool> ce1{ ce0 };
            NU::pair<copyable, bar>      const p0{ copyable{41}, bar{42} };
            NU::pair<foo, copyable>      const p1{ foo{43},    copyable{44} };
            NU::pair<copyable, copyable> const p2{ copyable{45}, copyable{46} };
            NU::pair<foo, bar> c0{ p0 };
            NU::pair<foo, bar> c1{ p1 };
            NU::pair<foo, bar> c2{ p2 };
            return KT::assert_equal(c, "value of ce1.first",  23, ce1.first)
                && KT::assert_true (c, "value of ce1.second", ce1.second)
                && KT::assert_equal(c, "value of c0.first",  41, c0.first.value)
                && KT::assert_equal(c, "value of c0.second", 42, c0.second.value)
                && KT::assert_equal(c, "value of c1.first",  43, c1.first.value)
                && KT::assert_equal(c, "value of c1.second", 44, c1.second.value)
                && KT::assert_equal(c, "value of c2.first",  45, c2.first.value)
                && KT::assert_equal(c, "value of c2.second", 46, c2.second.value)
                ;
        }),
    KT::expect_success("conversion rvalue constructor", [](KT::context& c)->bool{
#ifndef KUHLLIB_INTEL
            constexpr
#else
            const
#endif
                NU::pair<long, bool> ce{ NU::pair<int, bool>{ 23, true } };
            NU::pair<movable, bar> p0{ NU::pair<movable, movable>(1, 2) };
            NU::pair<foo, movable> p1{ NU::pair<movable, movable>(3, 4) };
            return KT::assert_equal(c, "value of ce.first",  23, ce.first)
                && KT::assert_true (c, "value of ce.second", ce.second)
                && KT::assert_equal(c, "value of p0.first",  1, p0.first.value)
                && KT::assert_equal(c, "value of p0.second", 2, p0.second.value)
                && KT::assert_equal(c, "value of p1.first",  3, p1.first.value)
                && KT::assert_equal(c, "value of p1.second", 4, p1.second.value)
                ;
        }),
    KT::expect_success("copy assignment", [](KT::context& c)->bool{
            NU::pair<foo, bar> p0{ 1, 2 };
            NU::pair<foo, bar> p1{ 3, 4 };
            p1 = p0;
            return KT::assert_type<NU::pair<foo, bar>&, decltype(p0 = p1)>(c, "return type")
                && KT::assert_equal(c, "value of p1.first",  1, p1.first.value)
                && KT::assert_equal(c, "value of p1.second", 2, p1.second.value)
                ;
        }),
    KT::expect_success("copy assignment (template)", [](KT::context& c)->bool{
            NU::pair<copyable, bar>      p0{ 1, 2 };
            NU::pair<foo,      copyable> p1{ 3, 4 };
            NU::pair<copyable, copyable> p2{ 5, 6 };
            NU::pair<foo, bar> c0{  7,  8 };
            NU::pair<foo, bar> c1{  9, 10 };
            NU::pair<foo, bar> c2{ 11, 12 };
            c0 = p0;
            c1 = p1;
            c2 = p2;
            return KT::assert_type<NU::pair<foo, bar>&, decltype(c0 = p0)>(c, "return type")
                && KT::assert_equal(c, "value of c0.first",  1, c0.first.value)
                && KT::assert_equal(c, "value of c0.second", 2, c0.second.value)
                && KT::assert_equal(c, "value of c1.first",  3, c1.first.value)
                && KT::assert_equal(c, "value of c1.second", 4, c1.second.value)
                && KT::assert_equal(c, "value of c2.first",  5, c2.first.value)
                && KT::assert_equal(c, "value of c2.second", 6, c2.second.value)
                ;
        }),
    KT::expect_success("move assignment", [](KT::context& c)->bool{
            NU::pair<movable, bar> p0( movable{ 1 }, 2 );
            p0 = NU::pair<movable, bar>{ movable{ 3 }, 4 };
            return KT::assert_type<NU::pair<movable, bar>&, decltype(p0 = NU::pair<movable, bar>{ 3, 4 })>(c, "return type")
                && KT::assert_equal(c, "value of p0.first",  3, p0.first.value)
                && KT::assert_equal(c, "value of p0.second", 4, p0.second.value)
                ;
        }),
    KT::expect_success("move assignment (template)", [](KT::context& c)->bool{
            NU::pair<movable, bar>     p0( movable{ 1 }, 2 );
            NU::pair<foo,     movable> p1( 3,            movable{ 4 });
            NU::pair<movable, movable> p2( movable{ 5 }, movable{ 6 });

            p0 = NU::pair<movable, movable>{ movable{  7 }, movable{ 8 } };
            p1 = NU::pair<movable, movable>{ movable{  9 }, movable{ 10 } };
            p2 = NU::pair<movable, movable>{ movable{ 11 }, movable{ 12 } };

            return KT::assert_type<NU::pair<movable, bar>&, decltype(p0 = NU::pair<movable, movable>{ 3, 4 })>(c, "return type")
                && KT::assert_equal(c, "value of p0.first",   7, p0.first.value)
                && KT::assert_equal(c, "value of p0.second",  8, p0.second.value)
                && KT::assert_equal(c, "value of p1.first",   9, p1.first.value)
                && KT::assert_equal(c, "value of p1.second", 10, p1.second.value)
                && KT::assert_equal(c, "value of p2.first",  11, p2.first.value)
                && KT::assert_equal(c, "value of p2.second", 12, p2.second.value)
                ;
        }),
    KT::expect_success("swap() int, int", [](KT::context& c)->bool{
            NU::pair<int, int> p0{ 1, 2 };
            NU::pair<int, int> p1{ 3, 4 };
            p0.swap(p1);
            return KT::assert_true(c, "noexcept", noexcept(p0.swap(p1)))
                && KT::assert_equal(c, "p0.first",  3, p0.first)
                && KT::assert_equal(c, "p0.second", 4, p0.second)
                && KT::assert_equal(c, "p1.first",  1, p1.first)
                && KT::assert_equal(c, "p1.second", 2, p1.second)
                ;
        }),
    KT::expect_success("swap() movable, nothrow_movable", [](KT::context& c)->bool{
            NU::pair<movable, nothrow_movable> p0{ 1, 2 };
            NU::pair<movable, nothrow_movable> p1{ 3, 4 };
            p0.swap(p1);
            return KT::assert_false(c, "noexcept", noexcept(p0.swap(p1)))
                && KT::assert_equal(c, "p0.first",  3, p0.first.value)
                && KT::assert_equal(c, "p0.second", 4, p0.second.value)
                && KT::assert_equal(c, "p1.first",  1, p1.first.value)
                && KT::assert_equal(c, "p1.second", 2, p1.second.value)
                ;
        }),
    KT::expect_success("swap() movable, nothrow_movable", [](KT::context& c)->bool{
            NU::pair<nothrow_movable, movable> p0{ 1, 2 };
            NU::pair<nothrow_movable, movable> p1{ 3, 4 };
            p0.swap(p1);
            return KT::assert_false(c, "noexcept", noexcept(p0.swap(p1)))
                && KT::assert_equal(c, "p0.first",  3, p0.first.value)
                && KT::assert_equal(c, "p0.second", 4, p0.second.value)
                && KT::assert_equal(c, "p1.first",  1, p1.first.value)
                && KT::assert_equal(c, "p1.second", 2, p1.second.value)
                ;
        }),
    KT::expect_success("swap() nothrow_movable, nothrow_movable", [](KT::context& c)->bool{
            NU::pair<nothrow_movable, nothrow_movable> p0{ 1, 2 };
            NU::pair<nothrow_movable, nothrow_movable> p1{ 3, 4 };
            p0.swap(p1);
            return KT::assert_true(c, "noexcept", noexcept(p0.swap(p1)))
                && KT::assert_equal(c, "p0.first",  3, p0.first.value)
                && KT::assert_equal(c, "p0.second", 4, p0.second.value)
                && KT::assert_equal(c, "p1.first",  1, p1.first.value)
                && KT::assert_equal(c, "p1.second", 2, p1.second.value)
                ;
        }),
    KT::expect_success("swap() copyable, nothrow_copyable", [](KT::context& c)->bool{
            NU::pair<copyable, nothrow_copyable> p0{ 1, 2 };
            NU::pair<copyable, nothrow_copyable> p1{ 3, 4 };
            p0.swap(p1);
            return KT::assert_false(c, "noexcept", noexcept(p0.swap(p1)))
                && KT::assert_equal(c, "p0.first",  3, p0.first.value)
                && KT::assert_equal(c, "p0.second", 4, p0.second.value)
                && KT::assert_equal(c, "p1.first",  1, p1.first.value)
                && KT::assert_equal(c, "p1.second", 2, p1.second.value)
                ;
        }),
    KT::expect_success("swap() copyable, nothrow_copyable", [](KT::context& c)->bool{
            NU::pair<nothrow_copyable, copyable> p0{ 1, 2 };
            NU::pair<nothrow_copyable, copyable> p1{ 3, 4 };
            p0.swap(p1);
            return KT::assert_false(c, "noexcept", noexcept(p0.swap(p1)))
                && KT::assert_equal(c, "p0.first",  3, p0.first.value)
                && KT::assert_equal(c, "p0.second", 4, p0.second.value)
                && KT::assert_equal(c, "p1.first",  1, p1.first.value)
                && KT::assert_equal(c, "p1.second", 2, p1.second.value)
                ;
        }),
    KT::expect_success("swap() nothrow_copyable, nothrow_copyable", [](KT::context& c)->bool{
            NU::pair<nothrow_copyable, nothrow_copyable> p0{ 1, 2 };
            NU::pair<nothrow_copyable, nothrow_copyable> p1{ 3, 4 };
            p0.swap(p1);
            return KT::assert_true(c, "noexcept", noexcept(p0.swap(p1)))
                && KT::assert_equal(c, "p0.first",  3, p0.first.value)
                && KT::assert_equal(c, "p0.second", 4, p0.second.value)
                && KT::assert_equal(c, "p1.first",  1, p1.first.value)
                && KT::assert_equal(c, "p1.second", 2, p1.second.value)
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("utility::pair", ac, av, ::tests);
}
