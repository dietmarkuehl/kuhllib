// nstd/functional/bind.t.cpp                                         -*-C++-*-
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

#include "nstd/functional/bind.hpp"
#include "nstd/functional/placeholders.hpp"
#include "nstd/functional/reference_wrapper.hpp"
#include "nstd/projection/model_value.hpp"
#include "nstd/type_traits/integral_constant.hpp"
#include "kuhl/test.hpp"
#include <tuple>

namespace NF = nstd::functional;
namespace NFP = NF::placeholders;
namespace NP = nstd::projection;
namespace NT = nstd::type_traits;

namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    template <typename... T>
    auto f(T&&... args) -> std::tuple<T...> {
        return std::tuple<T...>(args...);
    }

    template <typename... T>
    auto g(T&&... args) -> std::tuple<T...> {
        return std::tuple<T...>(args...);
    }
}

// ----------------------------------------------------------------------------

namespace {
    struct foo {
        int mem;
        foo(int mem): mem(mem) {}
        std::tuple<int> mem0() { return std::make_tuple(this->mem); }
        std::tuple<int> omem0() { return std::make_tuple(this->mem); }
        std::tuple<int, int> mem1(int v0) { return std::make_tuple(this->mem, v0); }
        std::tuple<int, int> omem1(int v0) { return std::make_tuple(this->mem, v0); }
        std::tuple<int, int, int> mem2(int v0, int v1) { return std::make_tuple(this->mem, v0, v1); }
        std::tuple<int, int, int> omem2(int v0, int v1) { return std::make_tuple(this->mem, v0, v1); }

        bool operator== (foo const& other) const { return this->mem == other.mem; }
    };

    struct ref_test0 {
        NT::true_type operator()(int& ref, int, int, int value) {
            ref = value;
            return NT::true_type();
        }
        template <typename T>
        NT::false_type operator()(T, int, int, int) {
            return NT::false_type();
        }
    };
    struct ref_test1 {
        NT::true_type operator()(int, int& ref, int, int value) {
            ref = value;
            return NT::true_type();
        }
        template <typename T>
        NT::false_type operator()(int, T, int, int) {
            return NT::false_type();
        }
    };
    struct ref_test2 {
        NT::true_type operator()(int, int, int& ref, int value) {
            ref = value;
            return NT::true_type();
        }
        template <typename T>
        NT::false_type operator()(int, int, T, int) {
            return NT::false_type();
        }
    };

    struct inner {
        int value;
        inner(int value): value(value) {}
        auto operator()(int value) ->int { return this->value + value; }
    };
    struct outer {
        int value;
        outer(int value): value(value) {}
        auto operator()(int value) -> int { return this->value + value; }
    };

