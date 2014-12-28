// nstd/functional/mem_fn.t.cpp                                       -*-C++-*-
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

#include "nstd/functional/mem_fn.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"
#include <tuple>

namespace NF = nstd::functional;
namespace NU = nstd::utility;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct foo {
        int mem;
        int omem;
        foo(int mem): mem(mem) {}
        std::tuple<int> mem0() { return std::make_tuple(this->mem); }
        std::tuple<int> cmem0() const { return std::make_tuple(this->mem); }
        std::tuple<int> vmem0() volatile { return std::make_tuple(this->mem); }
        std::tuple<int> cvmem0() const volatile { return std::make_tuple(this->mem); }
        std::tuple<int> omem0() { return std::make_tuple(this->mem); }
        std::tuple<int, int> mem1(int v0) { return std::make_tuple(this->mem, v0); }
        std::tuple<int, int> cmem1(int v0) const { return std::make_tuple(this->mem, v0); }
        std::tuple<int, int> vmem1(int v0) volatile { return std::make_tuple(this->mem, v0); }
        std::tuple<int, int> cvmem1(int v0) const volatile { return std::make_tuple(this->mem, v0); }
        std::tuple<int, int> omem1(int v0) { return std::make_tuple(this->mem, v0); }
        std::tuple<int, int, int> mem2(int v0, int v1) { return std::make_tuple(this->mem, v0, v1); }
        std::tuple<int, int, int> omem2(int v0, int v1) { return std::make_tuple(this->mem, v0, v1); }
    };

    template <typename T>
    struct smart {
        T object;
        template <typename... A>
        smart(A...a): object(a...) {}
        auto operator*() -> T& { return this->object; }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("prepare", [](KT::context& c)->bool{
            foo f(18);
            return KT::assert_true(c, "cmem0", std::make_tuple(18) == f.cmem0())
                && KT::assert_true(c, "vmem0", std::make_tuple(18) == f.vmem0())
                && KT::assert_true(c, "cvmem0", std::make_tuple(18) == f.cvmem0())
                && KT::assert_true(c, "cmem1", std::make_tuple(18, 42) == f.cmem1(42))
                && KT::assert_true(c, "vmem1", std::make_tuple(18, 42) == f.vmem1(42))
                && KT::assert_true(c, "cvmem1", std::make_tuple(18, 42) == f.cvmem1(42))
                ;
        }),
    KT::expect_success("mem_fn(mem)", [](KT::context& c)->bool{
            foo       f(18);
            foo const fc(19);
            return KT::assert_type<int&, decltype(NF::mem_fn(&foo::mem)(f))>(c, "type")
                && KT::assert_type<int const&, decltype(NF::mem_fn(&foo::mem)(fc))>(c, "type const")
                && KT::assert_type<int&&, decltype(NF::mem_fn(&foo::mem)(foo(20)))>(c, "type temporary")
                && KT::assert_true(c, "call(f)", 18 == NF::mem_fn(&foo::mem)(f))
                && KT::assert_true(c, "call(&f)", 18 == NF::mem_fn(&foo::mem)(&f))
                && KT::assert_true(c, "call(smart)", 18 == NF::mem_fn(&foo::mem)(smart<foo>(18)))
                && KT::assert_true(c, "equal", NF::mem_fn(&foo::mem) == NF::mem_fn(&foo::mem))
                && KT::assert_false(c, "not equal", NF::mem_fn(&foo::mem) == NF::mem_fn(&foo::omem))
                ;
        }),
    KT::expect_success("mem_fn(mem0)", [](KT::context& c)->bool{
            foo f(18);
            return KT::assert_type<std::tuple<int>, decltype(NF::mem_fn(&foo::mem0)(f))>(c, "type")
                && KT::assert_type<std::tuple<int>, decltype(NF::mem_fn(&foo::mem0))::result_type>(c, "result_type")
                && KT::assert_type<foo*,
                                   decltype(NF::mem_fn(&foo::mem0))::argument_type>(c, "foo::mem0 argument_type")
                && KT::assert_type<foo const*,
                                   decltype(NF::mem_fn(&foo::cmem0))::argument_type>(c, "foo::cmem0 argument_type")
                && KT::assert_type<foo volatile*,
                                   decltype(NF::mem_fn(&foo::vmem0))::argument_type>(c, "foo::vmem0 argument_type")
                && KT::assert_type<foo const volatile*,
                                   decltype(NF::mem_fn(&foo::cvmem0))::argument_type>(c, "foo::cvmem0 argument_type")
                && KT::assert_true(c, "call(f)", std::make_tuple(18) == NF::mem_fn(&foo::mem0)(f))
                && KT::assert_true(c, "call(&f)", std::make_tuple(18) == NF::mem_fn(&foo::mem0)(&f))
                && KT::assert_true(c, "call(smart)", std::make_tuple(18) == NF::mem_fn(&foo::mem0)(smart<foo>(18)))
                && KT::assert_true(c, "equal", NF::mem_fn(&foo::mem0) == NF::mem_fn(&foo::mem0))
                && KT::assert_false(c, "not equal", NF::mem_fn(&foo::mem0) == NF::mem_fn(&foo::omem0))
                ;
        }),
    KT::expect_success("mem_fn(mem1)", [](KT::context& c)->bool{
            foo f(18);
            return KT::assert_type<std::tuple<int, int>, decltype(NF::mem_fn(&foo::mem1)(f, 43))>(c, "type")
                && KT::assert_type<std::tuple<int, int>, decltype(NF::mem_fn(&foo::mem1))::result_type>(c, "result_type")
                && KT::assert_type<foo*,
                                   decltype(NF::mem_fn(&foo::mem1))::first_argument_type>(c, "foo::mem1 first_argument_type")
                && KT::assert_type<int,
                                   decltype(NF::mem_fn(&foo::mem1))::second_argument_type>(c, "foo::mem1 second_argument_type")
                && KT::assert_type<foo const*,
                                   decltype(NF::mem_fn(&foo::cmem1))::first_argument_type>(c, "foo::cmem1 first_argument_type")
                && KT::assert_type<int,
                                   decltype(NF::mem_fn(&foo::cmem1))::second_argument_type>(c, "foo::cmem1 second_argument_type")
                && KT::assert_type<foo volatile*,
                                   decltype(NF::mem_fn(&foo::vmem1))::first_argument_type>(c, "foo::vmem1 first_argument_type")
                && KT::assert_type<int,
                                   decltype(NF::mem_fn(&foo::vmem1))::second_argument_type>(c, "foo::vmem1 second_argument_type")
                && KT::assert_type<foo const volatile*,
                                   decltype(NF::mem_fn(&foo::cvmem1))::first_argument_type>(c, "foo::cvmem1 first_argument_type")
                && KT::assert_type<int,
                                   decltype(NF::mem_fn(&foo::cvmem1))::second_argument_type>(c, "foo::cvmem1 second_argument_type")
                && KT::assert_true(c, "call(f)", std::make_tuple(18, 43) == NF::mem_fn(&foo::mem1)(f, 43))
                && KT::assert_true(c, "call(&f)", std::make_tuple(18, 43) == NF::mem_fn(&foo::mem1)(&f, 43))
                && KT::assert_true(c, "call(smart)", std::make_tuple(18, 43) == NF::mem_fn(&foo::mem1)(smart<foo>(18), 43))
                && KT::assert_true(c, "equal", NF::mem_fn(&foo::mem1) == NF::mem_fn(&foo::mem1))
                && KT::assert_false(c, "not equal", NF::mem_fn(&foo::mem1) == NF::mem_fn(&foo::omem1))
                ;
        }),
    KT::expect_success("mem_fn(mem2)", [](KT::context& c)->bool{
            foo f(18);
            return KT::assert_type<std::tuple<int, int, int>, decltype(NF::mem_fn(&foo::mem2)(f, 43, 11))>(c, "type")
                && KT::assert_type<std::tuple<int, int, int>, decltype(NF::mem_fn(&foo::mem2))::result_type>(c, "result_type")
                && KT::assert_true(c, "call(f)", std::make_tuple(18, 43, 11) == NF::mem_fn(&foo::mem2)(f, 43, 11))
                && KT::assert_true(c, "call(&f)", std::make_tuple(18, 43, 11) == NF::mem_fn(&foo::mem2)(&f, 43, 11))
                && KT::assert_true(c, "call(smart)", std::make_tuple(18, 43, 11) == NF::mem_fn(&foo::mem2)(smart<foo>(18), 43, 11))
                && KT::assert_true(c, "equal", NF::mem_fn(&foo::mem2) == NF::mem_fn(&foo::mem2))
                && KT::assert_false(c, "not equal", NF::mem_fn(&foo::mem2) == NF::mem_fn(&foo::omem2))
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("functional::mem_fn", ac, av, ::tests);
}
