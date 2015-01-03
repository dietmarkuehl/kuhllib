// nstd/type_traits/is_assignable.t.cpp                               -*-C++-*-
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

#include "nstd/type_traits/is_assignable.hpp"
#include "kuhl/test.hpp"

namespace NT = nstd::type_traits;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    struct copyable {
        copyable& operator=(copyable const&) = default;
        copyable& operator=(copyable&&) = delete;
    };
    struct movable {
        movable& operator=(movable const&) = delete;
        movable& operator=(movable&&) = default;
    };
    class private_assign {
        private_assign& operator=(private_assign const&) = default;
        private_assign& operator=(private_assign&&) = default;
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("void", [](KT::context& c)->bool{
            return KT::assert_false(c, "is_assignable<void, void>", NT::is_assignable<void, void>::value)
                &&  KT::assert_false(c, "is_assignable<void,                void>",
                                     NT::is_assignable<void,                void>::value)
                &&  KT::assert_false(c, "is_assignable<void const,          void>",
                                     NT::is_assignable<void const,          void>::value)
                &&  KT::assert_false(c, "is_assignable<void volatile,       void>",
                                     NT::is_assignable<void volatile,       void>::value)
                &&  KT::assert_false(c, "is_assignable<void const volatile, void>",
                                     NT::is_assignable<void const volatile, void>::value)
                &&  KT::assert_false(c, "is_assignable<void,                void const>",
                                     NT::is_assignable<void,                void const>::value)
                &&  KT::assert_false(c, "is_assignable<void const,          void const>",
                                     NT::is_assignable<void const,          void const>::value)
                &&  KT::assert_false(c, "is_assignable<void volatile,       void const>",
                                     NT::is_assignable<void volatile,       void const>::value)
                &&  KT::assert_false(c, "is_assignable<void const volatile, void const>",
                                     NT::is_assignable<void const volatile, void const>::value)
                &&  KT::assert_false(c, "is_assignable<void,                void volatile>",
                                     NT::is_assignable<void,                void volatile>::value)
                &&  KT::assert_false(c, "is_assignable<void const,          void volatile>",
                                     NT::is_assignable<void const,          void volatile>::value)
                &&  KT::assert_false(c, "is_assignable<void volatile,       void volatile>",
                                     NT::is_assignable<void volatile,       void volatile>::value)
                &&  KT::assert_false(c, "is_assignable<void const volatile, void volatile>",
                                     NT::is_assignable<void const volatile, void volatile>::value)
                &&  KT::assert_false(c, "is_assignable<void,                void const volatile>",
                                     NT::is_assignable<void,                void const volatile>::value)
                &&  KT::assert_false(c, "is_assignable<void const,          void const volatile>",
                                     NT::is_assignable<void const,          void const volatile>::value)
                &&  KT::assert_false(c, "is_assignable<void volatile,       void const volatile>",
                                     NT::is_assignable<void volatile,       void const volatile>::value)
                &&  KT::assert_false(c, "is_assignable<void const volatile, void const volatile>",
                                     NT::is_assignable<void const volatile, void const volatile>::value)
                ;
        }),
    KT::expect_success("nullptr", [](KT::context& c)->bool{
            return KT::assert_false(c, "is_assignable<decltype(nullptr),                decltype(nullptr)>",
                                    NT::is_assignable<decltype(nullptr),                decltype(nullptr)>::value)
                && KT::assert_false(c, "is_assignable<decltype(nullptr) const,          decltype(nullptr)>",
                                    NT::is_assignable<decltype(nullptr) const,          decltype(nullptr)>::value)
                && KT::assert_false(c, "is_assignable<decltype(nullptr) volatile,       decltype(nullptr)>",
                                    NT::is_assignable<decltype(nullptr) volatile,       decltype(nullptr)>::value)
                && KT::assert_false(c, "is_assignable<decltype(nullptr) const volatile, decltype(nullptr)>",
                                    NT::is_assignable<decltype(nullptr) const volatile, decltype(nullptr)>::value)
                && KT::assert_false(c, "is_assignable<decltype(nullptr),                decltype(nullptr) const>",
                                    NT::is_assignable<decltype(nullptr),                decltype(nullptr) const>::value)
                && KT::assert_false(c, "is_assignable<decltype(nullptr) const,          decltype(nullptr) const>",
                                    NT::is_assignable<decltype(nullptr) const,          decltype(nullptr) const>::value)
                && KT::assert_false(c, "is_assignable<decltype(nullptr) volatile,       decltype(nullptr) const>",
                                    NT::is_assignable<decltype(nullptr) volatile,       decltype(nullptr) const>::value)
                && KT::assert_false(c, "is_assignable<decltype(nullptr) const volatile, decltype(nullptr) const>",
                                    NT::is_assignable<decltype(nullptr) const volatile, decltype(nullptr) const>::value)
                && KT::assert_false(c, "is_assignable<decltype(nullptr),                decltype(nullptr) volatile>",
                                    NT::is_assignable<decltype(nullptr),                decltype(nullptr) volatile>::value)
                && KT::assert_false(c, "is_assignable<decltype(nullptr) const,          decltype(nullptr) volatile>",
                                    NT::is_assignable<decltype(nullptr) const,          decltype(nullptr) volatile>::value)
                && KT::assert_false(c, "is_assignable<decltype(nullptr) volatile,       decltype(nullptr) volatile>",
                                    NT::is_assignable<decltype(nullptr) volatile,       decltype(nullptr) volatile>::value)
                && KT::assert_false(c, "is_assignable<decltype(nullptr) const volatile, decltype(nullptr) volatile>",
                                    NT::is_assignable<decltype(nullptr) const volatile, decltype(nullptr) volatile>::value)
                && KT::assert_false(c, "is_assignable<decltype(nullptr),                decltype(nullptr) const volatile>",
                                    NT::is_assignable<decltype(nullptr),                decltype(nullptr) const volatile>::value)
                && KT::assert_false(c, "is_assignable<decltype(nullptr) const,          decltype(nullptr) const volatile>",
                                    NT::is_assignable<decltype(nullptr) const,          decltype(nullptr) const volatile>::value)
                && KT::assert_false(c, "is_assignable<decltype(nullptr) volatile,       decltype(nullptr) const volatile>",
                                    NT::is_assignable<decltype(nullptr) volatile,       decltype(nullptr) const volatile>::value)
                && KT::assert_false(c, "is_assignable<decltype(nullptr) const volatile, decltype(nullptr) const volatile>",
                                    NT::is_assignable<decltype(nullptr) const volatile, decltype(nullptr) const volatile>::value)
                ;
        }),
    KT::expect_success("int", [](KT::context& c)->bool{
                return KT::assert_false(c, "is_assignable<int,                 int>",
                                        NT::is_assignable<int,                 int>::value)
                    && KT::assert_false(c, "is_assignable<int,                 int const>",
                                        NT::is_assignable<int,                 int const>::value)
                    && KT::assert_false(c, "is_assignable<int,                 int volatile>",
                                        NT::is_assignable<int,                 int volatile>::value)
                    && KT::assert_false(c, "is_assignable<int,                 int const volatile>",
                                        NT::is_assignable<int,                 int const volatile>::value)
                    && KT::assert_true (c, "is_assignable<int&,                int>",
                                        NT::is_assignable<int&,                int>::value)
                    && KT::assert_true (c, "is_assignable<int&,                int const>",
                                        NT::is_assignable<int&,                int const>::value)
                    && KT::assert_true (c, "is_assignable<int&,                int volatile>",
                                        NT::is_assignable<int&,                int volatile>::value)
                    && KT::assert_true (c, "is_assignable<int&,                int const volatile>",
                                        NT::is_assignable<int&,                int const volatile>::value)
                    && KT::assert_false(c, "is_assignable<int const&,          int>",
                                        NT::is_assignable<int const&,          int>::value)
                    && KT::assert_false(c, "is_assignable<int const&,          int const>",
                                        NT::is_assignable<int const&,          int const>::value)
                    && KT::assert_false(c, "is_assignable<int const&,          int volatile>",
                                        NT::is_assignable<int const&,          int volatile>::value)
                    && KT::assert_false(c, "is_assignable<int const&,          int const volatile>",
                                        NT::is_assignable<int const&,          int const volatile>::value)
                    && KT::assert_true (c, "is_assignable<int volatile&,       int>",
                                        NT::is_assignable<int volatile&,       int>::value)
                    && KT::assert_true (c, "is_assignable<int volatile&,       int const>",
                                        NT::is_assignable<int volatile&,       int const>::value)
                    && KT::assert_true (c, "is_assignable<int volatile&,       int volatile>",
                                        NT::is_assignable<int volatile&,       int volatile>::value)
                    && KT::assert_true (c, "is_assignable<int volatile&,       int const volatile>",
                                        NT::is_assignable<int volatile&,       int const volatile>::value)
                    && KT::assert_false(c, "is_assignable<int const volatile&, int>",
                                        NT::is_assignable<int const volatile&, int>::value)
                    && KT::assert_false(c, "is_assignable<int const volatile&, int const>",
                                        NT::is_assignable<int const volatile&, int const>::value)
                    && KT::assert_false(c, "is_assignable<int const volatile&, int volatile>",
                                        NT::is_assignable<int const volatile&, int volatile>::value)
                    && KT::assert_false(c, "is_assignable<int const volatile&, int const volatile>",
                                        NT::is_assignable<int const volatile&, int const volatile>::value)
                    && KT::assert_false(c, "is_assignable<int,                 int&>",
                                        NT::is_assignable<int,                 int&>::value)
                    && KT::assert_false(c, "is_assignable<int,                 int const&>",
                                        NT::is_assignable<int,                 int const&>::value)
                    && KT::assert_false(c, "is_assignable<int,                 int volatile&>",
                                        NT::is_assignable<int,                 int volatile&>::value)
                    && KT::assert_false(c, "is_assignable<int,                 int const volatile&>",
                                        NT::is_assignable<int,                 int const volatile&>::value)
                    && KT::assert_true (c, "is_assignable<int&,                int&>",
                                        NT::is_assignable<int&,                int&>::value)
                    && KT::assert_true (c, "is_assignable<int&,                int const&>",
                                        NT::is_assignable<int&,                int const&>::value)
                    && KT::assert_true (c, "is_assignable<int&,                int volatile&>",
                                        NT::is_assignable<int&,                int volatile&>::value)
                    && KT::assert_true (c, "is_assignable<int&,                int const volatile&>",
                                        NT::is_assignable<int&,                int const volatile&>::value)
                    && KT::assert_false(c, "is_assignable<int const&,          int&>",
                                        NT::is_assignable<int const&,          int&>::value)
                    && KT::assert_false(c, "is_assignable<int const&,          int const&>",
                                        NT::is_assignable<int const&,          int const&>::value)
                    && KT::assert_false(c, "is_assignable<int const&,          int volatile&>",
                                        NT::is_assignable<int const&,          int volatile&>::value)
                    && KT::assert_false(c, "is_assignable<int const&,          int const volatile&>",
                                        NT::is_assignable<int const&,          int const volatile&>::value)
                    && KT::assert_true (c, "is_assignable<int volatile&,       int&>",
                                        NT::is_assignable<int volatile&,       int&>::value)
                    && KT::assert_true (c, "is_assignable<int volatile&,       int const&>",
                                        NT::is_assignable<int volatile&,       int const&>::value)
                    && KT::assert_true (c, "is_assignable<int volatile&,       int volatile&>",
                                        NT::is_assignable<int volatile&,       int volatile&>::value)
                    && KT::assert_true (c, "is_assignable<int volatile&,       int const volatile&>",
                                        NT::is_assignable<int volatile&,       int const volatile&>::value)
                    && KT::assert_false(c, "is_assignable<int const volatile&, int&>",
                                        NT::is_assignable<int const volatile&, int&>::value)
                    && KT::assert_false(c, "is_assignable<int const volatile&, int const&>",
                                        NT::is_assignable<int const volatile&, int const&>::value)
                    && KT::assert_false(c, "is_assignable<int const volatile&, int volatile&>",
                                        NT::is_assignable<int const volatile&, int volatile&>::value)
                    && KT::assert_false(c, "is_assignable<int const volatile&, int const volatile&>",
                                        NT::is_assignable<int const volatile&, int const volatile&>::value)
                    ;
        }),
    KT::expect_success("copyable", [](KT::context& c)->bool{
                return KT::assert_false(c, "is_assignable<copyable,                 copyable>",
                                        NT::is_assignable<copyable,                 copyable>::value)
                    && KT::assert_true (c, "is_assignable<copyable,                 copyable const>",
                                        NT::is_assignable<copyable,                 copyable const>::value)
                    && KT::assert_false(c, "is_assignable<copyable,                 copyable volatile>",
                                        NT::is_assignable<copyable,                 copyable volatile>::value)
                    && KT::assert_false(c, "is_assignable<copyable,                 copyable const volatile>",
                                        NT::is_assignable<copyable,                 copyable const volatile>::value)
                    && KT::assert_false(c, "is_assignable<copyable&,                copyable>",
                                        NT::is_assignable<copyable&,                copyable>::value)
                    && KT::assert_true (c, "is_assignable<copyable&,                copyable const>",
                                        NT::is_assignable<copyable&,                copyable const>::value)
                    && KT::assert_false(c, "is_assignable<copyable&,                copyable volatile>",
                                        NT::is_assignable<copyable&,                copyable volatile>::value)
                    && KT::assert_false(c, "is_assignable<copyable&,                copyable const volatile>",
                                        NT::is_assignable<copyable&,                copyable const volatile>::value)
                    && KT::assert_false(c, "is_assignable<copyable const&,          copyable>",
                                        NT::is_assignable<copyable const&,          copyable>::value)
                    && KT::assert_false(c, "is_assignable<copyable const&,          copyable const>",
                                        NT::is_assignable<copyable const&,          copyable const>::value)
                    && KT::assert_false(c, "is_assignable<copyable const&,          copyable volatile>",
                                        NT::is_assignable<copyable const&,          copyable volatile>::value)
                    && KT::assert_false(c, "is_assignable<copyable const&,          copyable const volatile>",
                                        NT::is_assignable<copyable const&,          copyable const volatile>::value)
                    && KT::assert_false(c, "is_assignable<copyable volatile&,       copyable>",
                                        NT::is_assignable<copyable volatile&,       copyable>::value)
                    && KT::assert_false(c, "is_assignable<copyable volatile&,       copyable const>",
                                        NT::is_assignable<copyable volatile&,       copyable const>::value)
                    && KT::assert_false(c, "is_assignable<copyable volatile&,       copyable volatile>",
                                        NT::is_assignable<copyable volatile&,       copyable volatile>::value)
                    && KT::assert_false(c, "is_assignable<copyable volatile&,       copyable const volatile>",
                                        NT::is_assignable<copyable volatile&,       copyable const volatile>::value)
                    && KT::assert_false(c, "is_assignable<copyable const volatile&, copyable>",
                                        NT::is_assignable<copyable const volatile&, copyable>::value)
                    && KT::assert_false(c, "is_assignable<copyable const volatile&, copyable const>",
                                        NT::is_assignable<copyable const volatile&, copyable const>::value)
                    && KT::assert_false(c, "is_assignable<copyable const volatile&, copyable volatile>",
                                        NT::is_assignable<copyable const volatile&, copyable volatile>::value)
                    && KT::assert_false(c, "is_assignable<copyable const volatile&, copyable const volatile>",
                                        NT::is_assignable<copyable const volatile&, copyable const volatile>::value)
                    && KT::assert_true (c, "is_assignable<copyable,                 copyable&>",
                                        NT::is_assignable<copyable,                 copyable&>::value)
                    && KT::assert_true (c, "is_assignable<copyable,                 copyable const&>",
                                        NT::is_assignable<copyable,                 copyable const&>::value)
                    && KT::assert_false(c, "is_assignable<copyable,                 copyable volatile&>",
                                        NT::is_assignable<copyable,                 copyable volatile&>::value)
                    && KT::assert_false(c, "is_assignable<copyable,                 copyable const volatile&>",
                                        NT::is_assignable<copyable,                 copyable const volatile&>::value)
                    && KT::assert_true (c, "is_assignable<copyable&,                copyable&>",
                                        NT::is_assignable<copyable&,                copyable&>::value)
                    && KT::assert_true (c, "is_assignable<copyable&,                copyable const&>",
                                        NT::is_assignable<copyable&,                copyable const&>::value)
                    && KT::assert_false(c, "is_assignable<copyable&,                copyable volatile&>",
                                        NT::is_assignable<copyable&,                copyable volatile&>::value)
                    && KT::assert_false(c, "is_assignable<copyable&,                copyable const volatile&>",
                                        NT::is_assignable<copyable&,                copyable const volatile&>::value)
                    && KT::assert_false(c, "is_assignable<copyable const&,          copyable&>",
                                        NT::is_assignable<copyable const&,          copyable&>::value)
                    && KT::assert_false(c, "is_assignable<copyable const&,          copyable const&>",
                                        NT::is_assignable<copyable const&,          copyable const&>::value)
                    && KT::assert_false(c, "is_assignable<copyable const&,          copyable volatile&>",
                                        NT::is_assignable<copyable const&,          copyable volatile&>::value)
                    && KT::assert_false(c, "is_assignable<copyable const&,          copyable const volatile&>",
                                        NT::is_assignable<copyable const&,          copyable const volatile&>::value)
                    && KT::assert_false(c, "is_assignable<copyable volatile&,       copyable&>",
                                        NT::is_assignable<copyable volatile&,       copyable&>::value)
                    && KT::assert_false(c, "is_assignable<copyable volatile&,       copyable const&>",
                                        NT::is_assignable<copyable volatile&,       copyable const&>::value)
                    && KT::assert_false(c, "is_assignable<copyable volatile&,       copyable volatile&>",
                                        NT::is_assignable<copyable volatile&,       copyable volatile&>::value)
                    && KT::assert_false(c, "is_assignable<copyable volatile&,       copyable const volatile&>",
                                        NT::is_assignable<copyable volatile&,       copyable const volatile&>::value)
                    && KT::assert_false(c, "is_assignable<copyable const volatile&, copyable&>",
                                        NT::is_assignable<copyable const volatile&, copyable&>::value)
                    && KT::assert_false(c, "is_assignable<copyable const volatile&, copyable const&>",
                                        NT::is_assignable<copyable const volatile&, copyable const&>::value)
                    && KT::assert_false(c, "is_assignable<copyable const volatile&, copyable volatile&>",
                                        NT::is_assignable<copyable const volatile&, copyable volatile&>::value)
                    && KT::assert_false(c, "is_assignable<copyable const volatile&, copyable const volatile&>",
                                        NT::is_assignable<copyable const volatile&, copyable const volatile&>::value)
                    ;
        }),
    KT::expect_success("movable", [](KT::context& c)->bool{
                return KT::assert_true (c, "is_assignable<movable,                 movable>",
                                        NT::is_assignable<movable,                 movable>::value)
                    && KT::assert_false(c, "is_assignable<movable,                 movable const>",
                                        NT::is_assignable<movable,                 movable const>::value)
                    && KT::assert_false(c, "is_assignable<movable,                 movable volatile>",
                                        NT::is_assignable<movable,                 movable volatile>::value)
                    && KT::assert_false(c, "is_assignable<movable,                 movable const volatile>",
                                        NT::is_assignable<movable,                 movable const volatile>::value)
                    && KT::assert_true (c, "is_assignable<movable&,                movable>",
                                        NT::is_assignable<movable&,                movable>::value)
                    && KT::assert_false(c, "is_assignable<movable&,                movable const>",
                                        NT::is_assignable<movable&,                movable const>::value)
                    && KT::assert_false(c, "is_assignable<movable&,                movable volatile>",
                                        NT::is_assignable<movable&,                movable volatile>::value)
                    && KT::assert_false(c, "is_assignable<movable&,                movable const volatile>",
                                        NT::is_assignable<movable&,                movable const volatile>::value)
                    && KT::assert_false(c, "is_assignable<movable const&,          movable>",
                                        NT::is_assignable<movable const&,          movable>::value)
                    && KT::assert_false(c, "is_assignable<movable const&,          movable const>",
                                        NT::is_assignable<movable const&,          movable const>::value)
                    && KT::assert_false(c, "is_assignable<movable const&,          movable volatile>",
                                        NT::is_assignable<movable const&,          movable volatile>::value)
                    && KT::assert_false(c, "is_assignable<movable const&,          movable const volatile>",
                                        NT::is_assignable<movable const&,          movable const volatile>::value)
                    && KT::assert_false(c, "is_assignable<movable volatile&,       movable>",
                                        NT::is_assignable<movable volatile&,       movable>::value)
                    && KT::assert_false(c, "is_assignable<movable volatile&,       movable const>",
                                        NT::is_assignable<movable volatile&,       movable const>::value)
                    && KT::assert_false(c, "is_assignable<movable volatile&,       movable volatile>",
                                        NT::is_assignable<movable volatile&,       movable volatile>::value)
                    && KT::assert_false(c, "is_assignable<movable volatile&,       movable const volatile>",
                                        NT::is_assignable<movable volatile&,       movable const volatile>::value)
                    && KT::assert_false(c, "is_assignable<movable const volatile&, movable>",
                                        NT::is_assignable<movable const volatile&, movable>::value)
                    && KT::assert_false(c, "is_assignable<movable const volatile&, movable const>",
                                        NT::is_assignable<movable const volatile&, movable const>::value)
                    && KT::assert_false(c, "is_assignable<movable const volatile&, movable volatile>",
                                        NT::is_assignable<movable const volatile&, movable volatile>::value)
                    && KT::assert_false(c, "is_assignable<movable const volatile&, movable const volatile>",
                                        NT::is_assignable<movable const volatile&, movable const volatile>::value)
                    && KT::assert_false(c, "is_assignable<movable,                 movable&>",
                                        NT::is_assignable<movable,                 movable&>::value)
                    && KT::assert_false(c, "is_assignable<movable,                 movable const&>",
                                        NT::is_assignable<movable,                 movable const&>::value)
                    && KT::assert_false(c, "is_assignable<movable,                 movable volatile&>",
                                        NT::is_assignable<movable,                 movable volatile&>::value)
                    && KT::assert_false(c, "is_assignable<movable,                 movable const volatile&>",
                                        NT::is_assignable<movable,                 movable const volatile&>::value)
                    && KT::assert_false(c, "is_assignable<movable&,                movable&>",
                                        NT::is_assignable<movable&,                movable&>::value)
                    && KT::assert_false(c, "is_assignable<movable&,                movable const&>",
                                        NT::is_assignable<movable&,                movable const&>::value)
                    && KT::assert_false(c, "is_assignable<movable&,                movable volatile&>",
                                        NT::is_assignable<movable&,                movable volatile&>::value)
                    && KT::assert_false(c, "is_assignable<movable&,                movable const volatile&>",
                                        NT::is_assignable<movable&,                movable const volatile&>::value)
                    && KT::assert_false(c, "is_assignable<movable const&,          movable&>",
                                        NT::is_assignable<movable const&,          movable&>::value)
                    && KT::assert_false(c, "is_assignable<movable const&,          movable const&>",
                                        NT::is_assignable<movable const&,          movable const&>::value)
                    && KT::assert_false(c, "is_assignable<movable const&,          movable volatile&>",
                                        NT::is_assignable<movable const&,          movable volatile&>::value)
                    && KT::assert_false(c, "is_assignable<movable const&,          movable const volatile&>",
                                        NT::is_assignable<movable const&,          movable const volatile&>::value)
                    && KT::assert_false(c, "is_assignable<movable volatile&,       movable&>",
                                        NT::is_assignable<movable volatile&,       movable&>::value)
                    && KT::assert_false(c, "is_assignable<movable volatile&,       movable const&>",
                                        NT::is_assignable<movable volatile&,       movable const&>::value)
                    && KT::assert_false(c, "is_assignable<movable volatile&,       movable volatile&>",
                                        NT::is_assignable<movable volatile&,       movable volatile&>::value)
                    && KT::assert_false(c, "is_assignable<movable volatile&,       movable const volatile&>",
                                        NT::is_assignable<movable volatile&,       movable const volatile&>::value)
                    && KT::assert_false(c, "is_assignable<movable const volatile&, movable&>",
                                        NT::is_assignable<movable const volatile&, movable&>::value)
                    && KT::assert_false(c, "is_assignable<movable const volatile&, movable const&>",
                                        NT::is_assignable<movable const volatile&, movable const&>::value)
                    && KT::assert_false(c, "is_assignable<movable const volatile&, movable volatile&>",
                                        NT::is_assignable<movable const volatile&, movable volatile&>::value)
                    && KT::assert_false(c, "is_assignable<movable const volatile&, movable const volatile&>",
                                        NT::is_assignable<movable const volatile&, movable const volatile&>::value)
                    ;
        }),
    KT::expect_success("private_assign", [](KT::context& c)->bool{
            return KT::assert_false(c, "is_assignable<private_assign, private_assign>",
                                        NT::is_assignable<private_assign, private_assign>::value)
                    ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("type_traits::is_assignable", ac, av, ::tests);
}
