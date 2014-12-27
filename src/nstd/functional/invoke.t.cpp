// nstd/functional/invoke.t.cpp                                       -*-C++-*-
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

#include "nstd/functional/invoke.hpp"
#include "kuhl/test.hpp"

namespace NF = nstd::functional;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    int fun(char, double) { return 41; }
    int funv(char, double, ...) { return 40; }
    struct function {
        auto operator()(char, double) -> int { return 42; }
        auto operator()(int, double, ...) -> long { return 45; }
    };
    struct base {
        int value;
        base(int value): value(value) {}
        auto member(char, double) -> int { return this->value + 43; }
        auto vararg(char, double, ...) -> int { return this->value + 44; }
    };
    struct derived: base {
        base value;
        derived(int value): base(value), value(value * 2) {}
        //-dk:TODO auto operator*() -> base& { return this->value; }
    };

    template <typename T>
    struct smart_ptr {
        T pointee;
        template <typename... A>
        smart_ptr(A... a): pointee(a...) {}
        auto operator*() -> T& { return pointee; }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("invoke(fun)", [](KT::context& c)->bool{
            return KT::assert_type<int, decltype(NF::invoke(fun, 'c', 3.14))>(c, "type")
                && KT::assert_equal(c, "call", 41, NF::invoke(fun, 'c', 3.14))
               ;
        }),
    KT::expect_success("invoke(funv)", [](KT::context& c)->bool{
            return KT::assert_type<int, decltype(NF::invoke(funv, 'c', 3.14, 'a', 'b'))>(c, "type")
                && KT::assert_equal(c, "call", 40, NF::invoke(funv, 'c', 3.14, 'a', 'b'))
               ;
        }),
    KT::expect_success("invoke(function())", [](KT::context& c)->bool{
            return KT::assert_type<int, decltype(NF::invoke(function(), 'c', 3.14))>(c, "type")
                && KT::assert_equal(c, "call", 42, NF::invoke(function(), 'c', 3.14))
               ;
        }),
    KT::expect_success("invoke(function()) (vararg)", [](KT::context& c)->bool{
            return KT::assert_type<long, decltype(NF::invoke(function(), 'c', 3.14, 'a', 'b'))>(c, "type")
                && KT::assert_equal(c, "call", 45, NF::invoke(function(), 'c', 3.14, 'a', 'b'))
               ;
        }),
    KT::expect_success("invoke(&base::member, base)", [](KT::context& c)->bool{
            base object(10);
            return KT::assert_type<int, decltype(NF::invoke(&base::member, object, 'c', 3.14))>(c, "type")
                && KT::assert_equal(c, "call", 53, NF::invoke(&base::member, object, 'c', 3.14))
               ;
        }),
    KT::expect_success("invoke(&base::vararg, base)", [](KT::context& c)->bool{
            base object(13);
            return KT::assert_type<int, decltype(NF::invoke(&base::vararg, object, 'c', 3.14, 3, 4))>(c, "type")
                && KT::assert_equal(c, "call", 57, NF::invoke(&base::vararg, object, 'c', 3.14, 3, 4))
               ;
        }),
    KT::expect_success("invoke(&base::member, derived)", [](KT::context& c)->bool{
            derived object(11);
            return KT::assert_type<int, decltype(NF::invoke(&base::member, object, 'c', 3.14))>(c, "type")
                && KT::assert_equal(c, "call", 54, NF::invoke(&base::member, object, 'c', 3.14))
               ;
        }),
    KT::expect_success("invoke(&base::vararg, derived)", [](KT::context& c)->bool{
            derived object(12);
            return KT::assert_type<int, decltype(NF::invoke(&base::vararg, object, 'c', 3.14, 3, 4))>(c, "type")
                && KT::assert_equal(c, "call", 56, NF::invoke(&base::vararg, object, 'c', 3.14, 3, 4))
               ;
        }),
    KT::expect_success("invoke(&base::value, base)", [](KT::context& c)->bool{
            base object(10);
            return KT::assert_type<int&, decltype(NF::invoke(&base::value, object))>(c, "type")
                && KT::assert_equal(c, "call", 10, NF::invoke(&base::value, object))
               ;
        }),
    KT::expect_success("invoke(&base::value, derived)", [](KT::context& c)->bool{
            derived object(11);
            return KT::assert_type<int&, decltype(NF::invoke(&base::value, object))>(c, "type")
                && KT::assert_equal(c, "call", 11, NF::invoke(&base::value, object))
               ;
        }),
    KT::expect_success("invoke(&base::member, base*)", [](KT::context& c)->bool{
            base object(10);
            return KT::assert_type<int, decltype(NF::invoke(&base::member, &object, 'c', 3.14))>(c, "type")
                && KT::assert_equal(c, "call", 53, NF::invoke(&base::member, &object, 'c', 3.14))
               ;
        }),
    KT::expect_success("invoke(&base::vararg, base*)", [](KT::context& c)->bool{
            base object(13);
            return KT::assert_type<int, decltype(NF::invoke(&base::vararg, &object, 'c', 3.14, 3, 4))>(c, "type")
                && KT::assert_equal(c, "call", 57, NF::invoke(&base::vararg, &object, 'c', 3.14, 3, 4))
               ;
        }),
    KT::expect_success("invoke(&base::member, derived*)", [](KT::context& c)->bool{
            derived object(11);
            return KT::assert_type<int, decltype(NF::invoke(&base::member, &object, 'c', 3.14))>(c, "type")
                && KT::assert_equal(c, "call", 54, NF::invoke(&base::member, &object, 'c', 3.14))
               ;
        }),
    KT::expect_success("invoke(&base::vararg, derived*)", [](KT::context& c)->bool{
            derived object(12);
            return KT::assert_type<int, decltype(NF::invoke(&base::vararg, &object, 'c', 3.14, 3, 4))>(c, "type")
                && KT::assert_equal(c, "call", 56, NF::invoke(&base::vararg, &object, 'c', 3.14, 3, 4))
               ;
        }),
    KT::expect_success("invoke(&base::value, base*)", [](KT::context& c)->bool{
            base object(10);
            return KT::assert_type<int&, decltype(NF::invoke(&base::value, &object))>(c, "type")
                && KT::assert_equal(c, "call", 10, NF::invoke(&base::value, &object))
               ;
        }),
    KT::expect_success("invoke(&base::value, derived*)", [](KT::context& c)->bool{
            derived object(11);
            return KT::assert_type<int&, decltype(NF::invoke(&base::value, &object))>(c, "type")
                && KT::assert_equal(c, "call", 11, NF::invoke(&base::value, &object))
               ;
        }),
    KT::expect_success("invoke(&base::member, smart_ptr<base>)", [](KT::context& c)->bool{
            smart_ptr<base> object(10);
            return KT::assert_type<int, decltype(NF::invoke(&base::member, object, 'c', 3.14))>(c, "type")
                && KT::assert_equal(c, "call", 53, NF::invoke(&base::member, object, 'c', 3.14))
               ;
        }),
    KT::expect_success("invoke(&base::vararg, smart_ptr<base>)", [](KT::context& c)->bool{
            smart_ptr<base> object(13);
            return KT::assert_type<int, decltype(NF::invoke(&base::vararg, object, 'c', 3.14, 3, 4))>(c, "type")
                && KT::assert_equal(c, "call", 57, NF::invoke(&base::vararg, object, 'c', 3.14, 3, 4))
               ;
        }),
    KT::expect_success("invoke(&base::member, smart_ptr<derived>)", [](KT::context& c)->bool{
            smart_ptr<derived> object(11);
            return KT::assert_type<int, decltype(NF::invoke(&base::member, object, 'c', 3.14))>(c, "type")
                && KT::assert_equal(c, "call", 54, NF::invoke(&base::member, object, 'c', 3.14))
               ;
        }),
    KT::expect_success("invoke(&base::vararg, smart_ptr<derived>)", [](KT::context& c)->bool{
            smart_ptr<derived> object(12);
            return KT::assert_type<int, decltype(NF::invoke(&base::vararg, object, 'c', 3.14, 3, 4))>(c, "type")
                && KT::assert_equal(c, "call", 56, NF::invoke(&base::vararg, object, 'c', 3.14, 3, 4))
               ;
        }),
    KT::expect_success("invoke(&base::value, smart_ptr<base>)", [](KT::context& c)->bool{
            smart_ptr<base> object(10);
            return KT::assert_type<int&, decltype(NF::invoke(&base::value, object))>(c, "type")
                && KT::assert_equal(c, "call", 10, NF::invoke(&base::value, object))
               ;
        }),
    KT::expect_success("invoke(&base::value, smart_ptr<derived>)", [](KT::context& c)->bool{
            smart_ptr<derived> object(11);
            return KT::assert_type<int&, decltype(NF::invoke(&base::value, object))>(c, "type")
                && KT::assert_equal(c, "call", 11, NF::invoke(&base::value, object))
               ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("functional::invoke", ac, av, ::tests);
}
