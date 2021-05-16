// nstd/type_traits/decay.t.cpp                                       -*-C++-*-
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

#include "nstd/type_traits/decay.hpp"
#include "kuhl/test.hpp"

namespace NT = nstd::type_traits;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    class foo;
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("simple type", [](KT::context& c)->bool{
            return KT::assert_type<foo, NT::decay<foo>::type>(c, "foo type")
                && KT::assert_type<foo, NT::decay_t<foo>>(c, "foo _t")
                && KT::assert_type<foo, NT::decay<foo const>::type>(c, "foo const type")
                && KT::assert_type<foo, NT::decay_t<foo const>>(c, "foo const _t")
                && KT::assert_type<foo, NT::decay<foo volatile>::type>(c, "foo volatile type")
                && KT::assert_type<foo, NT::decay_t<foo volatile>>(c, "foo volatile _t")
                && KT::assert_type<foo, NT::decay<foo const volatile>::type>(c, "foo const volatile type")
                && KT::assert_type<foo, NT::decay_t<foo const volatile>>(c, "foo const volatile _t")
                ;
        }),
    KT::expect_success("lvalue-refence for simple type", [](KT::context& c)->bool{
            return KT::assert_type<foo, NT::decay<foo&>::type>(c, "foo& type")
                && KT::assert_type<foo, NT::decay_t<foo&>>(c, "foo& _t")
                && KT::assert_type<foo, NT::decay<foo const&>::type>(c, "foo const& type")
                && KT::assert_type<foo, NT::decay_t<foo const&>>(c, "foo const& _t")
                && KT::assert_type<foo, NT::decay<foo volatile&>::type>(c, "foo volatile& type")
                && KT::assert_type<foo, NT::decay_t<foo volatile&>>(c, "foo volatile& _t")
                && KT::assert_type<foo, NT::decay<foo const volatile&>::type>(c, "foo const volatile& type")
                && KT::assert_type<foo, NT::decay_t<foo const volatile&>>(c, "foo const volatile& _t")
                ;
        }),
    KT::expect_success("rvalue-refence for simple type", [](KT::context& c)->bool{
            return KT::assert_type<foo, NT::decay<foo&&>::type>(c, "foo&& type")
                && KT::assert_type<foo, NT::decay_t<foo&&>>(c, "foo&& _t")
                && KT::assert_type<foo, NT::decay<foo const&&>::type>(c, "foo const&& type")
                && KT::assert_type<foo, NT::decay_t<foo const&&>>(c, "foo const&& _t")
                && KT::assert_type<foo, NT::decay<foo volatile&&>::type>(c, "foo volatile&& type")
                && KT::assert_type<foo, NT::decay_t<foo volatile&&>>(c, "foo volatile&& _t")
                && KT::assert_type<foo, NT::decay<foo const volatile&&>::type>(c, "foo const volatile&& type")
                && KT::assert_type<foo, NT::decay_t<foo const volatile&&>>(c, "foo const volatile&& _t")
                ;
        }),
    KT::expect_success("single sized array type", [](KT::context& c)->bool{
            return KT::assert_type<foo*, NT::decay<foo[43]>::type>(c, "foo[43] type")
                && KT::assert_type<foo*, NT::decay_t<foo[43]>>(c, "foo[43] _t")
                && KT::assert_type<foo const*, NT::decay<foo const[43]>::type>(c, "foo const[43] type")
                && KT::assert_type<foo const*, NT::decay_t<foo const[43]>>(c, "foo const[43] _t")
                && KT::assert_type<foo volatile*, NT::decay<foo volatile[43]>::type>(c, "foo volatile[43] type")
                && KT::assert_type<foo volatile*, NT::decay_t<foo volatile[43]>>(c, "foo volatile[43] _t")
                && KT::assert_type<foo const volatile*, NT::decay<foo const volatile[43]>::type>(c, "foo const volatile[43] type")
                && KT::assert_type<foo const volatile*, NT::decay_t<foo const volatile[43]>>(c, "foo const volatile[43] _t")
                ;
        }),
    KT::expect_success("single sized array reference", [](KT::context& c)->bool{
            return KT::assert_type<foo*, NT::decay<foo(&)[43]>::type>(c, "foo(&)[43] type")
                && KT::assert_type<foo*, NT::decay_t<foo(&)[43]>>(c, "foo(&)[43] _t")
                && KT::assert_type<foo const*, NT::decay<foo const(&)[43]>::type>(c, "foo const(&)[43] type")
                && KT::assert_type<foo const*, NT::decay_t<foo const(&)[43]>>(c, "foo const(&)[43] _t")
                && KT::assert_type<foo volatile*, NT::decay<foo volatile(&)[43]>::type>(c, "foo volatile(&)[43] type")
                && KT::assert_type<foo volatile*, NT::decay_t<foo volatile(&)[43]>>(c, "foo volatile(&)[43] _t")
                && KT::assert_type<foo const volatile*, NT::decay<foo const volatile(&)[43]>::type>(c, "foo const volatile(&)[43] type")
                && KT::assert_type<foo const volatile*, NT::decay_t<foo const volatile(&)[43]>>(c, "foo const volatile(&)[43] _t")
                ;
        }),
    KT::expect_success("single sized array pointer", [](KT::context& c)->bool{
            return KT::assert_type<foo(*)[43], NT::decay<foo(*)[43]>::type>(c, "foo(*)[43] type")
                && KT::assert_type<foo(*)[43], NT::decay_t<foo(*)[43]>>(c, "foo(*)[43] _t")
                && KT::assert_type<foo const(*)[43], NT::decay<foo const(*)[43]>::type>(c, "foo const(*)[43] type")
                && KT::assert_type<foo const(*)[43], NT::decay_t<foo const(*)[43]>>(c, "foo const(*)[43] _t")
                && KT::assert_type<foo volatile(*)[43], NT::decay<foo volatile(*)[43]>::type>(c, "foo volatile(*)[43] type")
                && KT::assert_type<foo volatile(*)[43], NT::decay_t<foo volatile(*)[43]>>(c, "foo volatile(*)[43] _t")
                && KT::assert_type<foo const volatile(*)[43], NT::decay<foo const volatile(*)[43]>::type>(c, "foo const volatile(*)[43] type")
                && KT::assert_type<foo const volatile(*)[43], NT::decay_t<foo const volatile(*)[43]>>(c, "foo const volatile(*)[43] _t")
                ;
        }),
    KT::expect_success("double sized array type", [](KT::context& c)->bool{
            return KT::assert_type<foo(*)[17], NT::decay<foo[47][17]>::type>(c, "foo[47][17] type")
                && KT::assert_type<foo(*)[17], NT::decay_t<foo[47][17]>>(c, "foo[47][17] _t")
                && KT::assert_type<foo const(*)[17], NT::decay<foo const[47][17]>::type>(c, "foo const[47][17] type")
                && KT::assert_type<foo const(*)[17], NT::decay_t<foo const[47][17]>>(c, "foo const[47][17] _t")
                && KT::assert_type<foo volatile(*)[17], NT::decay<foo volatile[47][17]>::type>(c, "foo volatile[47][17] type")
                && KT::assert_type<foo volatile(*)[17], NT::decay_t<foo volatile[47][17]>>(c, "foo volatile[47][17] _t")
                && KT::assert_type<foo const volatile(*)[17], NT::decay<foo const volatile[47][17]>::type>(c, "foo const volatile[47][17] type")
                && KT::assert_type<foo const volatile(*)[17], NT::decay_t<foo const volatile[47][17]>>(c, "foo const volatile[47][17] _t")
                ;
        }),
    KT::expect_success("double sized array reference", [](KT::context& c)->bool{
            return KT::assert_type<foo(*)[17], NT::decay<foo(&)[47][17]>::type>(c, "foo(&)[47][17] type")
                && KT::assert_type<foo(*)[17], NT::decay_t<foo(&)[47][17]>>(c, "foo(&)[47][17] _t")
                && KT::assert_type<foo const(*)[17], NT::decay<foo const(&)[47][17]>::type>(c, "foo const(&)[47][17] type")
                && KT::assert_type<foo const(*)[17], NT::decay_t<foo const(&)[47][17]>>(c, "foo const(&)[47][17] _t")
                && KT::assert_type<foo volatile(*)[17], NT::decay<foo volatile(&)[47][17]>::type>(c, "foo volatile(&)[47][17] type")
                && KT::assert_type<foo volatile(*)[17], NT::decay_t<foo volatile(&)[47][17]>>(c, "foo volatile(&)[47][17] _t")
                && KT::assert_type<foo const volatile(*)[17], NT::decay<foo const volatile(&)[47][17]>::type>(c, "foo const volatile(&)[47][17] type")
                && KT::assert_type<foo const volatile(*)[17], NT::decay_t<foo const volatile(&)[47][17]>>(c, "foo const volatile(&)[47][17] _t")
                ;
        }),
    KT::expect_success("double sized array pointer", [](KT::context& c)->bool{
            return KT::assert_type<foo(*)[47][17], NT::decay<foo(*)[47][17]>::type>(c, "foo(*)[47][17] type")
                && KT::assert_type<foo(*)[47][17], NT::decay_t<foo(*)[47][17]>>(c, "foo(*)[47][17] _t")
                && KT::assert_type<foo const(*)[47][17], NT::decay<foo const(*)[47][17]>::type>(c, "foo const(*)[47][17] type")
                && KT::assert_type<foo const(*)[47][17], NT::decay_t<foo const(*)[47][17]>>(c, "foo const(*)[47][17] _t")
                && KT::assert_type<foo volatile(*)[47][17], NT::decay<foo volatile(*)[47][17]>::type>(c, "foo volatile(*)[47][17] type")
                && KT::assert_type<foo volatile(*)[47][17], NT::decay_t<foo volatile(*)[47][17]>>(c, "foo volatile(*)[47][17] _t")
                && KT::assert_type<foo const volatile(*)[47][17], NT::decay<foo const volatile(*)[47][17]>::type>(c, "foo const volatile(*)[47][17] type")
                && KT::assert_type<foo const volatile(*)[47][17], NT::decay_t<foo const volatile(*)[47][17]>>(c, "foo const volatile(*)[47][17] _t")
                ;
        }),
    KT::expect_success("single unsized array type", [](KT::context& c)->bool{
            return KT::assert_type<foo*, NT::decay<foo[]>::type>(c, "foo[] type")
                && KT::assert_type<foo*, NT::decay_t<foo[]>>(c, "foo[] _t")
                && KT::assert_type<foo const*, NT::decay<foo const[]>::type>(c, "foo const[] type")
                && KT::assert_type<foo const*, NT::decay_t<foo const[]>>(c, "foo const[] _t")
                && KT::assert_type<foo volatile*, NT::decay<foo volatile[]>::type>(c, "foo volatile[] type")
                && KT::assert_type<foo volatile*, NT::decay_t<foo volatile[]>>(c, "foo volatile[] _t")
                && KT::assert_type<foo const volatile*, NT::decay<foo const volatile[]>::type>(c, "foo const volatile[] type")
                && KT::assert_type<foo const volatile*, NT::decay_t<foo const volatile[]>>(c, "foo const volatile[] _t")
                ;
        }),
    KT::expect_success("single unsized array reference", [](KT::context& c)->bool{
            return KT::assert_type<foo*, NT::decay<foo(&)[]>::type>(c, "foo(&)[] type")
                && KT::assert_type<foo*, NT::decay_t<foo(&)[]>>(c, "foo(&)[] _t")
                && KT::assert_type<foo const*, NT::decay<foo const(&)[]>::type>(c, "foo const(&)[] type")
                && KT::assert_type<foo const*, NT::decay_t<foo const(&)[]>>(c, "foo const(&)[] _t")
                && KT::assert_type<foo volatile*, NT::decay<foo volatile(&)[]>::type>(c, "foo volatile(&)[] type")
                && KT::assert_type<foo volatile*, NT::decay_t<foo volatile(&)[]>>(c, "foo volatile(&)[] _t")
                && KT::assert_type<foo const volatile*, NT::decay<foo const volatile(&)[]>::type>(c, "foo const volatile(&)[] type")
                && KT::assert_type<foo const volatile*, NT::decay_t<foo const volatile(&)[]>>(c, "foo const volatile(&)[] _t")
                ;
        }),
    KT::expect_success("single unsized array pointer", [](KT::context& c)->bool{
            return KT::assert_type<foo(*)[], NT::decay<foo(*)[]>::type>(c, "foo(*)[] type")
                && KT::assert_type<foo(*)[], NT::decay_t<foo(*)[]>>(c, "foo(*)[] _t")
                && KT::assert_type<foo const(*)[], NT::decay<foo const(*)[]>::type>(c, "foo const(*)[] type")
                && KT::assert_type<foo const(*)[], NT::decay_t<foo const(*)[]>>(c, "foo const(*)[] _t")
                && KT::assert_type<foo volatile(*)[], NT::decay<foo volatile(*)[]>::type>(c, "foo volatile(*)[] type")
                && KT::assert_type<foo volatile(*)[], NT::decay_t<foo volatile(*)[]>>(c, "foo volatile(*)[] _t")
                && KT::assert_type<foo const volatile(*)[], NT::decay<foo const volatile(*)[]>::type>(c, "foo const volatile(*)[] type")
                && KT::assert_type<foo const volatile(*)[], NT::decay_t<foo const volatile(*)[]>>(c, "foo const volatile(*)[] _t")
                ;
        }),
    KT::expect_success("double unsized array type", [](KT::context& c)->bool{
            return KT::assert_type<foo(*)[17], NT::decay<foo[][17]>::type>(c, "foo[][17] type")
                && KT::assert_type<foo(*)[17], NT::decay_t<foo[][17]>>(c, "foo[][17] _t")
                && KT::assert_type<foo const(*)[17], NT::decay<foo const[][17]>::type>(c, "foo const[][17] type")
                && KT::assert_type<foo const(*)[17], NT::decay_t<foo const[][17]>>(c, "foo const[][17] _t")
                && KT::assert_type<foo volatile(*)[17], NT::decay<foo volatile[][17]>::type>(c, "foo volatile[][17] type")
                && KT::assert_type<foo volatile(*)[17], NT::decay_t<foo volatile[][17]>>(c, "foo volatile[][17] _t")
                && KT::assert_type<foo const volatile(*)[17], NT::decay<foo const volatile[][17]>::type>(c, "foo const volatile[][17] type")
                && KT::assert_type<foo const volatile(*)[17], NT::decay_t<foo const volatile[][17]>>(c, "foo const volatile[][17] _t")
                ;
        }),
    KT::expect_success("double unsized array reference", [](KT::context& c)->bool{
            return KT::assert_type<foo(*)[17], NT::decay<foo(&)[][17]>::type>(c, "foo(&)[][17] type")
                && KT::assert_type<foo(*)[17], NT::decay_t<foo(&)[][17]>>(c, "foo(&)[][17] _t")
                && KT::assert_type<foo const(*)[17], NT::decay<foo const(&)[][17]>::type>(c, "foo const(&)[][17] type")
                && KT::assert_type<foo const(*)[17], NT::decay_t<foo const(&)[][17]>>(c, "foo const(&)[][17] _t")
                && KT::assert_type<foo volatile(*)[17], NT::decay<foo volatile(&)[][17]>::type>(c, "foo volatile(&)[][17] type")
                && KT::assert_type<foo volatile(*)[17], NT::decay_t<foo volatile(&)[][17]>>(c, "foo volatile(&)[][17] _t")
                && KT::assert_type<foo const volatile(*)[17], NT::decay<foo const volatile(&)[][17]>::type>(c, "foo const volatile(&)[][17] type")
                && KT::assert_type<foo const volatile(*)[17], NT::decay_t<foo const volatile(&)[][17]>>(c, "foo const volatile(&)[][17] _t")
                ;
        }),
    KT::expect_success("double unsized array pointer", [](KT::context& c)->bool{
            return KT::assert_type<foo(*)[][17], NT::decay<foo(*)[][17]>::type>(c, "foo(*)[][17] type")
                && KT::assert_type<foo(*)[][17], NT::decay_t<foo(*)[][17]>>(c, "foo(*)[][17] _t")
                && KT::assert_type<foo const(*)[][17], NT::decay<foo const(*)[][17]>::type>(c, "foo const(*)[][17] type")
                && KT::assert_type<foo const(*)[][17], NT::decay_t<foo const(*)[][17]>>(c, "foo const(*)[][17] _t")
                && KT::assert_type<foo volatile(*)[][17], NT::decay<foo volatile(*)[][17]>::type>(c, "foo volatile(*)[][17] type")
                && KT::assert_type<foo volatile(*)[][17], NT::decay_t<foo volatile(*)[][17]>>(c, "foo volatile(*)[][17] _t")
                && KT::assert_type<foo const volatile(*)[][17], NT::decay<foo const volatile(*)[][17]>::type>(c, "foo const volatile(*)[][17] type")
                && KT::assert_type<foo const volatile(*)[][17], NT::decay_t<foo const volatile(*)[][17]>>(c, "foo const volatile(*)[][17] _t")
                ;
        }),
    KT::expect_success("function types", [](KT::context& c)->bool{
            return KT::assert_type<foo(*)(), NT::decay<foo()>::type>(c, "foo() type")
                && KT::assert_type<foo(*)(), NT::decay_t<foo()>>(c, "foo() _t")
                && KT::assert_type<foo(*)(int), NT::decay<foo(int)>::type>(c, "foo(int) type")
                && KT::assert_type<foo(*)(int), NT::decay_t<foo(int)>>(c, "foo(int) _t")
                && KT::assert_type<foo(*)(int, char), NT::decay<foo(int, char)>::type>(c, "foo(int, char) type")
                && KT::assert_type<foo(*)(int, char), NT::decay_t<foo(int, char)>>(c, "foo(int, char) _t")
                && KT::assert_type<foo(*)(int, ...), NT::decay<foo(int, ...)>::type>(c, "foo(int, ...) type")
                && KT::assert_type<foo(*)(int, ...), NT::decay_t<foo(int, ...)>>(c, "foo(int, ...) _t")
                ;
        }),
};

static KT::add_tests suite("type_traits::decay", ::tests);
