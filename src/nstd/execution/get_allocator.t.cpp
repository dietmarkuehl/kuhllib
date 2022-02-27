// nstd/execution/get_allocator.t.cpp                                 -*-C++-*-
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

#include "nstd/execution/get_allocator.hpp"
#include "nstd/execution/receiver.hpp"
#include <memory>
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = test_declarations;
namespace EX = ::nstd::execution;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    template <typename Receiver>
    concept has_get_allocator
        = requires(Receiver const& r) { EX::get_allocator(r); }
        ;

    template <typename Allocator, bool Noexcept, bool IsReceiver = true>
    struct receiver {
        bool* const value;
        friend auto tag_invoke(EX::set_stopped_t, receiver&&) noexcept(IsReceiver) {}
        friend auto tag_invoke(EX::set_error_t, receiver&&, std::exception_ptr) noexcept {}
        friend auto tag_invoke(EX::get_allocator_t, receiver const& r) noexcept(Noexcept) {
            *r.value = true;
            return ::std::allocator<int>();
        }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("get_allocator breathing", []{
        return KT::assert_type_exists<EX::get_allocator_t>
            && KT::type<EX::get_allocator_t const> == KT::type<decltype(EX::get_allocator)>
            ;
    }),
    KT::expect_success("test declarations behave as expected", []{
            return EX::receiver<TD::receiver<std::allocator<int>, true>>
                && EX::receiver<TD::receiver<std::allocator<int>, false>>
                && !EX::receiver<TD::receiver<std::allocator<int>, true, false>>
                ;
        }),
    KT::expect_success("receiver returning a allocator has get_allocator", []{
            bool                              value(false);
            TD::receiver<::std::allocator<int>, true> r{&value};
            EX::get_allocator(r);
            return TD::has_get_allocator<TD::receiver<::std::allocator<int>, true>>
                && KT::type<decltype(EX::get_allocator(r))> == KT::type<::std::allocator<int>>
                && value
                ;
        }),
    KT::expect_success("non-receiver doesn't have get_allocator", []{
            return !TD::has_get_allocator<TD::receiver<::std::allocator<int>, true, false>>
                ;
        }),
    KT::expect_success("throwing get_allocator isn't allowed ", []{
            return !TD::has_get_allocator<TD::receiver<::std::allocator<int>, false>>
                ;
        }),
};

static KT::add_tests suite("get_allocator", ::tests);