// nstd/execution/set_done.t.cpp                                      -*-C++-*-
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

#include "nstd/execution/set_done.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace EX = ::nstd::execution;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    struct member_set_done {
        bool* const ptr;
        void set_done() && { *ptr = true; }
    };
    void set_done(member_set_done&&) {}

    struct non_member_set_done {
        bool* const ptr;
    };
    void set_done(non_member_set_done&& r) { *r.ptr = true; }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("member set_done", []{
           bool value(false);
           EX::set_done(TD::member_set_done{&value});
           return requires(::TD::member_set_done& r){ EX::set_done(::nstd::utility::move(r)); }
               && value
               ;
        }),
    KT::expect_success("non-member set_done", []{
           bool value(false);
           EX::set_done(TD::non_member_set_done{&value});
           return requires(::TD::non_member_set_done& r){ EX::set_done(::nstd::utility::move(r)); }
               && value
               ;
        }),
};

static KT::add_tests suite("set_done", ::tests);
