// nstd/functional/equal_to.t.cpp                                     -*-C++-*-
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

#include "nstd/functional/equal_to.hpp"
#include "kuhl/test.hpp"

namespace NF = nstd::functional;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    struct foo {
        int value;
        foo(int value): value(value) {}
        foo(foo const&) = delete;
        foo(foo&&) = delete;
        
        int operator== (foo const& a) const { return this->value == a.value; }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("int", [](KT::context& c)->bool{
            constexpr NF::equal_to<int> predicate{};
            constexpr bool value{predicate(17, 17)};
            KT::use(value);
            return KT::assert_type<int, NF::equal_to<int>::first_argument_type>(c, "first argument type")
                && KT::assert_type<int, NF::equal_to<int>::second_argument_type>(c, "second argument type")
                && KT::assert_type<bool, NF::equal_to<int>::result_type>(c, "result type")
                && KT::assert_type<bool, decltype(predicate(17, 17))>(c, "type")
                && KT::assert_true(c, "equal", predicate(17, 17))
                && KT::assert_false(c, "not equal", predicate(17, 18))
                && KT::assert_false(c, "not equal", predicate(18, 17))
                ;
        }),
    KT::expect_success("foo", [](KT::context& c)->bool{
            constexpr NF::equal_to<foo> predicate{};
            foo const f0{0};
            foo const f1{1};
            return KT::assert_type<foo, NF::equal_to<foo>::first_argument_type>(c, "first argument type")
                && KT::assert_type<foo, NF::equal_to<foo>::second_argument_type>(c, "second argument type")
                && KT::assert_type<bool, NF::equal_to<foo>::result_type>(c, "result type")
                && KT::assert_type<bool, decltype(predicate(f0, f0))>(c, "type")
                && KT::assert_true(c, "equal", predicate(f0, f0))
                && KT::assert_false(c, "not equal", predicate(f0, f1))
                && KT::assert_false(c, "not equal", predicate(f1, f0))
                ;
        }),
    KT::expect_success("void/int", [](KT::context& c)->bool{
            constexpr NF::equal_to<> predicate{};
#ifndef KUHLLIB_INTEL
            constexpr
#endif
                bool           value{predicate(17, 17)};
            KT::use(value);
            return KT::assert_type<bool, decltype(predicate(17, 17))>(c, "type")
                && KT::assert_true(c, "equal", predicate(17, 17))
                && KT::assert_false(c, "not equal", predicate(17, 18))
                && KT::assert_false(c, "not equal", predicate(18, 17))
                ;
        }),
    KT::expect_success("foo", [](KT::context& c)->bool{
            constexpr NF::equal_to<> predicate{};
            foo const f0{0};
            foo const f1{1};
            return KT::assert_type<int, decltype(predicate(f0, f0))>(c, "type")
                && KT::assert_true(c, "equal", predicate(f0, f0))
                && KT::assert_false(c, "not equal", predicate(f0, f1))
                && KT::assert_false(c, "not equal", predicate(f1, f0))
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("functional::equal_to", ac, av, ::tests);
}