    struct function_object {
        auto operator()(int  const& , NP::model_value<long> const& ) const
            -> bool {
            return true;
        }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("prepare", [](KT::context& c)->bool{
            int value{0};
            foo f(0);
            return KT::assert_false(c, "ref_test0", ref_test0()(0, 1, 2, 3)())
                && KT::assert_true(c, "ref_test0", ref_test0()(value, 1, 2, 3)())
                && KT::assert_false(c, "ref_test1", ref_test1()(0, 1, 2, 3)())
                && KT::assert_true(c, "ref_test1", ref_test1()(0, value, 2, 3)())
                && KT::assert_false(c, "ref_test2", ref_test2()(0, 1, 2, 3)())
                && KT::assert_true(c, "ref_test2", ref_test2()(0, 1, value, 3)())
                && KT::assert_true(c, "f.mem1(10)", std::make_tuple(0, 10) == f.mem1(10))
                && KT::assert_true(c, "f.omem1(10)", std::make_tuple(0, 10) == f.omem1(10))
                && KT::assert_true(c, "f.mem2(10, 11)", std::make_tuple(0, 10, 11) == f.mem2(10, 11))
                && KT::assert_true(c, "f.omem2(10, 11)", std::make_tuple(0, 10, 11) == f.omem2(10, 11))
                ;
        }),
    KT::expect_success("bind predicate", [](KT::context& )->bool{
            namespace NF = nstd::functional;
            namespace NFP = NF::placeholders;
            auto fpred(NF::bind(function_object(), NFP::_1, NP::model_value<long>(3l)));
            return fpred(1);
        }),
    KT::expect_success("ref_test", [](KT::context& c)->bool{
            int value0{17};
            int value1{18};
            int value2{19};
            return true
#ifndef KUHLLIB_INTEL
                && KT::assert_static_true<NF::is_bind_expression<decltype(NF::bind(ref_test0(), 0, 0, 0, 4))> >(c,
                                                                   "is binder bind(ref_test0(), 0, 0, 0, 4)")
                && KT::assert_static_true<NF::is_bind_expression<decltype(NF::bind(ref_test0(), value0, 0, 0, 4))> >(c,
                                                                   "is binder bind(ref_test0(), value0, 0, 0, 4)")
                && KT::assert_static_true<NF::is_bind_expression<decltype(NF::bind(ref_test0(), NF::ref(value0), 0, 0, 4))> >(c,
                                                                   "is binder bind(ref_test0(), NF::ref(value0), 0, 0, 4)")
                && KT::assert_false(c, "bind(., ., ., ., 4) type",
                                    NF::bind(ref_test0(), 0, 0, 0, 4)())
                && KT::assert_false(c, "bind(., value0, ., ., 4) type",
                                   NF::bind(ref_test0(), value0, 0, 0, 4)())
                && KT::assert_true(c, "bind(., ref, ., ., 4) type",
                                   NF::bind(ref_test0(), NF::ref(value0), 0, 0, 4)())
                && KT::assert_equal(c, "bind(., ref, . ., 4) result", 4, value0)
                && KT::assert_false(c, "bind(., ., ., ., 5) type",
                                   NF::bind(ref_test1(), value1, 0, 0, 5)())
                && KT::assert_true(c, "bind(., ., ref, ., 5) type",
                                   NF::bind(ref_test1(), 0, NF::ref(value1), 0, 5)())
                && KT::assert_equal(c, "bind(., ., ref, ., 5) result", 5, value1)
                && KT::assert_false(c, "bind(., ., ., ., 6) type",
                                    NF::bind(ref_test2(), 0, 0, value2, 6)())
                && KT::assert_true(c, "bind(., .,, ., ref 6) type",
                                   NF::bind(ref_test2(), 0, 0, NF::ref(value2), 6)())
                && KT::assert_equal(c, "bind(., .,, ., ref 6) result", 6, value2)
#endif
                ;
        }),
    KT::expect_success("componsition", [](KT::context& c)->bool {
            return KT::assert_equal(c, "bind(inner(1), 10)()", 11, NF::bind(inner(1), 10)(2000, 3000, 4000))
                && KT::assert_equal(c, "bind(inner(1), _1)()", 2001, NF::bind(inner(1), NFP::_1)(2000, 3000, 4000))
                && KT::assert_equal(c, "bind(inner(1), _2)()", 3001, NF::bind(inner(1), NFP::_2)(2000, 3000, 4000))
                && KT::assert_equal(c, "bind(inner(1), _3)()", 4001, NF::bind(inner(1), NFP::_3)(2000, 3000, 4000))
                && KT::assert_equal(c, "bind(outer(10), 100)()", 110, NF::bind(outer(10), 100)(2000, 3000, 4000))
                && KT::assert_equal(c, "bind(outer(10), bind(inner(1), 100))()", 111,
                                    NF::bind(outer(10), NF::bind(inner(1), 100))(2000, 3000, 4000))
                && KT::assert_equal(c, "bind(outer(10), bind(inner(1), _1))()", 2011,
                                    NF::bind(outer(10), NF::bind(inner(1), NFP::_1))(2000, 3000, 4000))
                && KT::assert_equal(c, "bind(outer(10), bind(inner(1), _2))()", 3011,
                                    NF::bind(outer(10), NF::bind(inner(1), NFP::_2))(2000, 3000, 4000))
                && KT::assert_equal(c, "bind(outer(10), bind(inner(1), _3))()", 4011,
                                    NF::bind(outer(10), NF::bind(inner(1), NFP::_3))(2000, 3000, 4000))
                ;
        }),
    KT::expect_success("bind(f<>)", [](KT::context& c)->bool{
            return true
#ifndef KUHLLIB_INTEL
                && KT::assert_type<std::tuple<>, decltype(NF::bind(f<>)())>(c, "type 0")
                && KT::assert_true(c, "call", std::tuple<>() == NF::bind(f<>)())
                && KT::assert_true(c, "equal f<>/f<>", NF::bind(f<>) == NF::bind(f<>))
                && KT::assert_false(c, "equal f<>/g<>", NF::bind(f<>) == NF::bind(g<>))
                && KT::assert_false(c, "not equal f<>/f<>", NF::bind(f<>) != NF::bind(f<>))
                && KT::assert_true(c, "not equal f<>/g<>", NF::bind(f<>) != NF::bind(g<>))
#endif
                ;
        }),
    KT::expect_success("bind(f<int>)", [](KT::context& c)->bool{
            return KT::assert_type<std::tuple<int>, decltype(NF::bind(f<int>, 17)())>(c, "type 1")
                && KT::assert_true(c, "call(17)()", std::tuple<int>(17) == NF::bind(f<int>, 17)())
                && KT::assert_true(c, "call(17)(1, 2, 3, 4)", std::tuple<int>(17) == NF::bind(f<int>, 17)(1, 2, 3, 4))
                && KT::assert_true(c, "call(_1)(21, 22, 23, 24)", std::tuple<int>(21) == NF::bind(f<int>, NFP::_1)(21, 22, 23, 24))
                && KT::assert_true(c, "call(_2)(21, 22, 23, 24)", std::tuple<int>(22) == NF::bind(f<int>, NFP::_2)(21, 22, 23, 24))
                && KT::assert_true(c, "call(_3)(21, 22, 23, 24)", std::tuple<int>(23) == NF::bind(f<int>, NFP::_3)(21, 22, 23, 24))
                && KT::assert_true(c, "call(_4)(21, 22, 23, 24)", std::tuple<int>(24) == NF::bind(f<int>, NFP::_4)(21, 22, 23, 24))
                && KT::assert_true(c, "equal f<int>, 17/f<int>, 17", NF::bind(f<int>, 17) == NF::bind(f<int>, 17))
                && KT::assert_false(c, "equal f<int>, 17/f<int>, 18", NF::bind(f<int>, 17) == NF::bind(f<int>, 18))
                && KT::assert_false(c, "equal f<int>, 17/g<int>, 17", NF::bind(f<int>, 17) == NF::bind(g<int>, 17))
                && KT::assert_false(c, "not equal f<int>, 17/f<int>, 17", NF::bind(f<int>, 17) != NF::bind(f<int>, 17))
                && KT::assert_true(c, "not equal f<int>, 17/f<int>, 18", NF::bind(f<int>, 17) != NF::bind(f<int>, 18))
                && KT::assert_true(c, "not equal f<int>, 17/g<int>, 17", NF::bind(f<int>, 17) != NF::bind(g<int>, 17))
                ;
        }),
    KT::expect_success("bind(foo::mem0, ..)", [](KT::context& c)->bool{
            foo f0(19);
            foo f1(19);
            foo f2(20);
            return KT::assert_true(c, "bind(foo::mem0, f0)", std::make_tuple(19) == NF::bind(&foo::mem0, f0)())
                && KT::assert_true(c, "equal bind(foo::mem0, f0/f1", NF::bind(&foo::mem0, f0) == NF::bind(&foo::mem0, f1))
                && KT::assert_false(c, "equal bind(foo::mem0, f0/f2)", NF::bind(&foo::mem0, f0) == NF::bind(&foo::mem0, f2))
                && KT::assert_false(c, "equal bind(foo::omem0, f0/f1)", NF::bind(&foo::mem0, f0) == NF::bind(&foo::omem0, f1))
                ; 
        }),
    KT::expect_success("bind(foo::mem1, ..)", [](KT::context& c)->bool{
            foo f0(19);
            foo f1(19);
            foo f2(20);
            return true
#ifndef KUHLLIB_INTEL
                && KT::assert_true(c, "bind(foo::mem0, f0, 21)", std::make_tuple(19, 21) == NF::bind(&foo::mem1, f0, 21)())
                && KT::assert_true(c, "equal bind(foo::mem0, f0/f1", NF::bind(&foo::mem1, f0, 21) == NF::bind(&foo::mem1, f1, 21))
                && KT::assert_false(c, "equal bind(foo::mem0, f0/f2)", NF::bind(&foo::mem1, f0, 21) == NF::bind(&foo::mem1, f2, 21))
                && KT::assert_false(c, "equal bind(foo::omem0, f0/f1)", NF::bind(&foo::mem1, f0, 21) == NF::bind(&foo::omem1, f0, 21))
#endif
                ; 
        }),
};

static KT::add_tests suite("functional::bind", ::tests);
