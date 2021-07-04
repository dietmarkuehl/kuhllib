// nstd/type_traits/remove_cvref.t.cpp                                -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2021 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/type_traits/remove_cvref.hpp"
#include "kuhl/test.hpp"

namespace TT = ::nstd::type_traits;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("simple type looses CV qualification", [](KT::context& ){
           return KT::type<int> == KT::type<TT::remove_cvref<int>::type>
               && KT::type<int> == KT::type<TT::remove_cvref_t<int>>
               && KT::type<int> == KT::type<TT::remove_cvref<int const>::type>
               && KT::type<int> == KT::type<TT::remove_cvref_t<int const>>
               && KT::type<int> == KT::type<TT::remove_cvref<int volatile>::type>
               && KT::type<int> == KT::type<TT::remove_cvref_t<int volatile>>
               && KT::type<int> == KT::type<TT::remove_cvref<int const volatile>::type>
               && KT::type<int> == KT::type<TT::remove_cvref_t<int const volatile>>
               ;
        }),
    KT::expect_success("reference type looses reference and CV qualification", [](KT::context& ){
           return KT::type<int> == KT::type<TT::remove_cvref<int&>::type>
               && KT::type<int> == KT::type<TT::remove_cvref_t<int&>>
               && KT::type<int> == KT::type<TT::remove_cvref<int const&>::type>
               && KT::type<int> == KT::type<TT::remove_cvref_t<int const&>>
               && KT::type<int> == KT::type<TT::remove_cvref<int volatile&>::type>
               && KT::type<int> == KT::type<TT::remove_cvref_t<int volatile&>>
               && KT::type<int> == KT::type<TT::remove_cvref<int const volatile&>::type>
               && KT::type<int> == KT::type<TT::remove_cvref_t<int const volatile&>>
               ;
        }),
    KT::expect_success("array reference type looses reference and CV qualification", [](KT::context& ){
           return KT::type<int[5]> == KT::type<TT::remove_cvref<int(&)[5]>::type>
               && KT::type<int[5]> == KT::type<TT::remove_cvref_t<int(&)[5]>>
               && KT::type<int[5]> == KT::type<TT::remove_cvref<int const(&)[5]>::type>
               && KT::type<int[5]> == KT::type<TT::remove_cvref_t<int const(&)[5]>>
               && KT::type<int[5]> == KT::type<TT::remove_cvref<int volatile(&)[5]>::type>
               && KT::type<int[5]> == KT::type<TT::remove_cvref_t<int volatile(&)[5]>>
               && KT::type<int[5]> == KT::type<TT::remove_cvref<int const volatile(&)[5]>::type>
               && KT::type<int[5]> == KT::type<TT::remove_cvref_t<int const volatile(&)[5]>>
               ;
        }),
};

static KT::add_tests suite("type_traits::remove_cvref", ::tests);
