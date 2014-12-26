// nstd/type_traits/result_of.t.cpp                                   -*-C++-*-
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

#include "nstd/type_traits/result_of.hpp"
#include "kuhl/test.hpp"

#if 0
#include <type_traits>
namespace NT = std;
namespace std { template <typename T> using result_of_t = typename std::result_of<T>::type; }
#else
namespace NT = nstd::type_traits;
#endif
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    auto f0() -> bool;
    auto f1(int) -> char;
    auto f2(int, double) -> short;
    auto r0() ->bool&;
    auto r1(int) -> char&;
    auto r2(int, double) -> short&;

    struct fo {
        auto operator()() -> void;
        auto operator()(int) -> bool;
        auto operator()(void*) -> long;
        auto operator()(int, char) -> short;
    };

    struct foo {};
    struct bar: foo {};
    template <typename T>
    struct smart {
        T& operator*();
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("function pointers", [](KT::context& c)->bool{
            return KT::assert_type<bool,   NT::result_of<decltype(&f0)()>::type>(c, "f0 type")
                && KT::assert_type<bool,   NT::result_of_t<decltype(&f0)()>>(c, "f0 _t")
                && KT::assert_type<char,   NT::result_of<decltype(&f1)(int)>::type>(c, "f1 type")
                && KT::assert_type<char,   NT::result_of_t<decltype(&f1)(int)>>(c, "f1 _t")
                && KT::assert_type<short,  NT::result_of<decltype(&f2)(int, double)>::type>(c, "f2 type")
                && KT::assert_type<short,  NT::result_of_t<decltype(&f2)(int, double)>>(c, "f2 _t")
                && KT::assert_type<bool&,  NT::result_of<decltype(&r0)()>::type>(c, "r0 type")
                && KT::assert_type<bool&,  NT::result_of_t<decltype(&r0)()>>(c, "r0 _t")
                && KT::assert_type<char&,  NT::result_of<decltype(&r1)(int)>::type>(c, "r1 type")
                && KT::assert_type<char&,  NT::result_of_t<decltype(&r1)(int)>>(c, "r1 _t")
                && KT::assert_type<short&, NT::result_of<decltype(&r2)(int, double)>::type>(c, "r2 type")
                && KT::assert_type<short&, NT::result_of_t<decltype(&r2)(int, double)>>(c, "r2 _t")
                && KT::assert_no_nested_type<NT::result_of<decltype(&r2)(int*, double)>>(c, "not callable")
                ;
        }),
    KT::expect_success("function object", [](KT::context& c)->bool{
            return KT::assert_type<void,   NT::result_of<fo()>::type>(c, "fo() type")
                && KT::assert_type<void,   NT::result_of_t<fo()>>(c, "fo() _t")
                && KT::assert_type<bool,   NT::result_of<fo(int)>::type>(c, "fo(int) type")
                && KT::assert_type<bool,   NT::result_of_t<fo(int)>>(c, "fo(int) _t")
                && KT::assert_type<long,   NT::result_of<fo(void*)>::type>(c, "fo(void*) type")
                && KT::assert_type<long,   NT::result_of_t<fo(void*)>>(c, "fo(void*) _t")
                && KT::assert_type<bool,   NT::result_of<fo(char)>::type>(c, "fo(char) type")
                && KT::assert_type<bool,   NT::result_of_t<fo(char)>>(c, "fo(char) _t")
                && KT::assert_type<long,   NT::result_of<fo(char*)>::type>(c, "fo(char*) type")
                && KT::assert_type<long,   NT::result_of_t<fo(char*)>>(c, "fo(char*) _t")
                && KT::assert_type<short,  NT::result_of<fo(int, char)>::type>(c, "fo(int, char) type")
                && KT::assert_type<short,  NT::result_of_t<fo(int, char)>>(c, "fo(int, char) _t")
                && KT::assert_type<short,  NT::result_of<fo&(int, char)>::type>(c, "fo&")
                && KT::assert_type<short,  NT::result_of<fo&&(int, char)>::type>(c, "fo&&")
                && KT::assert_no_nested_type<NT::result_of<fo const(int, char)>>(c, "not callable")
                ;
        }),
    KT::expect_success("member function", [](KT::context& c)->bool{
            return false
#if 0
            return KT::assert_type<void,   NT::result_of<auto (foo) -> void(foo::*)()>::type>(c, "auto (foo) -> void(foo::*)() type")
                && KT::assert_type<void,   NT::result_of_t<auto (foo) -> void(foo::*)()> >(c, "auto (foo) -> void(foo::*)() _t")
                && KT::assert_type<bool,   NT::result_of<auto (foo, int) -> bool(foo::*)(int)>::type>(c, "auto (foo, int) -> bool(foo::*)(int) type")
                && KT::assert_type<bool,   NT::result_of_t<auto (foo, int) -> bool(foo::*)(int)> >(c, "auto (foo, int) -> bool(foo::*)(int) _t")
                && KT::assert_type<void,   NT::result_of<auto (bar) -> void(foo::*)()>::type>(c, "auto (bar) -> void(foo::*)() type")
                && KT::assert_type<void,   NT::result_of_t<auto (bar) -> void(foo::*)()> >(c, "auto (bar) -> void(foo::*)() _t")
                && KT::assert_type<bool,   NT::result_of<auto (bar, int) -> bool(foo::*)(int)>::type>(c, "auto (bar, int) -> bool(foo::*)(int) type")
                && KT::assert_type<bool,   NT::result_of_t<auto (bar, int) -> bool(foo::*)(int)> >(c, "auto (bar, int) -> bool(foo::*)(int) _t")
                && KT::assert_type<void,   NT::result_of<auto (foo*) -> void(foo::*)()>::type>(c, "auto (foo*) -> void(foo::*)() type")
                && KT::assert_type<void,   NT::result_of_t<auto (foo*) -> void(foo::*)()> >(c, "auto (foo*) -> void(foo::*)() _t")
                && KT::assert_type<bool,   NT::result_of<auto (foo*, int) -> bool(foo::*)(int)>::type>(c, "auto (foo*, int) -> bool(foo::*)(int) type")
                && KT::assert_type<bool,   NT::result_of_t<auto (foo*, int) -> bool(foo::*)(int)> >(c, "auto (foo*, int) -> bool(foo::*)(int) _t")
                && KT::assert_type<void,   NT::result_of<auto (smart<foo>) -> void(foo::*)()>::type>(c, "auto (smart<foo>) -> void(foo::*)() type")
                && KT::assert_type<void,   NT::result_of_t<auto (smart<foo>) -> void(foo::*)()> >(c, "auto (smart<foo>) -> void(foo::*)() _t")
                && KT::assert_type<bool,   NT::result_of<auto (smart<foo>, int) -> bool(foo::*)(int)>::type>(c, "auto (smart<foo>, int) -> bool(foo::*)(int) type")
                && KT::assert_type<bool,   NT::result_of_t<auto (smart<foo>, int) -> bool(foo::*)(int)> >(c, "auto (smart<foo>, int) -> bool(foo::*)(int) _t")
                && KT::assert_type<void,   NT::result_of<auto (smart<bar>) -> void(foo::*)()>::type>(c, "auto (smart<bar>) -> void(foo::*)() type")
                && KT::assert_type<void,   NT::result_of_t<auto (smart<bar>) -> void(foo::*)()> >(c, "auto (smart<bar>) -> void(foo::*)() _t")
                && KT::assert_type<bool,   NT::result_of<auto (smart<bar>, int) -> bool(foo::*)(int)>::type>(c, "auto (smart<bar>, int) -> bool(foo::*)(int) type")
                && KT::assert_type<bool,   NT::result_of_t<auto (smart<bar>, int) -> bool(foo::*)(int)> >(c, "auto (smart<bar>, int) -> bool(foo::*)(int) _t")
#endif
                ;
        }),
    KT::expect_success("member objects", [](KT::context& c)->bool{
            return false
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("type_traits::result_of", ac, av, ::tests);
}
