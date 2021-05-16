// nstd/type_traits/remove_extent.t.cpp                               -*-C++-*-
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

#include "nstd/type_traits/remove_extent.hpp"
#include "kuhl/test.hpp"

namespace NT = nstd::type_traits;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct foo;
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("non-array stay unchanged", [](KT::context& c)->bool{
           return KT::assert_type<foo, NT::remove_extent<foo>::type>(c, "foo type")
               && KT::assert_type<foo, NT::remove_extent_t<foo> >(c, "foo _t")
               && KT::assert_type<foo const, NT::remove_extent<foo const>::type>(c, "foo const type")
               && KT::assert_type<foo const, NT::remove_extent_t<foo const> >(c, "foo const _t")
               && KT::assert_type<foo volatile, NT::remove_extent<foo volatile>::type>(c, "foo volatile type")
               && KT::assert_type<foo volatile, NT::remove_extent_t<foo volatile> >(c, "foo volatile _t")
               && KT::assert_type<foo const volatile, NT::remove_extent<foo const volatile>::type>(c, "foo const volatile type")
               && KT::assert_type<foo const volatile, NT::remove_extent_t<foo const volatile> >(c, "foo const volatile _t")
               ;
        }),
    KT::expect_success("one level sized array extent is removed", [](KT::context& c)->bool{
           return KT::assert_type<foo, NT::remove_extent<foo[17]>::type>(c, "foo[17] type")
               && KT::assert_type<foo, NT::remove_extent_t<foo[17]> >(c, "foo[17] _t")
               && KT::assert_type<foo const, NT::remove_extent<foo const[17]>::type>(c, "foo const[17] type")
               && KT::assert_type<foo const, NT::remove_extent_t<foo const[17]> >(c, "foo const[17] _t")
               && KT::assert_type<foo volatile, NT::remove_extent<foo volatile[17]>::type>(c, "foo volatile[17] type")
               && KT::assert_type<foo volatile, NT::remove_extent_t<foo volatile[17]> >(c, "foo volatile[17] _t")
               && KT::assert_type<foo const volatile, NT::remove_extent<foo const volatile[17]>::type>(c, "foo const volatile[17] type")
               && KT::assert_type<foo const volatile, NT::remove_extent_t<foo const volatile[17]> >(c, "foo const volatile[17] _t")
               ;
        }),
    KT::expect_success("two level sized array extent has one level removed", [](KT::context& c)->bool{
           return KT::assert_type<foo[19], NT::remove_extent<foo[17][19]>::type>(c, "foo[17][19] type")
               && KT::assert_type<foo[19], NT::remove_extent_t<foo[17][19]> >(c, "foo[17][19] _t")
               && KT::assert_type<foo const[19], NT::remove_extent<foo const[17][19]>::type>(c, "foo const[17][19] type")
               && KT::assert_type<foo const[19], NT::remove_extent_t<foo const[17][19]> >(c, "foo const[17][19] _t")
               && KT::assert_type<foo volatile[19], NT::remove_extent<foo volatile[17][19]>::type>(c, "foo volatile[17][19] type")
               && KT::assert_type<foo volatile[19], NT::remove_extent_t<foo volatile[17][19]> >(c, "foo volatile[17][19] _t")
               && KT::assert_type<foo const volatile[19], NT::remove_extent<foo const volatile[17][19]>::type>(c, "foo const volatile[17][19] type")
               && KT::assert_type<foo const volatile[19], NT::remove_extent_t<foo const volatile[17][19]> >(c, "foo const volatile[17][19] _t")
               ;
        }),
    KT::expect_success("one level unsized array extent is removed", [](KT::context& c)->bool{
           return KT::assert_type<foo, NT::remove_extent<foo[]>::type>(c, "foo[] type")
               && KT::assert_type<foo, NT::remove_extent_t<foo[]> >(c, "foo[] _t")
               && KT::assert_type<foo const, NT::remove_extent<foo const[]>::type>(c, "foo const[] type")
               && KT::assert_type<foo const, NT::remove_extent_t<foo const[]> >(c, "foo const[] _t")
               && KT::assert_type<foo volatile, NT::remove_extent<foo volatile[]>::type>(c, "foo volatile[] type")
               && KT::assert_type<foo volatile, NT::remove_extent_t<foo volatile[]> >(c, "foo volatile[] _t")
               && KT::assert_type<foo const volatile, NT::remove_extent<foo const volatile[]>::type>(c, "foo const volatile[] type")
               && KT::assert_type<foo const volatile, NT::remove_extent_t<foo const volatile[]> >(c, "foo const volatile[] _t")
               ;
        }),
    KT::expect_success("two level unsized array extent has one level removed", [](KT::context& c)->bool{
           return KT::assert_type<foo[19], NT::remove_extent<foo[][19]>::type>(c, "foo[][19] type")
               && KT::assert_type<foo[19], NT::remove_extent_t<foo[][19]> >(c, "foo[][19] _t")
               && KT::assert_type<foo const[19], NT::remove_extent<foo const[][19]>::type>(c, "foo const[][19] type")
               && KT::assert_type<foo const[19], NT::remove_extent_t<foo const[][19]> >(c, "foo const[][19] _t")
               && KT::assert_type<foo volatile[19], NT::remove_extent<foo volatile[][19]>::type>(c, "foo volatile[][19] type")
               && KT::assert_type<foo volatile[19], NT::remove_extent_t<foo volatile[][19]> >(c, "foo volatile[][19] _t")
               && KT::assert_type<foo const volatile[19], NT::remove_extent<foo const volatile[][19]>::type>(c, "foo const volatile[][19] type")
               && KT::assert_type<foo const volatile[19], NT::remove_extent_t<foo const volatile[][19]> >(c, "foo const volatile[][19] _t")
               ;
        }),
};

static KT::add_tests suite("type_traits::remove_extent", ::tests);
