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
    struct fo {
        auto operator()() -> void {}
        auto operator()(int) -> bool { return false; }
        auto operator()(void*) -> long { return 2l; }
        auto operator()(int, char) -> short { return 3; }
    };

    struct foo {};
    struct bar: foo {};
    template <typename T>
    struct smart {
        T value;
        T& operator*() { return value; }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("prepare ", [](KT::context& c)->bool{
            fo o;
            smart<int> s;
            s.value = 5;
            smart<foo> fs;
            *fs;
            smart<bar> bs;
            *bs;
            return KT::assert_equal(c, "o()", true, (o(), true))
                && KT::assert_equal(c, "o(0)", false, o(0))
                && KT::assert_equal(c, "o(nullptr)", 2l, o(nullptr))
                && KT::assert_equal(c, "o(0, 'c')", 3, o(0, 'c'))
                && KT::assert_equal(c, "*s", 5, *s)
                ;
        }),
    KT::expect_success("function pointers", [](KT::context& c)->bool{
            return KT::assert_type<bool,   NT::result_of<bool (*())()>::type>(c, "f0 type")
                && KT::assert_type<bool,   NT::result_of_t<bool (*())()>>(c, "f0 _t")
                && KT::assert_type<char,   NT::result_of<char (*(int))(int)>::type>(c, "f1 type")
                && KT::assert_type<char,   NT::result_of_t<char (*(int))(int)>>(c, "f1 _t")
                && KT::assert_type<short,  NT::result_of<short (*(int, double))(int, double)>::type>(c, "f2 type")
                && KT::assert_type<short,  NT::result_of_t<short (*(int, double))(int, double)>>(c, "f2 _t")
                && KT::assert_type<bool&,  NT::result_of<bool& (*())()>::type>(c, "r0 type")
                && KT::assert_type<bool&,  NT::result_of_t<bool& (*())()>>(c, "r0 _t")
                && KT::assert_type<char&,  NT::result_of<char& (*(int))(int)>::type>(c, "r1 type")
                && KT::assert_type<char&,  NT::result_of_t<char& (*(int))(int)>>(c, "r1 _t")
                && KT::assert_type<short&, NT::result_of<short& (*(int, double))(int, double)>::type>(c, "r2 type")
                && KT::assert_type<short&, NT::result_of_t<short& (*(int, double))(int, double)>>(c, "r2 _t")
                && KT::assert_no_nested_type<NT::result_of<short& (*(int, double))(int*, double)>>(c, "not callable")
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
                && KT::assert_no_nested_type<NT::result_of<auto (smart<bar>, int*) -> bool(foo::*)(int)> >(c, "auto (smart<bar>, int) -> bool(foo::*)(int*) not callable")
                ;
        }),
    KT::expect_success("member objects", [](KT::context& c)->bool{
            return KT::assert_type<int&&,   NT::result_of<auto (foo) -> int foo::*>::type>(c, "auto (foo) -> int foo::* type")
                && KT::assert_type<int&&,   NT::result_of<auto (bar) -> int foo::*>::type>(c, "auto (bar) -> int foo::* type")
                && KT::assert_type<int&,   NT::result_of<auto (foo&) -> int foo::*>::type>(c, "auto (foo&) -> int foo::* type")
                && KT::assert_type<int&,   NT::result_of<auto (bar&) -> int foo::*>::type>(c, "auto (bar&) -> int foo::* type")
                && KT::assert_type<int&&,   NT::result_of<auto (foo const) -> int foo::*>::type>(c, "auto (foo const) -> int foo::* type")
                && KT::assert_type<int&&,   NT::result_of<auto (bar const) -> int foo::*>::type>(c, "auto (bar const) -> int foo::* type")
                && KT::assert_type<int const&,   NT::result_of<auto (foo const&) -> int foo::*>::type>(c, "auto (foo const&) -> int foo::* type")
                && KT::assert_type<int const&,   NT::result_of<auto (bar const&) -> int foo::*>::type>(c, "auto (bar const&) -> int foo::* type")
                && KT::assert_type<int&,   NT::result_of<auto (foo*) -> int foo::*>::type>(c, "auto (foo*) -> int foo::* type")
                && KT::assert_type<int&,   NT::result_of<auto (bar*) -> int foo::*>::type>(c, "auto (bar*) -> int foo::* type")
                && KT::assert_type<int&,   NT::result_of<auto (smart<foo>) -> int foo::*>::type>(c, "auto (smart<foo>) -> int foo::* type")
                && KT::assert_type<int&,   NT::result_of<auto (smart<bar>) -> int foo::*>::type>(c, "auto (smart<bar>) -> int foo::* type")
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("type_traits::result_of", ac, av, ::tests);
}
