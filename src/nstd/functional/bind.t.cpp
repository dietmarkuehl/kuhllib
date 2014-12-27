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
#include "kuhl/test.hpp"
#include <tuple>

#if 1
namespace NF = nstd::functional;
#else
#include <functional>
namespace NF = std;
#endif
namespace NFP = NF::placeholders;

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
        //-dk:TODO std::tuple<int, int> mem1(int v0) { return std::make_tuple(this->mem, v0); }
        //-dk:TODO std::tuple<int, int> omem1(int v0) { return std::make_tuple(this->mem, v0); }
        //-dk:TODO std::tuple<int, int, int> mem2(int v0, int v1) { return std::make_tuple(this->mem, v0, v1); }
        //-dk:TODO std::tuple<int, int, int> omem2(int v0, int v1) { return std::make_tuple(this->mem, v0, v1); }

        bool operator== (foo const& other) const { return this->mem == other.mem; }
        //-dk:TODO bool operator!= (foo const& other) const { return !(*this == other); }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
#if 0
    KT::expect_success("bind(f<>)", [](KT::context& c)->bool{
            return KT::assert_type<std::tuple<>, decltype(NF::bind(f<>)())>(c, "type 0")
                && KT::assert_true(c, "call", std::tuple<>() == NF::bind(f<>)())
                && KT::assert_true(c, "equal f<>/f<>", NF::bind(f<>) == NF::bind(f<>))
                && KT::assert_false(c, "equal f<>/g<>", NF::bind(f<>) == NF::bind(g<>))
                && KT::assert_false(c, "not equal f<>/f<>", NF::bind(f<>) != NF::bind(f<>))
                && KT::assert_true(c, "not equal f<>/g<>", NF::bind(f<>) != NF::bind(g<>))
                ;
        }),
#endif
    KT::expect_success("bind(f<int>)", [](KT::context& c)->bool{
            return KT::assert_type<std::tuple<int>, decltype(NF::bind(f<int>, 17)())>(c, "type 1")
                && KT::assert_true(c, "call()", std::tuple<int>(17) == NF::bind(f<int>, 17)())
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
            return true
                && KT::assert_true(c, "bind(foo::mem0, f0)", std::make_tuple(19) == NF::bind(&foo::mem0, f0)())
                && KT::assert_true(c, "equal bind(foo::mem0, f0/f1", NF::bind(&foo::mem0, f0) == NF::bind(&foo::mem0, f1))
                && KT::assert_false(c, "equal bind(foo::mem0, f0/f2)", NF::bind(&foo::mem0, f0) == NF::bind(&foo::mem0, f2))
                && KT::assert_false(c, "equal bind(foo::omem0, f0/f1)", NF::bind(&foo::mem0, f0) == NF::bind(&foo::omem0, f1))
                ; 
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("functional::bind", ac, av, ::tests);
}
