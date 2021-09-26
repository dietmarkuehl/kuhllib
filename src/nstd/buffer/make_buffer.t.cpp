// nstd/buffer/make_buffer.t.cpp                                      -*-C++-*-
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

#include "nstd/buffer/make_buffer.hpp"
#include "kuhl/test.hpp"

#include <vector>

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;
namespace NH = ::nstd::net::hidden_names;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct type
        {
            unsigned long long l{};
        };

        template <typename T>
        struct buffer
        {
            T*            data;
            ::std::size_t size;
            buffer(): data(), size() {}
            buffer(T* d, ::std::size_t s) : data(d) , size(s) {}
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("empty, non-const container", []{
            ::std::vector<TD::type> v;
            auto b = NH::make_buffer<TD::buffer<void>>(v);
            return KT::type<decltype(b)> == KT::type<TD::buffer<void>>
                && b.data == nullptr
                && b.size == 0u
                ;
        }),
    KT::expect_success("empty, const container", []{
            ::std::vector<TD::type> const v;
            auto b = NH::make_buffer<TD::buffer<void const>>(v);
            return KT::type<decltype(b)> == KT::type<TD::buffer<void const>>
                && b.data == nullptr
                && b.size == 0u
                ;
        }),
    KT::expect_success("non-empty, non-const container", []{
            ::std::vector<TD::type> v(9);
            auto b = NH::make_buffer<TD::buffer<void>>(v);
            return KT::type<decltype(b)> == KT::type<TD::buffer<void>>
                && b.data == v.data()
                && b.size == sizeof(TD::type) * v.size()
                ;
        }),
    KT::expect_success("non-empty, const container", []{
            ::std::vector<TD::type> const v(10);
            auto b = NH::make_buffer<TD::buffer<void const>>(v);
            return KT::type<decltype(b)> == KT::type<TD::buffer<void const>>
                && b.data == v.data()
                && b.size == sizeof(TD::type) * v.size()
                ;
        }),
};

static KT::add_tests suite("make_buffer", ::tests);
