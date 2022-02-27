// nstd/execution/set_stopped.t.cpp                                   -*-C++-*-
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

#include "nstd/execution/set_stopped.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace EX = ::nstd::execution;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    struct member_set_stopped {
        bool* const ptr;
        friend auto tag_invoke(EX::set_stopped_t, member_set_stopped&& r) noexcept -> void {
            *r.ptr = true;
        }
    };

    struct non_member_set_stopped {
        bool* const ptr;
    };
    auto tag_invoke(EX::set_stopped_t, non_member_set_stopped&& r) noexcept -> void {
        *r.ptr = true;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("member set_stopped", []{
           bool value(false);
           EX::set_stopped(TD::member_set_stopped{&value});
           return requires(::TD::member_set_stopped& r){ EX::set_stopped(::nstd::utility::move(r)); }
               && value
               ;
        }),
    KT::expect_success("non-member set_stopped", []{
           bool value(false);
           EX::set_stopped(TD::non_member_set_stopped{&value});
           return requires(::TD::non_member_set_stopped& r){ EX::set_stopped(::nstd::utility::move(r)); }
               && value
               ;
        }),
};

static KT::add_tests suite("set_stopped", ::tests);
